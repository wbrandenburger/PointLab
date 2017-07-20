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
#include <assert.h>

namespace utils
{

	template<typename ElementType> struct HeapNode {

		ElementType value;

		size_t index;

		/**
			Constructor
		*/
		HeapNode() : value(NULL), index(NULL) {}

		/**
			Constructor

			@param[in] value_ Element
			@param[in] index_ Index in array
		*/
		HeapNode(ElementType value_, size_t index_) : value(value_), index(index_) {}

		/**
			Operator < Compares two HeapNodes

			@param[in] heap_node_ Instance of HeapNode
		*/
		bool operator < (const HeapNode& heap_node_)
		{
			return value < heap_node_.value ? true : false;
		}

		/**
			Operator < Compares two HeapNodes

			@param[in] heap_node_ Instance of HeapNode
		*/
		bool operator > (const HeapNode& heap_node_)
		{
			return value > heap_node_.value ? true : false;
		}

		/**
			Clears the structure
		*/
		void clear()
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
		BaseHeap(size_t size_, bool greater_ = true) : size(size_), greater(greater_), count(0) {
			heaparray = new HeapNode<ElementType>[size_];
		}

		virtual ~BaseHeap() = 0 {}
		
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
		virtual void setHeap(size_t size_) = 0 {}

		/**
			Resizes the heaparray

			@param[in] size_ of the heaparray
		*/
		virtual void resize(size_t size_) = 0 {}

		/**
			Sets the elements to zero
		*/
		virtual void clear() = 0 {}

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
							if (heaparray[begin] < heaparray[2 * begin + 1]) { return 0; }
						}
						else {
							if (heaparray[begin] > heaparray[2 * begin + 1]) { return 0; }
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
			Swap two heaparray elements

			@param[in] x first heaparray element
			@param[in] y second heaparray element
		*/
		virtual void swap(size_t index_, size_t new_index) = 0 {}

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
		bool pushup(size_t index_) {

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
		bool pulldown(size_t index_) {

			bool flag = false;

			while (index_ < (size + 1) / 2 - 1) {
				if (greater) {
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
					size_t new_index;
					if (!heaparray[2 * index_ + 1].isEmpty() && !heaparray[2 * index_ + 2].isEmpty()) {
						new_index = heaparray[2 * index_ + 1] < heaparray[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
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
		virtual void push(ElementType value_, size_t index_ = NULL) = 0 {}

		/**
			Pops the minimal/maximal element;

			@return minimal/maximal value of the heap
		*/
		virtual ElementType pop()  = 0 {}

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
		Heap(size_t size_, bool greater_ = true) : BaseHeap(size_) {
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

			heaparray = new HeapNode<ElementType>[size_];
			size = size_;
			count = 0;
		}

		/**
			Resizes the heaparray

			@param[in] size_ of the heaparray
		*/
		void resize(size_t size_) {
			HeapNode<ElementType>* new_heaparray = new HeapNode<ElementType>[size_];

			for (int i = 0; i<size+1; i++) {
				new_heaparray[i] = heaparray[i];
			}

			delete[] heaparray;
			heaparray = new_heaparray;
			size = size_;
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
			ElementType value = heaparray[0].value;

			heaparray[0] = heaparray[count-1];
			heaparray[count-1].clear();
			count = count - 1;
			
			pulldown(0);

			return value;
		}

	public:

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
			heapvector.resize(size_);
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

			heaparray = new HeapNode<ElementType>[size_];
			heapvector.resize(size_);

			size = size_;
			count = 0;
		}

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
			assert(heapvector[index_].heap_node != nullptr);

			return heapvector[index_].heap_node->value;
		}

		/**
			Get the index in the array of the specified index

			@param[in] index_ Index of the element
			@return Index in the array
		*/
		size_t getIndex(size_t index_)
		{
			assert(heapvector[index_].heap_node != nullptr);

			return heapvector[index_].heap_node->index;
		}

	private:

		/**
			Swap two heaparray elements

			@param[in] x first heaparray element
			@param[in] y second heaparray element
		*/
		void swap(size_t index_, size_t new_index)
		{
			swapElements(heaparray[new_index], heaparray[index_]);
			
			heapvector[heaparray[new_index].index].index = new_index;
			heapvector[heaparray[index_].index].index = index_;
		}

	public:

		/**
			Adds a new element

			@param[in] value_ element which will be added
			@param[in] index_ Index ind the list of elements
		*/
		void push(ElementType value_, size_t index_) {
			if (count > size) {
				resize(size * 2 + 1);
			}

			HeapNode<ElementType> heap_node(value_, index_);
			heaparray[count] = heap_node;
			
			heapvector[index_].index = count;

			pushup(count);

			count = count + 1;
		}

		/**
			Pops the minimal/maximal element;

			@return minimal/maximal value of the heap
		*/
		ElementType pop() {
			ElementType value = heaparray[0].value;
			heapvector[heaparray[0].index].clear();

			heaparray[0] = heaparray[count - 1];
			heaparray[count - 1].clear();
			count = count - 1;

			pulldown(0);

			return value;
		}

		/**
			Update a value

			@param[in] index_ Index of the element
		*/
		void update(ElementType value_, size_t index_)
		{
			heaparray[heapvector[index_].index].value = value_;

			if (!pushup(heapvector[index_].index))
			{
				pulldown(heapvector[index_].index);
			}
			
		}

	public:

		/**
			Vector indexing the heaparray
		*/
		std::vector<HeapNode<ElementType>> heapvector;
	};

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
}

#endif /* UTILS_HEAP_H_ */