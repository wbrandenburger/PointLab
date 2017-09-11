/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017  Wolfgang Brandenburger. All rights reserved.
*
* THE BSD LICENSE
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/

#ifndef UTILS_HEAP_H_
#define UTILS_HEAP_H_

#include <vector>
#include <boost/atomic.hpp>

namespace utils
{

	inline size_t computeInitialSize(size_t size_) 
	{
		return std::pow(2, std::ceil(std::log2(size_ + 1))) - 1;
	} 

	inline size_t computeInitialSizeConcurrent(size_t size_, size_t cores)
	{
		return std::pow(2, std::ceil(std::log2(std::floor(size_ / cores) + 1 ))) - 1;
	}

	template<typename ElementType> struct HeapNode {

		/**
			Constructor
		*/
		HeapNode() : value(NULL), index(NULL) {}

		/**
			Constructor

			@param[in] value_ Element
			@param[in] index_ Index in heaparray
		*/
		HeapNode(ElementType value_, size_t index_) : value(value_), index(index_) {}

		/**
			Deconstructor
		*/
		~HeapNode() {}

		/**
			Operator < Compares two HeapNodes

			@param[in] heap_node_ Instance of HeapNode
		*/
		virtual bool operator < (const HeapNode<ElementType>& heap_node_)
		{
			return value < heap_node_.value ? true : false;
		}

		/**
			Operator < Compares two HeapNodes

			@param[in] heap_node_ Instance of HeapNode
		*/
		virtual bool operator <= (const HeapNode<ElementType>& heap_node_)
		{
			return value <= heap_node_.value ? true : false;
		}

		/**
			Operator < Compares two HeapNodes

			@param[in] heap_node_ Instance of HeapNode
		*/
		virtual bool operator > (const HeapNode<ElementType>& heap_node_)
		{
			return value > heap_node_.value ? true : false;
		}
		
		/**
			Operator < Compares two HeapNodes

			@param[in] heap_node_ Instance of HeapNode
		*/
		virtual bool operator >= (const HeapNode<ElementType>& heap_node_)
		{
			return value >= heap_node_.value ? true : false;
		}
		/**
			Clears the structure
		*/
		virtual void clear()
		{
			value = NULL;
			index = NULL;
		}

		/** 
			Return True if the structure is empty

			@return True if tje structure is empty
		*/
		bool isEmpty()
		{
			if (value == NULL && index == NULL ) {
				return true;
			}

			return false;
		}

		/** 
			Value
		*/
		ElementType value;

		/**
			Index
		*/
		size_t index;

	};

	template<typename ElementType>
	struct HeapNodeConcurrent : HeapNode<ElementType> {

		//boost::atomic<bool> lock;

		/**
			Constructor
		*/
		HeapNodeConcurrent() : HeapNode(), left_neighbor(nullptr), right_neighbor(nullptr) {}

		/**
			Deconstructor
		*/
		~HeapNodeConcurrent() {}

		/** 
			Clears the structure
		*/
		void clear() {
			value = NULL;
			index = NULL;

			left_neighbor = nullptr;
			right_neighbor = nullptr;
		}

		/**
			Assign operator

			@param[in] heapnode_ Reference of the element which has to be copied 
		*/
		void operator=(const HeapNodeConcurrent<ElementType>& heapnode_)
		{
			value = heapnode_.value;
			index = heapnode_.index;

			left_neighbor = heapnode_.left_neighbor;
			right_neighbor = heapnode_.right_neighbor;
		}

		/**
			Pointer to the left neighbor
		*/
		HeapNodeConcurrent<ElementType>* left_neighbor;
		
		/**
			Pointer to the left neighbor
		*/
		HeapNodeConcurrent<ElementType>* right_neighbor;
	};

	template <typename ElementType>
	struct HeapSuperNodeConcurrent 
	{	
		/**
			Constructor
		*/
		HeapSuperNodeConcurrent() : first_node(nullptr), last_node(nullptr), lock(0) {}

		void clear()
		{
			first_node = nullptr;
			last_node = nullptr;

			lock.store(0, boost::memory_order_seq_cst/*boost::memory_order_relaxed*/);
		}

		/**
			Lock this index

			@return True if index could be locked
		*/
		bool lockIndex() 
		{
			bool lock_value = 0;

			return lock.compare_exchange_weak(lock_value, 1, boost::memory_order_seq_cst/*boost::memory_order_relaxed*/);
		}

		/**
			Unlock this index

			@return True if index could be unlocked
		*/
		bool unlockIndex()
		{
			bool unlock_value = 1;

			return lock.compare_exchange_weak(unlock_value, 0, boost::memory_order_seq_cst/*boost::memory_order_relaxed*/);
		}
		
