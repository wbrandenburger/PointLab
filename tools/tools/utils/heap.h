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

namespace utils
{
	template<typename ElementType>
	class Heap {

	public:

		/**
			Constructor
		*/
		Heap() : heaparray(nullptr), size(0), count(0) {}
		
		/**
			Constructor

			@param[in] size_ size of the heaparray which has to be built
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		Heap(size_t size_, bool greater_ = true) : size(size_), greater(greater_) { 
			heaparray = new ElementType[size_];
			count = 0;
		}
	
		/**
			Desconstructor
		*/
		~Heap() { delete[] heaparray; }

		/**
			Sets the pointer heaparray and size 

			@param[in] size_ Size of the heaparray
		*/
		void setHeap(size_t size_) {

			if (heaparray) {
				delete[] heaparray;
			}

			heaparray = new ElementType[size_];
			size = size_;
			count = 0;
		}

		/**
			Resizes the heaparray

			@param[in] size_ of the heaparray
		*/
		void resize(size_t size_) {
			ElementType* new_heaparray = new ElementType[size_];

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
			size_t index = lastEntry();
				
			for (int i = 0; i < index + 1; i++) {
				heaparray[i] = 0;
			}

			count = 0;
		}

		/**
			Get the number of elements in the heaparray

			@return Number of elements
		*/
		size_t getElements() {
			return count;
		}

	private:
	
		/**
			Swap two heaparray elements

			@param[in] x first heaparray element
			@param[in] y second heaparray element
		*/
		void swap(ElementType& x, ElementType& y) {
			ElementType swap = x;
			x = y;
			y = swap;
		}

