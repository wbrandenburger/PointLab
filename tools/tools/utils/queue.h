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

#ifndef UTILS_QUEUE_H_
#define UTILS_QUEUE_H_

#include <vector>

namespace utils
{
	template<typename ElementType>
	class Queue
	{
	public:
		/**
			Constructor
		*/
		Queue() : queuearray(nullptr), size(0), count(0), greater(true) {}

		/**
			Constructor
		*/
		Queue(size_t size_, bool greater_ = true) : size(size_), count(0), greater(greater_)
		{
			queuearray = new ElementType[size];
			loc = queuearray;
		}

		/**
			Constructor
		*/
		Queue(ElementType* pointer_, size_t size_, bool greater_ = true)
			: queuearray(pointer_), loc(pointer_), size(size_), count(0), greater(greater_) {}

		/**
			Deconstructor
		*/
		~Queue() {}

		/**
			Set the array amd the size
		*/
		void setQueue(size_t size_) 
		{
			if (queuearray) {
				delete[] queuearray;
			}

			size = size_;
			queuearray = new ElementType[size];
			loc = queuearray;

			count = 0;
		}

		/**
			Set the array amd the size
		*/
		void setQueue(ElementType* pointer, size_t size_) 
		{
			if (queuearray) {
				delete[] queuearray;
			}

			size = size_;
			queuearray = pointer_;
			loc = queuearray;

			count = 0;
		}

		/**
			Resizes the queuearray

			@param[in] size_ Size of the queuearray
		*/
		void resize(size_t size_) {

			queuearray_new = new ElementType[size_];
			std::memcpy(queuearray_new, queuarray, size);
			if (queuearray) {
				delete[] queuearray;
			}
			queuarray = queuearray_new;

			size = size_;
			loc = queuearray;
			for (size_t i = 0; i < count; i++) {
				loc++;
			}

		}

	private:

		ElementType* queuearray;
		ElementType* loc;
		
		size_t size;
		size_t count;

		bool greater;
	};

	/////**
	////	Operator << Prints the values of the heap

	////	@param[in,out] out_ Outstream in which the node will be printed
	////	@param[in] heap_ Node which values shall be printed
	////*/
	////template<typename ElementType>
	////std::ostream& operator<<(std::ostream& out_, const HeapNode<ElementType>& heap_node_)
	////{
	////	out_ << heap_node_.value;

	////	return out_;
	////}

	/////**
	////	Operator << Prints the values of the heap

	////	@param[in,out] out_ Outstream in which the heap will be printed
	////	@param[in] heap_ Heap which values shall be printed
	////*/
	////template<typename ElementType>
	////std::ostream& operator<<(std::ostream& out_, const BaseHeap<ElementType>& heap_)
	////{
	////	for (size_t i = 0; i < heap_.count; i++) {
	////		out_ << heap_.heaparray[i] << " ";
	////	}

	////	return out_;
	////}

	/////**
	////	Operator << Prints the values of the heap

	////	@param[in,out] out_ Outstream in which the node will be printed
	////	@param[in] heap_ Node which values shall be printed
	////*/
	////template<typename ElementType>
	////std::ostream& operator<<(std::ostream& out_, const HeapNodeConcurrent<ElementType>& heap_node_)
	////{
	////	out_ << heap_node_.value;

	////	return out_;
	////}

	/////**
	////	Operator << Prints the values of the heap

	////	@param[in,out] out_ Outstream in which the heap will be printed
	////	@param[in] heap_ Heap which values shall be printed
	////*/
	////template<typename ElementType>
	////std::ostream& operator<<(std::ostream& out_, const BaseHeapConcurrent<ElementType>& heap_)
	////{
	////	for (size_t i = 0; i < heap_.count; i++) {
	////		out_ << heap_.heaparray[i] << " ";
	////	}

	////	return out_;
	////}
}

#endif /* UTILS_QUEUE_H_ */