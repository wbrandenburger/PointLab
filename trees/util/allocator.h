/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2008-2009  Marius Muja (mariusm@cs.ubc.ca). All rights reserved.
 * Copyright 2008-2009  David G. Lowe (lowe@cs.ubc.ca). All rights reserved.
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

#include <stdlib.h>
#include <stdio.h>

namespace trees
{

/**
 * Allocates (using C's malloc) a generic type T.
 *
 * Params:
 *     count = number of instances to allocate.
 * Returns: pointer (of type T*) to memory buffer
 */
template <typename T>
T* allocate(size_t count = 1)
{
    T* mem = (T*) ::malloc(sizeof(T)*count);
    return mem;
}



/**
 * Pooled storage allocator
 *
 * The following routines allow for the efficient allocation of storage in
 * small chunks from a specified pool.  Rather than allowing each structure
 * to be freed individually, an entire pool of storage is freed at once.
 * This method has two advantages over just using malloc() and free().  First,
 * it is far more efficient for allocating small objects, as there is
 * no overhead for remembering all the information needed to free each
 * object or consolidating fragmented memory.  Second, the decision about
 * how long to keep an object is made at the time of allocation, and there
 * is no need to track down all the objects to free them.
 *
 */

const size_t     WORDSIZE=16;
const  size_t     BLOCKSIZE=8192;

class PooledAllocator
{
    /* We maintain memory alignment to word boundaries by requiring that all
        allocations be in multiples of the machine wordsize.  */
    /* Size of machine word in bytes.  Must be power of 2. */
    /* Minimum number of bytes requested at a time from	the system.  Must be multiple of WORDSIZE. */


    int     remaining;  /* Number of bytes left in current block of storage. */
    void*   base;     /* Pointer to base of current block of storage. */
    void*   loc;      /* Current location in block to next allocate memory. */
    int     blocksize;


public:
    int     usedMemory;
    int     wastedMemory;

    /**
        Default constructor. Initializes a new pool.
     */
    PooledAllocator(int blocksize = BLOCKSIZE)
    {
        this->blocksize = blocksize;
        remaining = 0;
        base = NULL;

        usedMemory = 0;
        wastedMemory = 0;
    }

    /**
     * Destructor. Frees all the memory allocated in this pool.
     */
    ~PooledAllocator()
    {
        free();
    }
    
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
     * Returns a pointer to a piece of new memory of the given size in bytes
     * allocated from the pool.
     */
    void* allocateMemory(int size)
    {
        int blocksize;

        /* Round size up to a multiple of wordsize.  The following expression
            only works for WORDSIZE that is a power of 2, by masking last bits of
            incremented size to zero.
         */
        size = (size + (WORDSIZE - 1)) & ~(WORDSIZE - 1);

        /* Check whether a new block must be allocated.  Note that the first word
            of a block is reserved for a pointer to the previous block.
         */
        if (size > remaining) {

            wastedMemory += remaining;

            /* Allocate new storage. */
            blocksize = (size + sizeof(void*) + (WORDSIZE-1) > BLOCKSIZE) ?
                        size + sizeof(void*) + (WORDSIZE-1) : BLOCKSIZE;

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
        loc = (char*)loc + size;
        remaining -= size;

        usedMemory += size;

        return rloc;
    }

    /**
     * Allocates (using this pool) a generic type T.
     *
     * Params:
     *     count = number of instances to allocate.
     * Returns: pointer (of type T*) to memory buffer
     */
    template <typename T>
    T* allocate(size_t count = 1)
    {
        T* mem = (T*) this->allocateMemory((int)(sizeof(T)*count));
        return mem;
    }

};

	class Allocator {
	
	public:
	
		size_t size;
		size_t chunk;
		int number;


		void* base;
		void* current;

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

			@param size_ number of elements
			@param chunk_ size of the elements in Bytes
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

			@param chunk_ size of the element which is required in Bytes
			@return pointer to the memory area
		*/
		void* allocate()
		{
			if (number == size) {
				std::cout << __FILE__ << " Line " << __LINE__ << " " << number << std::endl;
				exit(EXIT_FAILURE);
			}

			void* pointer = current;
			current = (char*)current + chunk;

			number = number + 1;
			return pointer;
		}

		/**
			Return a pointer to a memory area that can be used

			@param chunk_ size of the element which is required in Bytes
			@param number_ integer which specifies the location of the pointer
			@return pointer to the memory area
			@return number_ integer which specifies the location of the pointer
		*/
		void* allocate(int& number_) 
		{

			if (number == size) {
				std::cout << __FILE__ << " Line " << __LINE__ << " " << number << std::endl;
				exit(EXIT_FAILURE);
			}

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

			@return number of Bytes that are used by this object
		*/
		size_t usedMemory() 
		{
			return number*chunk;
		}

		/**
			Shows how much memory is remaining

			@return number of Bytes that could be used
		*/
		size_t remainedMemory()
		{
			return (size - number)*chunk;
		}

		inline void* operator[](int number_)
		{
			return ((char*)base + chunk*number_);
		}

		void* ptr()
		{
			return base;
		}
	};

}

inline void* operator new (std::size_t size, trees::PooledAllocator& allocator)
{
    return allocator.allocateMemory(size) ;
}

inline void operator delete(void* p, trees::PooledAllocator& allocator)
{
}


#endif /* TREES_ALLOCATOR_H_ */
