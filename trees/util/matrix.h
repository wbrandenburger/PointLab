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

#ifndef TREES_MATRIX_H_
#define TREES_MATRIX_H_

namespace trees
{

	template <typename ElementType>
	class Matrix
	{
	public:
	
		/**
			Constructor
		*/
		Matrix(void) :
			rows(0), cols(0), data(NULL)
		{
		}

		/**
			Constructor
			
			@param[in] data_ Row-array of a specific Type
			@param[in] rows_ Rows of the matrix
			@param[in] cols_ Columns of the matrix
		*/
		Matrix(ElementType* data_, size_t rows_, size_t cols_) :
			rows(rows_), cols(cols_), data(data_)
		{
		}

		/**
			Copy Constructor

			@param[in] matrix_ Matrix which shall be copied
		*/
		Matrix(const Matrix<ElementType>& matrix_)
		{
			data = new ElementType[matrix_.rows*matrix_.cols];
			for (size_t i = 0; i < matrix_.rows*matrix_.cols; i++){
				data[i] = matrix_[std::ceil(i/3)][i%3];
			}
			rows = matrix_.rows;
			cols = matrix_.cols;
		}
		
		/**
			Copy assignment operator

			@param[in] matrix_ Matrix which shall be copied
		*/
		void operator=(const Matrix<ElementType>& matrix_)
		{
			data = new ElementType[matrix_.rows*matrix_.cols];
			for (size_t i = 0; i < matrix_.rows*matrix_.cols; i++) {
				data[i] = matrix_[std::ceil(i / 3)][i % 3];
			}
			rows = matrix_.rows;
			cols = matrix_.cols;
		}

		/**
			Deconstructor
		*/
		~Matrix()
		{
		}

		/**
			Deletes the data array
		*/
		void clear() const
		{
			delete[] data;
		}

		/**
			Returns the pointer of the data array
			
			@return Return pointer of data array
		*/
		ElementType* getPtr() const
		{
			return (data);
		}

		/**
			Return the pointer of the indexth row

			@param[in] index_ Index of the row
			@return Returns the pointer of the indexth row
		*/
		inline ElementType* operator[](size_t index_) const
		{
			return data + index_*cols;
		}
		
	public:
		/** 
			Rows of matrix 
		*/
		size_t rows; 
		/** 
			Columns of matrix 
		*/
		size_t cols;  
		/** 
			Pointer to data 
		*/ 
		ElementType* data; 
	};

}

#endif /* TREES_MATRIX_H_ */