		/**
			Pull an element

			@param[in,out] pull_node_ Pointer to the element which has to be evaluated
			@param[in,out] push_node_ Pointer to the element which has to be sorted in the previous node
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		void pull(HeapNodeConcurrent<ElementType>** pull_node_, HeapNodeConcurrent<ElementType>** push_node_, bool greater_ = true)
		{
			if (greater_) {
				if (**pull_node_ > *first_node) {
					*push_node_ = *pull_node_;
					*pull_node_ = nullptr;
				}
				else if (**pull_node_ <= *last_node) {
					*push_node_ = first_node;

					first_node = (*first_node).right_neighbor;
					(*first_node).left_neighbor = nullptr;
				}
				else {
					*push_node_ = first_node;

					first_node = (*first_node).right_neighbor;
					(*first_node).left_neighbor = nullptr;

					sort(*pull_node_, greater_);

					*pull_node_ = nullptr;
				}
			}
			else {
				if (**pull_node_ < *first_node) {
					*push_node_ = *pull_node_;
					*pull_node_ = nullptr;
				}
				else if (**pull_node_ >= *last_node) {
					*push_node_ = first_node;

					first_node = (*first_node).right_neighbor;
					(*first_node).left_neighbor = nullptr;
				}
				else {
					*push_node_ = first_node;

					first_node = (*first_node).right_neighbor;
					(*first_node).left_neighbor = nullptr;

					sort(*pull_node_, greater_);

					*pull_node_ = nullptr;
				}
			}
		}
		
		/**
			Push an element

			@param[in,out] push_node_ Pointer to the element which has to be evaluated
			@param[in,out] pull_node_ Pointer to the element which has to be sorted in the previous node
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		void push(HeapNodeConcurrent<ElementType>** push_node_, HeapNodeConcurrent<ElementType>** pull_node_, bool greater_ = true)
		{
			if (greater_){
				if (**push_node_ <= *last_node) {
					*pull_node_ = *push_node_;
					*push_node_ = nullptr;
				}
				else if(**push_node_ > *first_node) {
					*pull_node_ = last_node;

					last_node = (*last_node).left_neighbor;
					(*last_node).right_neighbor = nullptr;
				}
				else {
					*pull_node_ = last_node;

					last_node = (*last_node).left_neighbor;
					(*last_node).right_neighbor = nullptr;

					sort(*push_node_, greater_);

					*push_node_ = nullptr;
				}
			}
			else {
				if (**push_node_ >= *last_node) {
					*pull_node_ = *push_node_;
					*push_node_ = nullptr;
				}
				else if (**push_node_ > *first_node) {
					*pull_node_ = last_node;

					last_node = (*last_node).left_neighbor;
					(*last_node).right_neighbor = nullptr;
				}
				else {
					*pull_node_ = last_node;

					last_node = (*last_node).left_neighbor;
					(*last_node).right_neighbor = nullptr;

					sort(*push_node_, greater_);

					*push_node_ = nullptr;
				}
			}
		}

		/**
			Sort a element in the list

			@param[in] node_ Pointer to the element which has to be sorted in the list
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		void sort(HeapNodeConcurrent<ElementType>* node_, bool greater_ = true)
		{
			if (greater_) {
				if (first_node) {
					if ( *node_ <= *first_node) {
						HeapNodeConcurrent<ElementType>* next_node = first_node;
						while (next_node) {
							if (*node_ <= *next_node) {
								next_node = (*next_node).right_neighbor;
							}
							else {
								(*(*next_node).left_neighbor).right_neighbor = node_;
								(*node_).left_neighbor = (*next_node).left_neighbor;
								(*next_node).left_neighbor = node_;
								(*node_).right_neighbor = next_node;

								return;
							}
						}

						(*node_).left_neighbor = last_node;

						(*last_node).right_neighbor = node_;

						last_node = node_;
					}
					else {
						(*node_).right_neighbor = first_node;

						(*first_node).left_neighbor = node_;

						first_node = node_;
					}
				}
				else {
					first_node = node_;
					last_node = node_;
				}
			}
			else {
				if (first_node) {
					if (*node_ >= *first_node) {
						HeapNodeConcurrent<ElementType>* next_node = first_node;
						while (next_node) {
							if (*node_ <= *next_node) {
								next_node = (*next_node).right_neighbor;
							}
							else {
								(*(*next_node).left_neighbor).right_neighbor = node_;
								(*node_).left_neighbor = (*next_node).left_neighbor;
								(*next_node).left_neighbor = node_;
								(*node_).right_neighbor = next_node;

								return;
							}
						}

						(*node_).left_neighbor = last_node;

						(*last_node).right_neighbor = node_;

						last_node = node_;

					}
					else {
						(*node_).right_neighbor = first_node;

						(*first_node).left_neighbor = node_;
					
						first_node = node_;
					}
				}
				else {
					first_node = node_;
					last_node = node_;
				}
			}
		}

		/**
			Pointer to the first element of the list
		*/
		HeapNodeConcurrent<ElementType>* first_node;
		
		/**
			Pointer to the first element of the list
		*/
		HeapNodeConcurrent<ElementType>* last_node;
		/**
			Lock
		*/
		boost::atomic<bool> lock;
	};

	template<typename ElementType>
	class BaseHeap 
	{

	public:

		/**
			Constructor
		*/
		BaseHeap () : heaparray(nullptr), size(0), count(0), greater(true) {}
		/**
			Constructor

			@param[in] size_ size of the heaparray which has to be built
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		BaseHeap(size_t size_, bool greater_ = true) : greater(greater_), count(0) {
			size = computeInitialSize(size_);
			heaparray = new HeapNode<ElementType>[size];
		}
		
		/**
			Get the number of elements in the heaparray

			@return Number of elements
		*/
		size_t getElements() {
			return count;
		}

		/**
			Sets the pointer heaparray and size

			@param[in] size_ Size of the heaparray
		*/
		virtual void setHeap(size_t size_) = 0;

		/**
			Sets the array and the size
		*/
		virtual void setHeap(void* pointer_, size_t size_) = 0;

		/**
			Sets the elements to zero
		*/
		virtual void clear() = 0;


		/**
			Get size of a node in the array

			@return Size of a node in the array
		*/	
		size_t getSize()
		{
			return sizeof(HeapNode<ElementType>);
		}

