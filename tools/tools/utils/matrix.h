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
		Matrix() :
			rows_(0), cols_(0), data_(NULL)
		{
		}

		/**
			Constructor
			
			@param[in] data_ Row-array of a specific Type
			@param[in] rows_ Rows of the matrix
			@param[in] cols_ Columns of the matrix
		*/
		Matrix(ElementType* data, size_t rows, size_t cols) : Matrix()
		{
			rows_ = rows;
			cols_ = cols;
			data_ = data;
		}

		/**
			Deconstructor
		*/
		~Matrix()
		{
			clearMemory();
		}

		/**
			Deletes the data array
		*/
		void clearMemory ()
		{
			if (data_) {
				delete[] data_;
				data_ = nullptr;
			}
		}

		/**
			Copy constructor

			@param[in] matrix An instance of class Matrix
		*/
		Matrix(const Matrix<ElementType>& matrix) : Matrix()
		{

			data_ = new ElementType[matrix_.getRows()*matrix_.getCols()];
			std::memcpy(data_, matrix.getPtr(), sizeof(ElementType)*matrix.getRows()*matrix.getCols());

			rows_ = matrix.getRows();
			cols_ = matrix.getCols();
		}
		
		/**
			Copy constructor

			@param[in] matrix An instance of class Matrix
		*/
		Matrix(const Matrix<ElementType>&& matrix) : Matrix()
		{
			*this = matrix;
		}	

		/**
			Operator =
	
			@param[in] matrix An instance of class Matrix
			@return Returns reference to the current instance
		*/
		Matrix<ElementType>& operator=(const Matrix<ElementType>& matrix)
		{
			clearMemory();

			data_ = new ElementType[matrix.getRows()*matrix.getCols()];
			std::memcpy(data_, matrix.getPtr(), sizeof(ElementType)*matrix.getRows()*matrix.getCols());

			rows_ = matrix.getRows();
			cols_ = matrix.getCols();

			return *this;
		}

		/**
			Operator =
	
			@param[in] matrix An instance of class Matrix
			@return Returns reference to the current instance
		*/
		Matrix<ElementType>& operator=(const Matrix<ElementType>&& matrix)
		{
			clearMemory();

			*this = matrix;

			return *this;
		}

		/**
			Set Matrix
			
			@param[in] data_ Row-array of a specific Type
			@param[in] rows_ Rows of the matrix
			@param[in] cols_ Columns of the matrix
		*/
		void setMatrix(ElementType* data, size_t rows, size_t cols)
		{
			clearMemory();

			data_ = data;
			rows_ = rows;
			cols_ = cols;
		}

		/**
			Returns the pointer of the data array
			
			@return Return pointer of data array
		*/
		ElementType* getPtr() const
		{
			return data_;
		}

		/**
			Returns the number of rows

			@return Number of rows
		*/
		size_t getRows() const
		{
			return rows_;
		}

		/**
			Returns the number of cols

			@return Number of cols
		*/
		size_t getCols() const
		{
			return cols_;
		}

		/**
			Return the pointer of the indexth row

			@param[in] index_ Index of the row
			@return Returns the pointer of the indexth row
		*/
		inline ElementType* operator[](size_t index) const
		{
			return data_ + index*cols_;
		}
		
	public:

		/** 
			Rows of matrix 
		*/
		size_t rows_; 

		/** 
			Columns of matrix 
		*/
		size_t cols_;  

		/** 
			Pointer to data 
		*/ 
		ElementType* data_; 
	};

	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out, const Matrix<ElementType>& matrix)
	{
		size_t number = matrix.getRows() < 10 ? matrix.getRows() : 10;

		for (size_t i = 0; i < number; i++) {
			for (size_t j = 0; j < matrix.getCols(); j++) {
				out << matrix_[i][j] << " ";
			}
			out <<  std::endl;
		}

		return out;
	}

}

#endif /* UTILS_MATRIX_H_ */