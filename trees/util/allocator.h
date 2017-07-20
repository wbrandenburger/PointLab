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

#ifndef TREES_ALLOCATOR_H_
#define TREES_ALLOCATOR_H_

namespace trees
{

	class Allocator
	{

	public:

		/**
			Constructor

			@param[in] Number of bytes of allocated memory buffer
			@param[in] Number of bytes of one element
		*/
		Allocator(size_t blocksize_ = 8192, size_t wordsize_ = 16) : blocksize(blocksize_)
		{
			base = nullptr;
			loc = nullptr;

			blocks = 0;
			remaining = 0;
			usedMemory = 0;
		}

		/**
			Deconstructor
		*/
		virtual ~Allocator() {}

		/**
			Frees memory
		*/
		virtual void clear() = 0;

		/**
			Returns a pointer to a piece of new memory of the given size in bytes

			@param size_ Number of bytes which shall be allocated
			@return Returns a pointer to a piece of new memory of the given size in bytes
		*/
		virtual void* allocate(int size_) = 0;

	protected:

		/**
			Number of bytes of allocated memory buffer
		*/
		const size_t blocksize;
		/**
			Pointer to the allocated memory buffer
		*/
		void* base;
		/**
			Current location in block to next allocate memory.
		*/
		void* loc;
		/**
			Number of blocks
		*/
		size_t blocks;
		/**
			Number of bytes left in current block of storage.
		*/
		size_t remaining;
		/**
			Number of bytes which has been already used
		*/
		size_t usedMemory;
	};

	class PooledAllocator : public Allocator
	{

	public:

		/**
			Constructor

			@param[in] Number of bytes of allocated memory buffer
			@param[in] Number of bytes of one element
		*/
		PooledAllocator(size_t blocksize_ = 8192) : Allocator(blocksize_) {}

		/**
			Destructor
		*/
		~PooledAllocator() {}

		/**
			Frees memory
		*/
		void clear()
		{
			void* prev;
			void* current_base = base;
			while (current_base != nullptr) {
				prev = *((void**)current_base); /* Get pointer to prev block. */
				delete[] current_base;
				current_base = prev;
			}
			base = nullptr;
			loc = nullptr;
			remaining = 0;
			usedMemory = 0;
		}

		/**
			Returns a pointer to a piece of new memory of the given size in bytes

			@param size_ Number of bytes which shall be allocated
			@return Returns a pointer to a piece of new memory of the given size in bytes
		*/
		void* allocate(int size_)
		{
			/**
				Check whether a new block must be allocated.  Note that the first word
				of a block is reserved for a pointer to the previous block.
			*/
			if (size_ > remaining) {
				void* pointer = (void*) new char[blocksize + sizeof(void*)];
				if (!pointer) {
					std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
					std::exit(EXIT_FAILURE);
				}
				((void**)pointer)[0] = base;
				base = pointer;
				loc = ((char*)pointer + sizeof(void*));

				blocks++;
				remaining = blocksize - sizeof(void*);
			}

			/**
				Return current location and sets remaining and used memory
			*/
			void* return_loc = loc;
			loc = (char*)loc + size_;

			remaining -= size_;
			usedMemory += size_;

			return return_loc;
		}

		/**
			Joins the different blocks and return the poiner of this memory buffer
		*/
		void* join() 
		{
			void* pointer = (void*) new char[blocks * blocksize];
			
			void* current_base = base;
			for (size_t i = blocks; i > 0; i--) {
				std::copy((char*)current_base + sizeof(void*), (char*)current_base + sizeof(void*) + blocksize, (char*)pointer + (i - 1)*blocksize);
				current_base = *((void**)current_base);
			}

			return pointer;
		}

	};
}

/**
	Overloaded Operator new to allocate memory of size size_ from PooledAllocator

	@param[in] size_ Number of bytes which shall be allocated
	@param[in,out] allocator_ Instance of PooledAllocator
	@return Pointer to the allocated memory
*/
inline void* operator new (std::size_t size_, trees::Allocator& allocator_)
{
	return allocator_.allocate(size_);
}

#endif /* TREES_ALLOCATOR_H_ */