		/**
			Checks whether the elements in the heaparray are ordered

			@return True when the heaparray is ordered
		*/
		bool checkHeap() {
			size_t begin = 0;
			size_t depth = 0;
			while (begin < (size + 1) / 2 - 1) {
				size_t elements = (size_t)pow((float)2, (float)depth);
				for (size_t i = 0; i < elements; i++) {
					if (!heaparray[2 * begin + 1].isEmpty()) {
						if (greater) {
							if (heaparray[begin] < heaparray[2 * begin + 1]) { return 0; }
						}
						else {
							if (heaparray[begin] > heaparray[2 * begin + 1]) { return 0; }
						}
					}
					if (!heaparray[2 * begin + 2].isEmpty()) {
						if (greater) {
							if (heaparray[begin] < heaparray[2 * begin + 2]) { return 0; }
						}
						else {
							if (heaparray[begin] > heaparray[2 * begin + 2]) { return 0; }
						}
					}
					begin = begin + 1;
				}
				depth = depth + 1;
			}
			return 1;
		}
	
	protected:

		/**
			Resizes the heaparray

			@param[in] size_ of the heaparray
		*/
		virtual void resize(size_t size_) = 0;


		/**
			Swap two heaparray elements

			@param[in] x first heaparray element
			@param[in] y second heaparray element
		*/
		virtual void swap(size_t index_, size_t new_index) = 0;

		/**
			Swap two heaparray elements

			@param[in] x first heaparray element
			@param[in] y second heaparray element
		*/
		void swapElements(HeapNode<ElementType>& x, HeapNode<ElementType>& y)
		{
			HeapNode<ElementType> swap = x;
			x = y;
			y = swap;
		}

		/**
			Push up a element in the heaparray

			@param[in] index_ index of the element which has to push up
			@return True when pushing the element up was successful
		*/
		virtual bool pushup(size_t index_) {

			bool flag = false;

			while (index_ != 0) {
				size_t new_index;
				if (index_ % 2 == 0) {
					new_index = (index_ / 2) - 1;
				}
				else {
					new_index = (index_ - 1) / 2;
				}

				if (greater) {
					if (heaparray[new_index] < heaparray[index_]) {
						swap(new_index, index_);
						flag = true;
					}
					else { return flag; }
				}
				else {
					if (heaparray[new_index] > heaparray[index_]) {
						swap(new_index, index_);
						flag = true;
					}
					else { return flag; }
				}
				index_ = new_index;
			}

			return flag;
		}

		/**
			Pull down a element in the heaparray

			@param[in] index_ index of the element which has to pull down
			@return True when pulling the element down was successful
		*/
		virtual bool pulldown(size_t index_) {

			bool flag = false;

			while (index_ < std::pow(2, std::floor(std::log2(count))) - 1) { //(size + 1) / 2 - 1
				
				size_t new_index;
				if (!heaparray[2 * index_ + 1].isEmpty() && !heaparray[2 * index_ + 2].isEmpty()) {
					new_index = heaparray[2 * index_ + 1] > heaparray[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
				}
				else if (!heaparray[2 * index_ + 1].isEmpty() && heaparray[2 * index_ + 2].isEmpty()) {
					new_index = 2 * index_ + 1;
				}
				else if (heaparray[2 * index_ + 1].isEmpty() && !heaparray[2 * index_ + 2].isEmpty()) {
					new_index = 2 * index_ + 2;
				}
				else {
					return flag;
				}

				if (greater) {
					if (heaparray[index_] < heaparray[new_index]) {
						swap(index_, new_index);
						index_ = new_index;
						flag = true;
					}
					else {
						return flag;
					}
				}
				else {
					if (heaparray[index_] > heaparray[new_index]) {
						swap(index_, new_index);
						index_ = new_index;
						flag = true;
					}
					else {
						return flag;
					}
				}
			}

			return flag;
		}

	public:

		/**
			Adds a new element

			@param[in] value_ Element which will be added
			@param[in] index_ Index ind the list of elements
		*/
		virtual void push(ElementType value_, size_t index_ = NULL) = 0;

		/**
			Pops the minimal/maximal element;

			@return minimal/maximal value of the heap
		*/
		virtual ElementType pop() = 0;

	public:

		/**
			heaparray with the size of 2^n-1
		*/
		HeapNode<ElementType>* heaparray;

		/**
			Size of heaparray
		*/
		size_t size;

		/**
			Number of elements in heap
		*/
		size_t count;

		/**
			Flag which specifies wheter the set will be descendendly ordered
		*/
		bool greater;
	};

	template<typename ElementType>
	class Heap : public BaseHeap<ElementType> {

	public:

		/**
			Constructor
		*/
		Heap() : BaseHeap(){}
		
		/**
			Constructor

			@param[in] size_ size of the heaparray which has to be built
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		Heap(size_t size_, bool greater_ = true) : BaseHeap(size_, greater) {
		}
	
		/**
			Desconstructor
		*/
		~Heap() { 
			delete[] heaparray; 
		}

		/**
			Sets the pointer heaparray and size 

			@param[in] size_ Size of the heaparray
		*/
		void setHeap(size_t size_) {

			if (heaparray) {
				delete[] heaparray;
			}

			size = computeInitialSize(size_);
			heaparray = new HeapNode<ElementType>[size];
			count = 0;
		}

		/**
			Sets the array and the size
		*/
		void setHeap(void* pointer_, size_t size_)
		{
			if (heaparray) {
				delete[] heaparray;
			}

			size = size_;
			heaparray = (HeapNode<ElementType>*) pointer_;
			count = 0;
		}

		/**
			Sets the elements to zero
		*/
		void clear() {
			for (int i = 0; i < count; i++) {
				heaparray[i].clear();
			}

			count = 0;
		}

