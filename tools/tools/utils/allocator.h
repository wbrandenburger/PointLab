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

#ifndef UTILS_ALLOCATOR_H_ 
#define UTILS_ALLOCATOR_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

namespace utils
{

	/**
		Allocates memory of a specific type
	
		@param count_ Number of instances to allocate.
		@return Pointer of type T* to memory buffer
	*/	
	template <typename T> T* allocate(size_t count_ = 1)
	{
		T* mem = (T*) ::malloc(sizeof(T)*count_);
		return mem;
	}

	class PooledAllocator
	{

	public:

		/**
			Constructor
		*/
		PooledAllocator(int blocksize_ = 8192, int wordsize_ = 16)
		{
			blocksize = blocksize_;
			wordsize = wordsize_;
			remaining = 0;
			base = NULL;

			usedMemory = 0;
			wastedMemory = 0;
		}

		/**
			Destructor
		*/
		~PooledAllocator()
		{
			free();
		}
    
		/**
			Frees meory
		*/
		void free()
		{
			void* prev;
			while (base != NULL) {
				prev = *((void**) base); /* Get pointer to prev block. */
				::free(base);
				base = prev;
			}
			base = NULL;
			remaining = 0;
			usedMemory = 0;
			wastedMemory = 0;
		}

		/**
			Returns a pointer to a piece of new memory of the given size in bytes
			
			@param size_ Number of bytes which shall be allocated
			@return Returns a pointer to a piece of new memory of the given size in bytes
		*/
		void* allocateMemory(int size_)
		{
			/* Round size up to a multiple of wordsize.  The following expression
				only works for WORDSIZE that is a power of 2, by masking last bits of
				incremented size to zero.
			*/
			size_ = (size_ + (wordsize - 1)) & ~(wordsize - 1);

			/* Check whether a new block must be allocated.  Note that the first word
				of a block is reserved for a pointer to the previous block.
			*/
			if (size_ > remaining) {

				wastedMemory += remaining;

				/* Allocate new storage. */
				blocksize = (size_ + sizeof(void*) + (wordsize-1) > blocksize) ?
							size_ + sizeof(void*) + (wordsize-1) : blocksize;

				// use the standard C malloc to allocate memory
				void* m = ::malloc(blocksize);
				if (!m) {
					fprintf(stderr,"Failed to allocate memory.\n");
					return NULL;
				}

				/* Fill first word of new block with pointer to previous block. */
				((void**) m)[0] = base;
				base = m;

				int shift = 0;
				//int shift = (WORDSIZE - ( (((size_t)m) + sizeof(void*)) & (WORDSIZE-1))) & (WORDSIZE-1);

				remaining = blocksize - sizeof(void*) - shift;
				loc = ((char*)m + sizeof(void*) + shift);
			}
			void* rloc = loc;
			loc = (char*)loc + size_;
			remaining -= size_;

			usedMemory += size_;

			return rloc;
		}

		/**
			Allocates (using this pool) a generic type T.
		
			@param count_  Number of instances to allocate.
			@return Pointer of type T* to memory buffer
		*/
		template <typename T> T* allocate(size_t count_ = 1)
		{
			T* mem = (T*) this->allocateMemory((int)(sizeof(T)*count_));
			return mem;
		}
	
	    
	private:
		/**
			Number of bytes left in current block of storage. 
		*/
		int remaining;  
		/**
			Pointer to base of current block of storage. 
		*/
		void* base;     
		/**
			Current location in block to next allocate memory. 
		*/
		void* loc;      
		/**
			Number of bytes of allocated memory buffer
		*/
		int blocksize;
	
		int wordsize;

	public:
		/**
			Number of bytes which has been already used
		*/
		int     usedMemory;
		/**
			Number of bytes which are unused
		*/
		int     wastedMemory;

	};

	class Allocator {
	
	public:

		/**
			Constructor
		*/
		Allocator()
		{
			size = 0;
			chunk = 0;
			number = 0;

			base = nullptr;
			current = nullptr;
		}

		/**
			Constructor

			@param[in] size_ Number of elements
			@param[in] chunk_ Size of the elements in bytes
		*/
		Allocator(size_t size_, size_t chunk_) 
		{
			size = size_;
			chunk = chunk_;
			number = 0;

			base = (void*) new char[size*chunk];
			current = base;
		}

		 /**
			Deconstructor
		 */
		~Allocator() {}
		
		/**
			Return a pointer to a memory area that can be used

			@param[in] chunk_ size of the element which is required in Bytes
			@return Pointer to the memory within buffer
		*/
		void* allocate()
		{
			assert(number != size);

			void* pointer = current;
			current = (char*)current + chunk;

			number = number + 1;
			return pointer;
		}

		/**
			Return a pointer to a memory buffer that can be used

			@param[in] chunk_ Size of the element which is required in bytes
			@param[in,out] number_ Integer which specifies the location of the pointer
			@return Pointer to the memory buffer
		*/
		void* allocate(int& number_) 
		{

			assert(number != size);

			void* pointer = current;
			current = (char*)current + chunk;

			number_ = number;
			number = number + 1;
			return pointer;
		}

		/**
			Free memory
		*/
		void clear()
		{
			size = 0;
			chunk = 0;
			number = 0;
			
			delete[] base;
			base = nullptr;
			current = nullptr;
		}

		/**
			Shows how much memory is used

			@return Number of Bytes that are used by this object
		*/
		size_t usedMemory() 
		{
			return number*chunk;
		}

		/**
			Shows how much memory is remaining

			@return Number of Bytes that could be used
		*/
		size_t remainedMemory()
		{
			return (size - number)*chunk;
		}

		/**
			Return a pointer from a specific position within the buffer
			
			@param number_ Position within the buffer which is requested
			@return Pointer to a specific position within the buffer
		*/
		inline void* operator[](int number_)
		{
			return ((char*)base + chunk*number_);
		}

		/**
			Return the pointer to the beginning of the memory buffer
			
			@return Pointer to the beginning of the meory buffer
		*/
		void* ptr()
		{
			return base;
		}
		
		public:
	
		/** 
			Number of bytes of the allocated memory buffer
		*/
		size_t size;
		/** 
			Number of bytes of allocated chunks
		*/
		size_t chunk;
		/**
			Number of bytes which has been already used
		*/
		int number;

		/**
			Pointer to the allocated memory buffer
		*/
		void* base;
		/**
			Pointer to the current position within the memory buffer
		*/
		void* current;
	};

}

/**
	Overloaded Operator new to allocate memory of size size_ from PooledAllocator
	
	@param[in] size_ Number of bytes which shall be allocated
	@param[in,out] allocator_ Instance of PooledAllocator
	@return Pointer to the allocated memory
*/
inline void* operator new (std::size_t size_, utils::PooledAllocator& allocator_)
{
    return allocator_.allocateMemory(size_) ;
}

inline void operator delete(void* p, utils::PooledAllocator& allocator)
{
}

#endif /* UTILS_ALLOCATOR_H_ */