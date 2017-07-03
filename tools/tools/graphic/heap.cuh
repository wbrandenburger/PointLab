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

#ifndef GRAPHIC_HEAP_CUH_
#define GRAPHIC_HEAP_CUH_

namespace graphic
{
	template<typename ElementType, bool greater>
	class Heap {

	public:

		ElementType* array;
		/* size in {2^n-1}*/
		size_t size;
		size_t count;

		/**
			Constructor
		*/
		__device__ Heap() 
		{
			array = nullptr;
			
			size = 0;
			count = 0;
		};

		/**
			Constructor
			
			@param size_ size of the array which has to be built
		*/
		__device__ Heap(size_t size_) 
		{
			array = new ElementType[size_];
			size = size_;
			count = 0;
		}

		/**
			Constructor

			@param array_ pointer of an array
			@param size_ size of the array 
		*/
		__device__ Heap(ElementType *array_, size_t size_)
		{
			array = array_;
			size = size_;
			count = 0;
		}

		/**
			Desconstructor
		*/
		__device__ ~Heap() {}

		/**
			Sets the pointer array and size

			@param array_ pointer of an array
			@param size_ size of the array
		*/
		__device__ void setHeap(size_t size_) 
		{
			if (size > 0) {
				clear();
			}
			
			array = new ElementType[size_];
			size = size_;
			count = 0;
		}

		/**
			Resizes the array
			
			@param size_ of the array
		*/
		__device__ void resize(size_t size_) 
		{
			ElementType* new_array = new ElementType[size_];

			for (int i = 0; i < size + 1; i++) {
				new_array[i] = array[i];
			}
			
			delete[] array;
			array = new_array;
			size = size_;
		}

		/**
			Clear the array and sets the elements to zero
		*/
		__device__ void clear() 
		{
			delete[] array;
			
			size = 0;
			count = 0;
		}

		/**
			Sets the elements to zero
		*/
		__device__ void empty() 
		{
			size_t index = lastEntry();

			for (int i = 0; i < index + 1; i++) {
				array[i] = NULL;
			}

			count = 0;
		}

		/**
			Get the number of elements in the array

			@return number of elements
		*/
		__device__ size_t getElements()
		{
			return count;
		}

		private:
		/**
			Swap two array elements

			@param x first array element
			@param y second array element
		*/
		__device__ void swap(ElementType& x, ElementType& y) 
		{
			ElementType swap = x;
			x = y;
			y = swap;
		}

		/**
			Push up a element in the array

			@param index_ index of the element which has to push up
		*/
		__device__ void pushup(size_t index_) 
		{
			while (index_ != 0) {
				size_t new_index;
				if (index_ % 2 == 0) {
					new_index = (index_ / 2) - 1;
				}
				else {
					new_index = (index_ - 1) / 2;
				}
					
				if (greater) {
					if (array[new_index] < array[index_]) {
						swap(array[new_index], array[index_]);
					}
					else { return; }
				}
				else {
					if (array[new_index] > array[index_]) {
						swap(array[new_index], array[index_]);
					}
					else { return; }
				}
				index_ = new_index;
			}
		}