	private:
		
		/**
			Resizes the heaparray

			@param[in] size_ of the heaparray
		*/
		void resize(size_t size_) {
			HeapNode<ElementType>* new_heaparray = new HeapNode<ElementType>[size_];

			for (int i = 0; i<size + 1; i++) {
				new_heaparray[i] = heaparray[i];
			}

			delete[] heaparray;
			heaparray = new_heaparray;
			size = size_;
		}

		/**
			Swap two heaparray elements

			@param[in] x first heaparray element
			@param[in] y second heaparray element
		*/
		void swap(size_t index_, size_t new_index)
		{
			swapElements(heaparray[new_index], heaparray[index_]);
		}

	public:

		/**
			Adds a new element

			@param[in] value_ element which will be added
		*/
		void push(ElementType value_, size_t index_ = NULL) {
			if (count > size) {
				resize(size * 2 + 1);
			}

			heaparray[count].value = value_;
			pushup(count);

			count = count + 1;
		}

		/**
			Pops the minimal/maximal element;

			@return minimal/maximal value of the heap
		*/
		ElementType pop() {
			if (count == 0) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			ElementType value = heaparray[0].value;

			if (count == 1) {
				heaparray[0].clear();
				count--;
			}
			else {
				heaparray[0] = heaparray[count - 1];
				heaparray[count - 1].clear();
				count--;
				pulldown(0);
			}


			return value;
		}

	};	

	template<typename ElementType>
	class HeapWrapper : public BaseHeap<ElementType> 
	{
	public:

		/**
			Constructor
		*/
		HeapWrapper() : BaseHeap() {}

		/**
			Constructor

			@param[in] size_ size of the heaparray which has to be built
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		HeapWrapper(size_t size_, bool greater_ = true) : BaseHeap(size_) {
			heapvector.resize(computeInitialSize(size_));
		}

		/**
			Desconstructor
		*/
		~HeapWrapper() 
		{ 
			delete[] heaparray; 
			heapvector.clear();
		}

		/**
			Sets the pointer heaparray and size

			@param[in] size_ Size of the heaparray
		*/
		void setHeap(size_t size_) {

			if (heaparray) {
				delete[] heaparray;
			}
			heapvector.clear();

			size = computeInitialSize(size_);
			heaparray = new HeapNode<ElementType>[size];
			heapvector.resize(size);

			count = 0;
		}

		/**
			Sets the array and the size
		*/
		void setHeap(void* pointer_, size_t size_)
		{
			if (heaparray) {
				delete[] heaparray;
			}

			size = size_;
			heaparray = (HeapNode<ElementType>*) pointer_;
			count = 0;
		}

		/**
			Sets the elements to zero
		*/
		void clear() {
			for (int i = 0; i < count; i++) {
				heaparray[i].clear();
			}

			heapvector.clear();

			count = 0;
		}

		/**
			Get the element of the specified index

			@param[in] index_ Index of the element
			@return Value of the element
		*/
		ElementType getElement(size_t index_)
		{
			return heaparray[heapvector[index_]].value;
		}

		/**
			Get the index in the array of the specified index

			@param[in] index_ Index of the element
			@return Index in the array
		*/
		size_t getIndex(size_t index_)
		{
			return heaparray[heapvector[index_]].index;
		}

	private:

		/**
			Resizes the heaparray

			@param[in] size_ of the heaparray
		*/
		void resize(size_t size_) {
			HeapNode<ElementType>* new_heaparray = new HeapNode<ElementType>[size_];

			for (int i = 0; i<size + 1; i++) {
				new_heaparray[i] = heaparray[i];
			}

			delete[] heaparray;
			heaparray = new_heaparray;

			heapvector.resize(size_);

			size = size_;
		}

		/**
			Swap two heaparray elements

			@param[in] x first heaparray element
			@param[in] y second heaparray element
		*/
		void swap(size_t index_, size_t new_index)
		{
			swapElements(heaparray[new_index], heaparray[index_]);
			
			heapvector[heaparray[new_index].index] = new_index;
			heapvector[heaparray[index_].index] = index_;
		}

	public:

		/**
			Adds a new element

			@param[in] value_ element which will be added
			@param[in] index_ Index ind the list of elements
		*/
		void push(ElementType value_, size_t index_) 
		{
			if (count > size) {
				resize(size * 2 + 1);
			}

			HeapNode<ElementType> heap_node(value_, index_);
			heaparray[count] = heap_node;
			
			heapvector[index_] = count;

			pushup(count);

			count = count + 1;
		}

		/**
			Pops the minimal/maximal element;

			@return minimal/maximal value of the heap
		*/
		ElementType pop() 
		{
			if (count == 0) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			ElementType value = heaparray[0].value;
			heapvector[heaparray[0].index] = NULL;

			if (count == 1) {
				heaparray[0].clear();
				count = 0;
			}
			else {
				heaparray[0] = heaparray[count - 1];
				heapvector[heaparray[0].index] = 0;
				heaparray[count - 1].clear();
				count-- ;
				pulldown(0);
			}

			return value;
		}

		/**
			Update a value

			@param[in] index_ Index of the element
		*/
		void update(ElementType value_, size_t index_)
		{
			heaparray[heapvector[index_]].value = value_;

			if (!pushup(heapvector[index_]))
			{
				pulldown(heapvector[index_]);
			}
			
		}

	private:

		/**
			Vector indexing the heaparray
		*/
		std::vector<size_t> heapvector;
	};

	template<typename ElementType>
	class BaseHeapConcurrent
	{