		/**
			Push up a element in the heaparray

			@param[in] index_ index of the element which has to push up
		*/
		void pushup(size_t index_) {
			
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
							swap(heaparray[new_index], heaparray[index_]);
					}
					else { return; }
				}
				else {
					if (heaparray[new_index] > heaparray[index_]) {
						swap(heaparray[new_index], heaparray[index_]);
					}
					else { return; }
				}
				index_ = new_index;
			}
		}

		/**
			Pull down a element in the heaparray

			@param[in] index_ index of the element which has to pull down
		*/
		void pulldown(size_t index_) {
			while (index_ < (size + 1) / 2 - 1) {
				if (greater) {
					size_t new_index;
					if (heaparray[2 * index_ + 1] && heaparray[2 * index_ + 2]) {
						new_index = heaparray[2 * index_ + 1] > heaparray[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
					}
					else if (heaparray[2 * index_ + 1] && !heaparray[2 * index_ + 2]) {
						new_index = 2 * index_ + 1;
					}
					else if (!heaparray[2 * index_ + 1] && heaparray[2 * index_ + 2]) {
						new_index = 2 * index_ + 2;
						}
					else {
						return;
					}
					if (heaparray[index_] < heaparray[new_index]) {
						swap(heaparray[index_], heaparray[new_index]);
						index_ = new_index;
					}
					else {
						return;
					}
				}
				else {
					size_t new_index;
					if (heaparray[2 * index_ + 1] && heaparray[2 * index_ + 2]) {
						new_index = heaparray[2 * index_ + 1] < heaparray[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
					}
					else if (heaparray[2 * index_ + 1] && !heaparray[2 * index_ + 2]) {
						new_index = 2 * index_ + 1;
					}
					else if (!heaparray[2 * index_ + 1] && heaparray[2 * index_ + 2]) {
						new_index = 2 * index_ + 2;
					}
					else {
						return;
					}
					if (heaparray[index_] > heaparray[new_index]) {
						swap(heaparray[index_], heaparray[new_index]);
						index_ = new_index;
					}
					else {
						return;
					}
				}
			}
		}
	public:
		/**
			Adds a new element

			@param[in] value_ element which will be added
		*/
		void push(ElementType value_) {
			if (count > size) {
				resize(size * 2 + 1);
			}

			heaparray[count] = value_;
			pushup(count);

			count = count + 1;
		}

		/**
			Pops the minimal/maximal element;

			@return minimal/maximal value of the heap
		*/
		ElementType pop() {
			ElementType value = heaparray[0];

			heaparray[0] = heaparray[count-1];
			heaparray[count-1] = 0;
			count = count - 1;
			
			pulldown(0);

			return value;
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
					if (heaparray[2 * begin + 1]) {
						if (greater) {
							if (heaparray[begin] < heaparray[2 * begin + 1]) { return 0; }
						}
						else {
							if (heaparray[begin] > heaparray[2 * begin + 1]) { return 0; }
						}
					}
					if (heaparray[2 * begin + 2]) {
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


	public:

		/** 
			heaparray with the size of 2^n-1
		*/
		ElementType* heaparray;
		
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

	template<typename ElementType> struct HeapNode {

		ElementType value;

		size_t index;


		//HeapNode* heap_node;

		//HeapUpdate<HeapNode<ElementType>, greater>* heap;

		//HeapNode() : value(0), index(0), heap_node(nullptr), heap(nullptr) {}

		//HeapNode(ElementType value_, HeapUpdate<HeapNode<ElementType>, greater>* heap_) : value(value_), heap(heap_)
		//{
		//	index = heap_->count;
		//	heap_node = &heap_->heaparray[heap->count];

		//	heap_->push(*this);
		//}

		//void setHeapNode(ElementType value_, HeapUpdate<HeapNode<ElementType> greater>* heap_)
		//{
		//	value = value_;
		//	heap = heap_)
		//	index = heap_->count;
		//	heap_node = &heap_->heaparray[heap->count];

		//	heap_->push(*this);
		//}

		bool operator < (const HeapNode& node_)
		{
			return value < node_.value ? true : false;
		}
		
		bool operator > (const HeapNode& node_)
		{
			return value > node_.value ? true : false;
		}

	};

	template<typename ElementType>
	class HeapUpdate {

	public:

		/**
			Constructor
		*/
		HeapUpdate() : heaparray(nullptr), size(0), count(0) {}

		/**
			Constructor

			@param[in] size_ size of the heaparray which has to be built
			@param[in] greater Flag which specifies whether the set will be descendendly ordered
		*/
		HeapUpdate(size_t size_, bool greater_ = true) : size(size_), greater(greater_) {
			heaparray = new ElementType[size_];
			count = 0;
		}

		/**
			Desconstructor
		*/
		~HeapUpdate() { delete[] heaparray; }

		/**
		Sets the pointer heaparray and size

		@param[in] size_ Size of the heaparray
		*/
		void setHeap(size_t size_) {

			if (heaparray) {
				delete[] heaparray;
			}

			heaparray = new ElementType[size_];
			size = size_;
			count = 0;
		}

		/**
			Resizes the heaparray

			@param[in] size_ of the heaparray
		*/
		void resize(size_t size_) {
			ElementType* new_heaparray = new ElementType[size_];

			for (int i = 0; i<size + 1; i++) {
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
			size_t index = lastEntry();

			for (int i = 0; i < index + 1; i++) {
				heaparray[i] = 0;
			}

			count = 0;
		}

		/**
			Get the number of elements in the heaparray

			@return Number of elements
		*/
		size_t getElements() {
			return count;
		}

	private:

		/**
			Swap two heaparray elements

			@param[in] x first heaparray element
			@param[in] y second heaparray element
		*/
		void swap(ElementType& x, ElementType& y) 
		{
			ElementType swap = x;
			x = y;
			y = swap;
		}

		/**
			Swap two heaparray elements

			@param[in] x first heaparray element
			@param[in] y second heaparray element
		*/
		void swap(size_t index_, size_t new_index)
		{
			swap(heaparray[new_index], heaparray[index_]);
			
			heaparray[new_index].heap_node->heap_node = &heaparray[new_index];
			heaparray[new_index].index = new_index;
			
			heaparray[index_].heap_node->heap_node = &heaparray[index_];
			heaparray[index_].index = index_;
		}

		/**
			Push up a element in the heaparray

			@param[in] index_ index of the element which has to push up
		*/
		void pushup(size_t index_) {

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
					}
					else { return; }
				}
				else {
					if (heaparray[new_index] > heaparray[index_]) {
						swap(new_index, index_);
					}
					else { return; }
				}
				index_ = new_index;
			}
		}

		/**
			Pull down a element in the heaparray

			@param[in] index_ index of the element which has to pull down
		*/
		void pulldown(size_t index_) {
			while (index_ < (size + 1) / 2 - 1) {
				if (greater) {
					size_t new_index;
					if (heaparray[2 * index_ + 1] && heaparray[2 * index_ + 2]) {
						new_index = heaparray[2 * index_ + 1] > heaparray[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
					}
					else if (heaparray[2 * index_ + 1] && !heaparray[2 * index_ + 2]) {
						new_index = 2 * index_ + 1;
					}
					else if (!heaparray[2 * index_ + 1] && heaparray[2 * index_ + 2]) {
						new_index = 2 * index_ + 2;
					}
					else {
						return;
					}
					if (heaparray[index_] < heaparray[new_index]) {
						swap(index_, new_index);
						index_ = new_index;
					}
					else {
						return;
					}
				}
				else {
					size_t new_index;
					if (heaparray[2 * index_ + 1] && heaparray[2 * index_ + 2]) {
						new_index = heaparray[2 * index_ + 1] < heaparray[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
					}
					else if (heaparray[2 * index_ + 1] && !heaparray[2 * index_ + 2]) {
						new_index = 2 * index_ + 1;
					}
					else if (!heaparray[2 * index_ + 1] && heaparray[2 * index_ + 2]) {
						new_index = 2 * index_ + 2;
					}
					else {
						return;
					}
					if (heaparray[index_] > heaparray[new_index]) {
						swap(index_, new_index);
						index_ = new_index;
					}
					else {
						return;
					}
				}
			}
		}
	public:
		/**
			Adds a new element

			@param[in] value_ element which will be added
		*/
		void push(ElementType& value_) {
			if (count > size) {
				resize(size * 2 + 1);
			}

			heaparray[count] = value_;
			pushup(count);

			count = count + 1;
		}

		/**
			Pops the minimal/maximal element;

			@return minimal/maximal value of the heap
		*/
		ElementType pop() {
			ElementType value = heaparray[0];

			heaparray[0] = heaparray[count - 1];
			heaparray[count - 1] = 0;
			count = count - 1;

			pulldown(0);

			return value;
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
					if (heaparray[2 * begin + 1]) {
						if (greater) {
							if (heaparray[begin] < heaparray[2 * begin + 1]) { return 0; }
						}
						else {
							if (heaparray[begin] > heaparray[2 * begin + 1]) { return 0; }
						}
					}
					if (heaparray[2 * begin + 2]) {
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


	public:

		/**
			heaparray with the size of 2^n-1
		*/
		ElementType* heaparray;

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
	std::ostream& operator<<(std::ostream& out_, const HeapUpdate<ElementType>& heap_)
	{
		for (size_t i = 0; i < heap_.count; i++) {
			out_ << heap_.heaparray[i] << " ";
		}

		return out_;
	}

	/**
		Operator << Prints the values of the heap

		@param[in,out] out_ Outstream in which the heap will be printed
		@param[in] heap_ Heap which values shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const Heap<ElementType>& heap_)
	{
		for (size_t i = 0; i < heap_.count; i++) {
			out_ << heap_.heaparray[i] << " ";
		}

		return out_;
	}
}

#endif /* UTILS_HEAP_H_ */