		/**
			Pull down a element in the array

			@param index_ index of the element which has to pull down
		*/
		__device__ void pulldown(size_t index_) 
		{
			while (index_ < (size + 1) / 2 - 1) {
				if (greater) {
					size_t new_index;
					if (array[2 * index_ + 1] != NULL && array[2 * index_ + 2] != NULL) {
						new_index = array[2 * index_ + 1] > array[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
					}
					else if (array[2 * index_ + 1] != NULL && array[2 * index_ + 2] == NULL) {
						new_index = 2 * index_ + 1;
					}
					else if (array[2 * index_ + 1] == NULL && array[2 * index_ + 2] != NULL) {
						new_index = 2 * index_ + 2;
					}
					else {
						return;
					}
					if (array[index_] < array[new_index]) {
						swap(array[index_], array[new_index]);
						index_ = new_index;
					}
					else {
						return;
					}
				}
				else {
					size_t new_index;
					if (array[2 * index_ + 1] != NULL  && array[2 * index_ + 2] != NULL) {
						new_index = array[2 * index_ + 1] < array[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
					}
					else if (array[2 * index_ + 1] != NULL && !array[2 * index_ + 2] == NULL) {
						new_index = 2 * index_ + 1;
					}
					else if (!array[2 * index_ + 1] == NULL && array[2 * index_ + 2] != NULL) {
						new_index = 2 * index_ + 2;
					}
					else {
						return;
					}
					if (array[index_] > array[new_index]) {
						swap(array[index_], array[new_index]);
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

		@param value_ element which will be added
		*/
		__device__ void add(ElementType value_) 
		{
			if (count > size) {
				resize(size * 2 + 1);
			}
			array[count] = value_;
			pushup(count);

			count = count + 1;
		}

		/**
			Pops the minimal/maximal element;
	
			@return minimal/maximal value of the heap
		*/
		__device__ bool pop(ElementType& value_) 
		{
			if (count == 0) {
				return false;
			}
			value_ = array[0];

			if (count > 1) {
				array[0] = array[count - 1];
				pulldown(0);
			}

			array[count - 1] = NULL;
			count = count - 1;

			return true;
		}

		/**
			Checks whether the elements in the array are ordered

			@return true when the array is ordered
		*/
		__device__ bool checkHeap() 
		{
			size_t begin = 0;
			size_t depth = 0;
			while (begin < (size + 1) / 2 - 1) {
				size_t elements = (size_t)pow((float)2, (float)depth);
				for (size_t i = 0; i < elements; i++) {
					if (array[2 * begin + 1]) {
						if (greater) {
							if (array[begin] < array[2 * begin + 1]) { return 0; }
						}
						else {
							if (array[begin] > array[2 * begin + 1]) { return 0; }
						}
					}
					if (array[2 * begin + 2]) {
						if (greater) {
							if (array[begin] < array[2 * begin + 1]) { return 0; }
						}
						else {
							if (array[begin] > array[2 * begin + 1]) { return 0; }
						}
					}
					begin = begin + 1;
				}
				depth = depth + 1;
			}
			return 1;
		}
	};

	template<typename ElementType, bool greater>
	class HeapPreallocated {

	public:

		ElementType* array;
		/* size in {2^n-1}*/
		size_t size;
		size_t count;

		/**
			Constructor
		*/
		__device__ HeapPreallocated()
		{
			array = nullptr;

			size = 0;
			count = 0;
		};

		/**
			Constructor

			@param array_ pointer of an array
			@param size_ size of the array
		*/
		__device__ HeapPreallocated(ElementType *array_, size_t size_)
		{
			array = array_;
			size = size_;
			count = 0;
		}

		/**
			Desconstructor
		*/
		__device__ ~HeapPreallocated() {}

		/**
			Sets the elements to zero
		*/
		__device__ void empty()
		{

			for (int i = 0; i < size; i++) {
				array[i] = NULL;
			}

			count = 0;
		}

		/**
			Get the number of elements in the array

			@return number of elements
		*/
		__device__ size_t getElements()
		{
			return count;
		}

	private:
		/**
			Swap two array elements

			@param x first array element
			@param y second array element
		*/
		__device__ void swap(ElementType& x, ElementType& y)
		{
			ElementType swap = x;
			x = y;
			y = swap;
		}

		/**
			Push up a element in the array

			@param index_ index of the element which has to push up
		*/
		__device__ void pushup(size_t index_)
		{
			while (index_ != 0) {
				size_t new_index;
				if (index_ % 2 == 0) {
					new_index = (index_ / 2) - 1;
				}
				else {
					new_index = (index_ - 1) / 2;
				}

				if (greater) {
					if (array[new_index] < array[index_]) {
						swap(array[new_index], array[index_]);
					}
					else { return; }
				}
				else {
					if (array[new_index] > array[index_]) {
						swap(array[new_index], array[index_]);
					}
					else { return; }
				}
				index_ = new_index;
			}
		}

		/**
			Pull down a element in the array

			@param index_ index of the element which has to pull down
		*/
		__device__ void pulldown(size_t index_)
		{
			while (index_ < (size + 1) / 2 - 1) {
				if (greater) {
					size_t new_index;
					if (array[2 * index_ + 1] != NULL && array[2 * index_ + 2] != NULL) {
						new_index = array[2 * index_ + 1] > array[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
					}
					else if (array[2 * index_ + 1] != NULL && array[2 * index_ + 2] == NULL) {
						new_index = 2 * index_ + 1;
					}
					else if (array[2 * index_ + 1] == NULL && array[2 * index_ + 2] != NULL) {
						new_index = 2 * index_ + 2;
					}
					else {
						return;
					}
					if (array[index_] < array[new_index]) {
						swap(array[index_], array[new_index]);
						index_ = new_index;
					}
					else {
						return;
					}
				}
				else {
					size_t new_index;
					if (array[2 * index_ + 1] != NULL  && array[2 * index_ + 2] != NULL) {
						new_index = array[2 * index_ + 1] < array[2 * index_ + 2] ? 2 * index_ + 1 : 2 * index_ + 2;
					}
					else if (array[2 * index_ + 1] != NULL && !array[2 * index_ + 2] == NULL) {
						new_index = 2 * index_ + 1;
					}
					else if (!array[2 * index_ + 1] == NULL && array[2 * index_ + 2] != NULL) {
						new_index = 2 * index_ + 2;
					}
					else {
						return;
					}
					if (array[index_] > array[new_index]) {
						swap(array[index_], array[new_index]);
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

			@param value_ element which will be added
		*/
		__device__ void add(ElementType value_)
		{
			if (count >= size) {
				array[count-1] = value_;
				pushup(count-1);
			}
			else {
				array[count] = value_;
				pushup(count);

				count = count + 1;
			}
		}

		/**
		Pops the minimal/maximal element;

		@return minimal/maximal value of the heap
		*/
		__device__ bool pop(ElementType& value_)
		{
			if (count == 0) {
				return false;
			}
			value_ = array[0];

			if (count > 1) {
				array[0] = array[count - 1];
				pulldown(0);
			}

			array[count - 1] = NULL;
			count = count - 1;

			return true;
		}

		/**
		Checks whether the elements in the array are ordered

		@return true when the array is ordered
		*/
		__device__ bool checkHeap()
		{
			size_t begin = 0;
			size_t depth = 0;
			while (begin < (size + 1) / 2 - 1) {
				size_t elements = (size_t)pow((float)2, (float)depth);
				for (size_t i = 0; i < elements; i++) {
					if (array[2 * begin + 1]) {
						if (greater) {
							if (array[begin] < array[2 * begin + 1]) { return 0; }
						}
						else {
							if (array[begin] > array[2 * begin + 1]) { return 0; }
						}
					}
					if (array[2 * begin + 2]) {
						if (greater) {
							if (array[begin] < array[2 * begin + 1]) { return 0; }
						}
						else {
							if (array[begin] > array[2 * begin + 1]) { return 0; }
						}
					}
					begin = begin + 1;
				}
				depth = depth + 1;
			}
			return 1;
		}
	};
}

#endif /* GRAPHIC_HEAP_CUH_ */