	public:

		/**
			Constructor
		*/
		BaseHeapConcurrent() : heaparray(nullptr), size(0), cores(0), count(0), greater(true) {}

		/**
			Constructor

			@param[in] size_ size of the heaparray which has to be built
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		BaseHeapConcurrent(size_t size_, size_t cores_, bool greater_ = true) : cores(cores_), greater(greater_), count(0)
		{
			size = computeInitialSizeConcurrent(size_, cores_);
			heaparray = new HeapSuperNodeConcurrent<ElementType>[size];
		}

		/**
			Get the number of elements in the heaparray

			@return Number of elements
		*/
		size_t getElements()
		{
			return count.load(boost::memory_order_seq_cst);
		}

	//	/**
	//		Sets the pointer heaparray and size

	//		@param[in] size_ Size of the heaparray
	//	*/
	//	virtual void setHeap(size_t size_, size_t cores) = 0;
	//	
	//	/**
	//		Sets the array and the size
	//	*/
	//	virtual void setHeap(void* pointer_, size_t size_, size_t cores_) = 0;

		///**
		//	Sets the elements to zero
		//*/
		//virtual void clear() = 0;

		/**
			Checks whether the elements in the heaparray are ordered

			@return True when the heaparray is ordered
		*/
		bool checkHeap()
		{
			return checkHeapRecursive(0);
		}

		/**
			Checks recursively whether the elements in the heaparray are ordered

			@return True when the heaparray is ordered
		*/
		bool checkHeapRecursive(size_t index_)
		{
			if (heaparray[index_].last_node) {
				size_t child_index_left = 2 * index_ + 1;
				if (child_index_left >= size) {
					return true;
				}
				size_t child_index_right = 2 * index_ + 2;
				if (child_index_right >= size) {
					return true;
				}

				if (greater) {
					if (heaparray[child_index_left].first_node) {
						if (*(heaparray[index_].last_node) >= *(heaparray[child_index_left].first_node)) {
							return checkHeapRecursive(child_index_left);
						}
						else {
							return false;
						}
					}
					if (heaparray[child_index_right].first_node) {
						if (*(heaparray[index_].last_node) >= *(heaparray[child_index_right].first_node)) {
							return checkHeapRecursive(child_index_left);
						}
						else {
							return false;
						}
					}
				}
				else {
					if (heaparray[child_index_left].first_node) {
						if (*(heaparray[index_].last_node) <= *(heaparray[child_index_left].first_node)) {
							return checkHeapRecursive(child_index_left);
						}
						else {
							return false;
						}
					}
					if (heaparray[child_index_right].first_node) {
						if (*(heaparray[index_].last_node) <= *(heaparray[child_index_right].first_node)) {
							return checkHeapRecursive(child_index_left);
						}
						else {
							return false;
						}
					}

				}

				HeapNodeConcurrent<ElementType>* list_node = heaparray[index_].first_node;
				while (list_node) {
					if ((*list_node).right_neighbor) {
						if (greater) {
							if (*list_node >= *(*list_node).right_neighbor){
								list_node = (*list_node).right_neighbor;
							}
							else {
								return false;
							}
						}
						else {
							if (*list_node <= *(*list_node).right_neighbor) {
								list_node = (*list_node).right_neighbor;
							}
							else {
								return false;
							}
						}
					}
				}

			}
			else {
				return true;
			}
		}

	protected:

		/**
			Push up a element in the heaparray

			@param[in] index_ Index of the current node
			@param[in] node_ Pointer to the element which has to be sorted in
		*/
		virtual void pushUp(size_t index_, HeapNodeConcurrent<ElementType>* node_) 
		{	
			HeapNodeConcurrent<ElementType>* sortin_node = nullptr;
			
			size_t parent_index;
			if (index_ % 2 == 0) {
				parent_index = index_ / 2 - 1;
			}
			else {
				parent_index = ( index_ - 1 ) / 2;
			}

			if (index_ > 0) {
				while (!heaparray[parent_index].lockIndex());
					heaparray[parent_index].push(&node_, &sortin_node, greater);
				while (!heaparray[parent_index].unlockIndex());
			}
			else {
				while (!heaparray[index_].lockIndex());
					heaparray[index_].sort(node_, greater);
				while (!heaparray[index_].unlockIndex());
				return;
			}

			while (!heaparray[index_].lockIndex());
				heaparray[index_].sort(sortin_node, greater);
			while (!heaparray[index_].unlockIndex());

			if (node_) {
				pushUp(parent_index, node_);
			}
		}

		/**
			Pull down a element in the heaparray
		
			@param[in] index_ Index of the current node
			@param[in] node_ Pointer to the element which has to be sorted in
		*/
		virtual void pullDown(size_t index_, HeapNodeConcurrent<ElementType>* node_)
		{
			//HeapNodeConcurrent<ElementType>* sortin_node = nullptr;

			//size_t child_index;
			//size_t child_left = index_ * 2 + 1;
			//size_t child_right = index_ * 2 + 2;

			//if (child_left < size){
			//	while (!heaparray[child_left].lockIndex());
			//    while (!heaparray[child_right].lockIndex());

			//		if (greater) {
			//			if (*heaparray[child_left].first_node >= *heaparray[child_right].first_node) {
			//				while (!heaparray[child_right].unlockIndex());
			//				child_index = child_left;
			//			}
			//			else {
			//				while (!heaparray[child_left].unlockIndex());
			//				child_index = child_right;
			//			}
			//		}
			//		else {
			//			if (*heaparray[child_left].first_node <= *heaparray[child_right].first_node) {
			//				while (!heaparray[child_right].unlockIndex());
			//				child_index = child_left;
			//			}
			//			else {
			//				while (!heaparray[child_left].unlockIndex());
			//				child_index = child_right;
			//			}
			//		}
			//		
			//		heaparray[child_index].pull(&node_, &sortin_node, greater);

			//	while (!heaparray[child_index].unlockIndex());

			//	while (!heaparray[index_].lockIndex());
			//		heaparray[index_].sort(sortin_node, greater);
			//	while (!heaparray[index_].unlockIndex());
			//}
			//else {
			//	while (!heaparray[index_].lockIndex());
			//		heaparray[index_].sort(node_, greater);
			//	while (!heaparray[index_].unlockIndex());
			//	return;
			//}

			//if (node_) {
			//	pullDown(child_index, node_);
			//}
		}

