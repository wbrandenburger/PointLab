/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017	Wolfgang Brandenburger
*					(w.brandenburger@unibw.de).
*					All rights reserved.
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

#ifndef GRAPHIC_QUEUE_CUH_
#define GRAPHIC_QUEUE_CUH_

namespace graphic
{
	template<typename ElementType>
	class Queue {

	public:

		ElementType* array;

		size_t size;
		size_t popHead;
		size_t pushHead;

		/**
			Constructor
		*/
		__device__
			Queue()
		{
			array = nullptr;

			size = 0;
			popHead = 0;
			pushHead = 0;
		};

		/**
			Constructor

			@param size_ size of the array which has to be built
		*/
		__device__
			Queue(size_t size_)
		{
			array = new ElementType[size_];
			size = size_;
			popHead = 0;
			pushHead = 0;
		}

		/**
			Constructor

			@param array_ pointer of an array
			@param size_ size of the array
		*/
		__device__
			Queue(ElementType *array_, size_t size_)
		{
			array = array_;
			size = size_;
			popHead = 0;
			pushHead = 0;
		}

		/**
			Desconstructor
		*/
		__device__
			~Queue() {}

		/**
			Sets the pointer array and size

			@param array_ pointer of an array
			@param size_ size of the array
		*/
		__device__ void setQueue(size_t size_)
		{
			if (size > 0) {
				clear();
			}

			array = new ElementType[size_];
			size = size_;
			popHead = 0;
			pushHead = 0;
		}

		__device__
			ElementType pop()
		{
			if (popHead == size) {
				popHead = 0;
			}

			return array[popHead++];
		}

		__device__
			void push(ElementType element_)
		{
			if (pushHead == size) {
				pushHead = 0;
			}

			array[pushHead++] = element_;
		}
	};

	template<typename ElementType>
	class QueueSorted
	{

	public:

		/**
		Constructor
		*/
		__device__
			QueueSorted()
		{
			array = nullptr;

			popHead = 0;
			pushHead = 0;
			count = 0;
			size = 0;
		}

		/**
		Constructor

		@param size_ size of the array which has to be built
		*/
		__device__
			QueueSorted(size_t size_)
		{
			array = new ElementType[size_];

			popHead = 0;
			pushHead = 0;
			count = 0;
			size = size_;
		}

		/**
		Constructor

		@param array_ pointer of an array
		@param size_ size of the array
		*/
		__device__
			QueueSorted(ElementType *array_, size_t size_)
		{
			array = array_;

			popHead = 0;
			pushHead = 0;
			count = 0;
			size = size_;
		}

		/**
		Deconstructor
		*/
		__device__
			~QueueSorted() {}

		/**
		Removes an element
		*/
		__device__
			bool pop(ElementType& element_)
		{
			if (count == 0) {
				return false;
			}

			element_ = array[popHead];
			array[popHead] = NULL;
			popHead++;
			count--;
			popHead = popHead % size;

			return  true;
		}

		/**
		Adds an element
		*/
		__device__
			void push(ElementType element_)
		{
			size_t sortHeadFirst;
			size_t sortHeadSecond;
			if (count < size) {
				array[pushHead] = element_;
				
				sortHeadSecond = pushHead;
				if (pushHead == 0) {
					sortHeadFirst = size - 1;

				}
				else {
					sortHeadFirst = pushHead - 1;
				}
				
				pushHead++;
				count++;

				pushHead = pushHead % size;
			}
			else {
				int tempPushHead = pushHead;
				if (tempPushHead == 0) {
					tempPushHead = size - 1;
				}
				else {
					tempPushHead--;
				}

				if (array[tempPushHead] > element_) {
					array[tempPushHead] = element_;
					
					sortHeadSecond = tempPushHead;
					if (tempPushHead == 0) {
						sortHeadFirst = size - 1;
					}
					else {
						sortHeadFirst = tempPushHead - 1;
						
					}
				}
			}

			int sort = count;
			while (array[sortHeadFirst] > array[sortHeadSecond] && sort-->1) {
				ElementType element = array[sortHeadFirst];
				array[sortHeadFirst] = array[sortHeadSecond];
				array[sortHeadSecond] = element;

				sortHeadSecond = sortHeadFirst;
				if (sortHeadFirst == 0) {
					sortHeadFirst = size - 1;
				}
				else {
					sortHeadFirst--;
				}
			}
		}

			ElementType* array;
		private:
			size_t popHead;
			size_t pushHead;
			size_t count;
			size_t size;
	};
}

#endif /* GRAPHIC_QUEUE_CUH_ */