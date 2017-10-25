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

#ifndef UTILS_MATRIX_H_
#define UTILS_MATRIX_H_

namespace utils
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
			clear();

			data = new ElementType[matrix_.rows*matrix_.cols];
			std::memcpy(data, matrix_.getPtr(), sizeof(ElementType)*matrix_.rows*matrix_.cols);

			rows = matrix_.rows;
			cols = matrix_.cols;
		}
		
		/**
			Operator =

			@param[in] matrix_ Matrix which shall be copied
		*/
		void operator=(const Matrix<ElementType>& matrix_)
		{
			clear();

			data = new ElementType[matrix_.rows*matrix_.cols];
			std::memcpy(data, matrix_.getPtr(), sizeof(ElementType)*matrix_.rows*matrix_.cols);

			rows = matrix_.rows;
			cols = matrix_.cols;
		}

		/**
			Set Matrix
			
			@param[in] data_ Row-array of a specific Type
			@param[in] rows_ Rows of the matrix
			@param[in] cols_ Columns of the matrix
		*/
		void setMatrix(ElementType* data_, size_t rows_, size_t cols_)
		{
			clear();

			data = data_;
			rows = rows_;
			cols = cols_;
		}

		/**
			Deconstructor
		*/
		~Matrix(void)
		{
			clear();
		}

		/**
			Deletes the data array
		*/
		void clear()
		{
			rows = 0;
			cols = 0;
			if (data) {
				delete[] data;
				data = nullptr;
			}
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
			Returns the number of rows

			@return Number of rows
		*/
		size_t getRows() const
		{
			return rows;
		}

		/**
			Returns the number of cols

			@return Number of cols
		*/
		size_t getCols() const
		{
			return cols;
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

	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const Matrix<ElementType>& matrix_)
	{
		size_t number = matrix_.rows < 10 ? matrix_.rows : 10;

		for (size_t i = 0; i < number; i++) {
			for (size_t j = 0; j < matrix_.cols; j++) {
				out_ << matrix_[i][j] << " ";
			}
			out_ <<  std::endl;
		}

		return out_;
	}

}

#endif /* UTILS_MATRIX_H_ */