	public:

		/**
			Adds a new element

			@param[in] value_ Element which will be added
			@param[in] index_ Index ind the list of elements
		*/
		virtual void push(ElementType value_, size_t index_ = NULL) = 0;

		/**
			Pops the minimal/maximal element;

			@return minimal/maximal value of the heap
		*/
		virtual ElementType pop() = 0;

	public:

		/**
			heaparray with the size of 2^n-1
		*/
		HeapSuperNodeConcurrent<ElementType>* heaparray;

		/**
			Size of heaparray
		*/
		size_t size;

		/**
			Number of parallel cores working in heap
		*/
		size_t cores;

		/**
			Number of elements in heap
		*/
		boost::atomic<int> count;

		/**
			Flag which specifies wheter the set will be descendendly ordered
		*/
		bool greater;
	};

	template<typename ElementType>
	class HeapConcurrent : public BaseHeapConcurrent<ElementType> {

	public:

		/**
			Constructor
		*/
		HeapConcurrent() : BaseHeapConcurrent() {}

		/**
			Constructor

			@param[in] size_ size of the heaparray which has to be built
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		HeapConcurrent(size_t size_, size_t cores_, bool greater_ = true) : BaseHeapConcurrent(size_, cores_, greater_) {
		}

		/**
			Desconstructor
		*/
		~HeapConcurrent() {
			delete[] heaparray;
		}

	//	/**
	//		Sets the pointer heaparray and size

	//		@param[in] size_ Size of the heaparray
	//	*/
	//	void setHeap(size_t size_, size_t cores_) {

	//		if (heaparray) {
	//			delete[] heaparray;
	//			delete[] nodelocks;
	//		}
	//		
	//		cores = cores_;
	//		size = computeInitialSizeConcurrent(size_, cores_);
	//		heaparray = new HeapNodeConcurrent<ElementType>[size];
	//		nodelocks = new boost::atomic<bool>[std::floor(size / cores)];

	//		for (int i = 0; i < std::floor(size / cores) + 1; i++) {
	//			nodelocks[i].store(0, boost::memory_order_seq_cst /*boost::memory_order_relaxed*/);
	//		}

	//		count = 0;
	//	}

	//	/**
	//		Sets the array and the size
	//	*/
	//	void setHeap(void* pointer_, size_t size_, size_t cores_)
	//	{
	//		if (heaparray) {
	//			delete[] heaparray;
	//			delete[] nodelocks;
	//		}

	//		cores = cores_;
	//		size = size_;
	//		heaparray = (HeapNodeConcurrent<ElementType>*) pointer_;
	//		nodelocks = new boost::atomic<bool>[std::floor(size / cores)];

	//		for (int i = 0; i < std::floor(size / cores) + 1; i++) {
	//			nodelocks[i].store(0, boost::memory_order_seq_cst /*boost::memory_order_relaxed*/);
	//		}

	//		count = 0;
	//	}

		///**
		//	Sets the elements to zero
		//*/
		//void clear() {
		//	for (int i = 0; i < count; i++) {
		//		heaparray[i].clear();
		//	}

		//	for (int i = 0; i < std::floor(size / cores) + 1; i++) {
		//		nodelocks[i].store(0, boost::memory_order_seq_cst /*boost::memory_order_relaxed*/);
		//	}

		//	count = 0;
		//}

	public:

		/**
			Adds a new element

			@param[in] value_ element which will be added
			@param[in] index_ the respective index for assigning purposes
		*/
		void push(ElementType value_, size_t index_ = NULL)
		{
			int count_value = count.fetch_add(1, boost::memory_order_seq_cst /*boost::memory_order_relaxed*/);

			if (count_value > size * cores) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			HeapNodeConcurrent<ElementType>* node = new HeapNodeConcurrent<ElementType>;
			(*node).value = value_;
			(*node).index = index_;

			pushUp(std::floor(count_value / cores), node);
		}

		/**
			Pops the minimal/maximal element;

			@return minimal/maximal value of the heap
		*/
		ElementType pop() 
		{
			if (count == 0) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			while (!heaparray[0].lockIndex());
				ElementType value = heaparray[0].first_node->value;
				heaparray[0].first_node = heaparray[0].first_node->right_neighbor;
				heaparray[0].first_node->left_neighbor = nullptr;
			while (!heaparray[0].unlockIndex());

			int count_value = count.fetch_sub(1, boost::memory_order_seq_cst /*boost::memory_order_relaxed*/);
			size_t index = std::floor(count_value / cores);
			while (!heaparray[index].lockIndex());
				HeapNodeConcurrent<ElementType>* node = heaparray[index].last_node;
				heaparray[index].last_node = heaparray[index].last_node->left_neighbor;
				heaparray[index].last_node->right_neighbor = nullptr;

				(*node).left_neighbor = nullptr;
			while (!heaparray[index].unlockIndex());
				
			pullDown(0, node);

			return value;
		}
	};

	//template<typename ElementType>
	//class HeapWrapperConcurrent : public BaseHeapConcurrent<ElementType> 
	//{
	//public:

	//	/**
	//		Constructor
	//	*/
	//	HeapWrapperConcurrent() : BaseHeapConcurrent() {}

	//	/**
	//		Constructor

	//		@param[in] size_ size of the heaparray which has to be built
	//		@param[in] greater Flag which specifies whether the set will be descendendly ordered
	//	*/
	//	HeapWrapperConcurrent(size_t size_, size_t cores_, bool greater_ = true) : BaseHeapConcurrent(size_, cores_, greater) {
	//		heapvector.resize(computeInitialSize(size_));
	//	}

	//	/**
	//		Desconstructor
	//	*/
	//	~HeapWrapperConcurrent() 
	//	{ 
	//		delete[] heaparray; 
	//		delete[] nodelocks;
	//		heapvector.clear();
	//	}

	//	/**
	//		Sets the pointer heaparray and size

	//		@param[in] size_ Size of the heaparray
	//	*/
	//	void setHeap(size_t size_) {

	//		if (heaparray) {
	//			delete[] heaparray;
	//			delete[] nodelocks;
	//		}
	//		heapvector.clear();

	//		size = computeInitialSize(size_);
	//		heaparray = new HeapNodeConcurrent<ElementType>[size];
	//		nodelocks = new boost::atomic<size_t>[std::floor(size / cores)];
	//		heapvector.resize(size);

	//		count = 0;
	//	}

	//	/**
	//		Sets the array and the size
	//	*/
	//	void setHeap(void* pointer_, size_t size_)
	//	{
	//		if (heaparray) {
	//			delete[] heaparray;
	//			delete[] nodelocks;
	//		}
	//		heapvector.clear();

	//		size = size_;
	//		heaparray = (HeapNodeConcurrent<ElementType>*) pointer_;
	//		nodelocks = new boost::atomic<size_t>[std::floor(size / cores)];
	//		count = 0;
	//	}

	//	/**
	//		Sets the elements to zero
	//	*/
	//	void clear() {
	//		for (int i = 0; i < count; i++) {
	//			heaparray[i].clear();
	//			nodelocks[i].store(0, boost::memory_order_seq_cst /*boost::memory_order_relaxed*/);
	//		}

	//		heapvector.clear();

	//		count = 0;
	//	}

	//	/**
	//		Get the element of the specified index

	//		@param[in] index_ Index of the element
	//		@return Value of the element
	//	*/
	//	ElementType getElement(size_t index_)
	//	{
	//		return heaparray[heapvector[index_]].value;
	//	}

	//	/**
	//		Get the index in the array of the specified index

	//		@param[in] index_ Index of the element
	//		@return Index in the array
	//	*/
	//	size_t getIndex(size_t index_)
	//	{
	//		return heaparray[heapvector[index_]].index;
	//	}

	//private:

	//	/**
	//		Resizes the heaparray

	//		@param[in] size_ of the heaparray
	//	*/
	//	void resize(size_t size_) {
	//		HeapNodeConcurrent<ElementType>* new_heaparray = new HeapNodeConcurrent<ElementType>[size_];

	//		for (int i = 0; i<size + 1; i++) {
	//			new_heaparray[i] = heaparray[i];
	//		}

	//		delete[] heaparray;
	//		heaparray = new_heaparray;

	//		heapvector.resize(size_);

	//		size = size_;
	//	}

	//	/**
	//		Swap two heaparray elements

	//		@param[in] x first heaparray element
	//		@param[in] y second heaparray element
	//	*/
	//	void swap(size_t index_, size_t new_index)
	//	{
	//		swapElements(heaparray[new_index], heaparray[index_]);
	//		
	//		heapvector[heaparray[new_index].index] = new_index;
	//		heapvector[heaparray[index_].index] = index_;
	//	}

	//public:

	//	/**
	//		Adds a new element

	//		@param[in] value_ element which will be added
	//	*/
	//	void push(ElementType value_, size_t index_ = NULL)
	//	{
	//		if (count >= size) {
	//			std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
	//			std::exit(EXIT_FAILURE);
	//		}

	//		while (!lockCount());
	//		size_t countlockvalue = count;
	//		while (!heaparray[count].lockIndex());
	//		count++;
	//		while (!unlockCount());
	//		heaparray[countlockvalue].value = value_;
	//		heapvector[index_] = countlockvalue;
	//		
	//		pushup(countlockvalue);
	//	}

	//	/**
	//		Pops the minimal/maximal element;

	//		@return minimal/maximal value of the heap
	//	*/
	//	ElementType pop()
	//	{

	//		if (count == 0) {
	//			std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
	//			std::exit(EXIT_FAILURE);
	//		}

	//		while (!heaparray[0].lockIndex());
	//		ElementType value = heaparray[0].value;
	//		
	//		if (count == 1) {
	//			while (!lockCount());
	//			count--;
	//			heapvector[heaparray[0].index] = 0;
	//			heaparray[count].clear();
	//			while (!unlockCount());
	//		}
	//		else {

	//			while (!lockCount());
	//			count--;
	//			while (!heaparray[count].lockIndex());
	//			heaparray[0] = heaparray[count];
	//			heapvector[heaparray[0].index] = 0;
	//			heaparray[count].clear();
	//			while (!unlockCount());
	//			
	//			pulldown(0);
	//		}

	//		return value;
	//	}

	//	/**
	//		Update a value

	//		@param[in] index_ Index of the element
	//	*/
	//	void update(ElementType value_, size_t index_)
	//	{
	//		while (!heaparray[heapvector[index_]].lockIndex());
	//		heaparray[heapvector[index_]].value = value_; 		
	//		pushpull(heapvector[index_]);
	//	}

	//	/**
	//		Push up and pull down a element in the heaparray

	//		@param[in] index_ index of the element which has to push up
	//	*/
	//	void pushpull(size_t index_) {

	//		bool flag = false;

	//		while (index_ != 0) {
	//			size_t new_index;
	//			if (index_ % 2 == 0) {
	//				new_index = (index_ / 2) - 1;
	//			}
	//			else {
	//				new_index = (index_ - 1) / 2;
	//			}

	//			while (!heaparray[new_index].lockIndex());
	//			if (greater) {
	//				if (heaparray[new_index] < heaparray[index_]) {
	//					swap(new_index, index_);

	//					while (!heaparray[index_].unlockIndex());

	//					index_ = new_index;
	//					flag = true;
	//				}
	//				else {
	//					while (!heaparray[new_index].unlockIndex());

	//					break;
	//				}
	//			}
	//			else {
	//				if (heaparray[new_index] > heaparray[index_]) {
	//					swap(new_index, index_);

	//					while (!heaparray[index_].unlockIndex());

	//					index_ = new_index;
	//					flag = true;
	//				}
	//				else {
	//					while (!heaparray[new_index].unlockIndex());

	//					break;
	//				}
	//			}
	//		}

	//		if (!flag) {

	//			while (index_ < std::pow(2, std::floor(std::log2(count))) - 1) { //(size + 1) / 2 - 1

	//				while (!heaparray[2 * index_ + 1].lockIndex());
	//				while (!heaparray[2 * index_ + 2].lockIndex());

	//				size_t new_index;
	//				if (!heaparray[2 * index_ + 1].isEmpty() && !heaparray[2 * index_ + 2].isEmpty()) {
	//					if (heaparray[2 * index_ + 1] > heaparray[2 * index_ + 2]) {
	//						new_index = 2 * index_ + 1;
	//						while (!heaparray[2 * index_ + 2].unlockIndex());
	//					}
	//					else {
	//						new_index = 2 * index_ + 2;
	//						while (!heaparray[2 * index_ + 1].unlockIndex());
	//					}
	//				}
	//				else if (!heaparray[2 * index_ + 1].isEmpty() && heaparray[2 * index_ + 2].isEmpty()) {
	//					new_index = 2 * index_ + 1;
	//					while (!heaparray[2 * index_ + 2].unlockIndex());

	//				}
	//				else if (heaparray[2 * index_ + 1].isEmpty() && !heaparray[2 * index_ + 2].isEmpty()) {
	//					new_index = 2 * index_ + 2;
	//					while (!heaparray[2 * index_ + 1].unlockIndex());
	//				}
	//				else {
	//					while (!heaparray[2 * index_ + 1].unlockIndex());
	//					while (!heaparray[2 * index_ + 2].unlockIndex());

	//					break;
	//				}

	//				if (greater) {
	//					if (heaparray[index_] < heaparray[new_index]) {
	//						swap(new_index, index_);

	//						while (!heaparray[index_].unlockIndex());

	//						index_ = new_index;
	//					}
	//					else {
	//						while (!heaparray[new_index].unlockIndex());

	//						break;
	//					}
	//				}
	//				else {
	//					if (heaparray[index_] > heaparray[new_index]) {
	//						swap(new_index, index_);

	//						while (!heaparray[index_].unlockIndex());

	//						index_ = new_index;
	//					}
	//					else {
	//						while (!heaparray[new_index].unlockIndex());

	//						break;
	//					}
	//				}
	//			}
	//		}

	//		while (!heaparray[index_].unlockIndex());
	//	}

	//private:

	//	/**
	//		Vector indexing the heaparray
	//	*/
	//	std::vector<size_t> heapvector;
	//};

	/**
		Operator << Prints the values of the heap

		@param[in,out] out_ Outstream in which the node will be printed
		@param[in] heap_ Node which values shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const HeapNode<ElementType>& heap_node_)
	{
		out_ << heap_node_.value;

		return out_;
	}

	/**
		Operator << Prints the values of the heap

		@param[in,out] out_ Outstream in which the heap will be printed
		@param[in] heap_ Heap which values shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const BaseHeap<ElementType>& heap_)
	{
		for (size_t i = 0; i < heap_.count; i++) {
			out_ << heap_.heaparray[i] << " ";
		}

		return out_;
	}

	/**
		Operator << Prints the values of the heap

		@param[in,out] out_ Outstream in which the node will be printed
		@param[in] heap_super_node Node which values shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const HeapSuperNodeConcurrent<ElementType>& heap_super_node_)
	{
		HeapNodeConcurrent<ElementType>* heap_node = heap_super_node_.first_node;
		while(heap_node) {
			out_ << (*heap_node).value << " ";
			heap_node = (*heap_node).right_neighbor;
		}

		return out_;
	}

	/**
		Operator << Prints the values of the heap

		@param[in,out] out_ Outstream in which the heap will be printed
		@param[in] heap_ Heap which values shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const BaseHeapConcurrent<ElementType>& heap_)
	{
		for (size_t i = 0; i < heap_.size; i++) {
			out_ << heap_.heaparray[i] << std::endl;
		}

		return out_;
	}
}

#endif /* UTILS_HEAP_H_ */