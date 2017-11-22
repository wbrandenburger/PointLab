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
			rows_(0), cols_(0), data_(nullptr)
		{
		}
		
		/**
			Constructor
			
			@param[in] rows_ Rows of the matrix
			@param[in] cols_ Columns of the matrix
		*/
		Matrix(size_t rows, size_t cols) : Matrix()
		{
			rows_ = rows;
			cols_ = cols;

			data_ = new ElementType[rows_ * cols_];
			std::memset(data_, (ElementType)0, sizeof(ElementType) * rows_ * cols_);
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
			data_ = new ElementType[matrix.getRows()*matrix.getCols()];

			Matrix<ElementType>::Iterator it_cpy = begin();
			ElementType* it_end = matrix.end();
			for (Matrix<ElementType>::Iterator it = matrix.begin(); it != it_end; it++) {
				*it_cpy = *it;
				it_cpy++;
			}

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
			
			Matrix<ElementType>::Iterator it_cpy = begin();
			ElementType* it_end = matrix.end();
			for (Matrix<ElementType>::Iterator it = matrix.begin(); it != it_end; it++) {
				*it_cpy = *it;
				it_cpy++;
			}

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
			Reset the data-array
		*/
		void reset()
		{
			std::memset(data_, (ElementType)0, sizeof(ElementType) * rows_ * cols_);
		}

		/**
			Set Matrix
			
			@param[in] rows_ Rows of the matrix
			@param[in] cols_ Columns of the matrix
		*/
		void setMatrix(size_t rows, size_t cols)
		{
			clearMemory();

			rows_ = rows;
			cols_ = cols;

			data_ = new ElementType[rows_ * cols_];
			std::memset(data_, (ElementType)0, sizeof(ElementType) * rows_ * cols_);
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

			rows_ = rows;
			cols_ = cols;

			data_ = data;
		}

		/**
			Returns the pointer to the data array
			
			@return Return pointer of data array
		*/
		ElementType* getPtr() const
		{
			return data_;
		}

		/**
			Returns the pointer to the data array

			@return Return pointer of data array
		*/
		ElementType* getAllocatedPtr() const
		{
			ElementType* data_new = new ElementType[rows_ * cols_];
			std::memcpy(data_new, data_, sizeof(ElementType) * rows_ * cols_);

			return data_new;
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
			return data_ + index * cols_;
		}

		/**
			Returns a pointer to the first entry of data_

			@return Pointer to the first entry of data_
		*/
		ElementType* begin() const
		{
			return data_;
		}
	
		/**
			Returns a pointer to the last entry + 1 of the points

			@return Pointer to the last entry + 1 of the points
		*/
		ElementType* end() const
		{
			return data_ + rows_ * cols_;
		}

		/**
			Structure of a iterator for points, colors, normals and triangles
		*/
		class Iterator
		{
		public:
			/**
				Constructor
			*/
			Iterator() : iterator_(nullptr)
			{
			}
		
			/**
				Constructor

				@param[in] begin Initial pointer
			*/
			Iterator(ElementType* begin)
			{
				iterator_ = begin;
			}

			/**
				Destructor
				*/
			~Iterator()
			{
			}
			
			/**
				Copy Constructor

				@param[in] An instance of class Iterator
			*/
			Iterator(const Iterator& iterator) = delete;

			/**
				Operator = 

				@param[in] An instance of class Iterator
			*/
			Iterator(const Iterator&& iterator) = delete;

			/**
				Operator =
				
				@param[in] begin Initial pointer
			*/
			Iterator& operator=(const ElementType* begin)
			{
				iterator_ = begin;
			
				return *this;
			}

			/**
				Operator = 

				@param[in] An instance of class Iterator
				@return Returns reference to the current instance
			*/
			Iterator& operator=(const Iterator& iterator) = delete;

			/**
				Operator = 
			
				@param[in] iterator An instance of class Iterator
				@return Returns reference to the current instance
			*/
			Iterator& operator=(const Iterator&& iterator) = delete;
	
			/**
				Operator ++

				@param[in] Increment
				@return Returns reference to the current instance
			*/
			Iterator& operator++(int)
			{
				iterator_++; 
			
				return *this;
			}

			/**
				Operator = 

				@param[in] iterator Pointer to an element
				@return Returns reference to the current instance
			*/
			Iterator& operator=(ElementType* iterator)
			{
				iterator_ = iterator;
			
				return *this;
			}

			/**
				Operator != 
			
				@param[in] iterator Pointer to an element
				@return Returns reference to the current instance
			*/
			bool operator!=(ElementType* iterator)
			{
				return iterator_ != iterator;
			}

			/**
				Operator == 

				@param[in] iterator Pointer to an element
				@return Returns reference to the current instance
			*/
			bool operator==(ElementType* iterator)
			{
				return iterator_ == iterator;
			}

			/**
				Operator *

				@return Content of current location of iterator
			*/
			ElementType& operator*()
			{
				return *iterator_;
			}

		private:		
			/**
				Pointer to the current element
			*/
			ElementType* iterator_;
		};

		/**
			Operator +

			@param[in] a Scalar
		*/
		Matrix<ElementType> operator+(ElementType a)
		{
			Matrix<ElementType> matrix_new(rows_, cols_);

			Matrix<ElementType>::Iterator it = begin();
			Matrix<ElementType>::Iterator it_matrix_new = matrix_new.begin();

			while (it != end()) {
				*it_matrix_new = *it + a;
				it_matrix_new++;
				it++;
			}

			return matrix_new;
		}

		/**
			Operator +=

			@param[in] a Scalar
		*/
		Matrix<ElementType>& operator+=(ElementType a)
		{
			Matrix<ElementType>::Iterator it = begin();

			while (it != end()) {
				*it += a;
				it++;
			}

			return (*this);
		}

		/**
			Operator -

			@param[in] a Scalar
		*/
		Matrix<ElementType> operator-(ElementType a)
		{
			return (*this) + (-1) * a;
		}	
				
		/**
			Operator +=

			@param[in] a Scalar
		*/
		Matrix<ElementType>& operator-=(ElementType a)
		{
			(*this) += (-1) * a;

			return (*this);
		}

		/**
			Operator +

			@param[in] matrix An instance of class Matrix
		*/
		Matrix<ElementType> operator+(Matrix<ElementType> matrix)
		{
			Matrix<ElementType> matrix_new(rows_, cols_);
			
			Matrix<ElementType>::Iterator it = begin();
			Matrix<ElementType>::Iterator it_matrix_new = matrix_new.begin();
			if (rows_ == matrix.getRows() && cols_ == matrix.getCols()) {
				Matrix<ElementType>::Iterator it_matrix = matrix.begin();

				while (it != end()) {
					*it_matrix_new = *it + *it_matrix;
					it_matrix_new++;
					it_matrix++;
					it++;
				}
			}
			else if (matrix.getRows() == 1 && cols_ == matrix.getCols()) {
				for (size_t i = 0; i < rows_*cols_; i++) {
					*it_matrix_new = *it + matrix[0][i % matrix.getCols()];
					it_matrix_new++;
					it++;
				}
			}
			else {
				exitFailure(__FILE__, __LINE__);
			}

			return matrix_new;
		}

		/**
			Operator +=

			@param[in] matrix An instance of class Matrix
		*/
		Matrix<ElementType>& operator+=(Matrix<ElementType> matrix)
		{
			Matrix<ElementType>::Iterator it = begin();
			if (rows_ == matrix.getRows() && cols_ == matrix_.getCols()) {
				Matrix<ElementType>::Iterator it_matrix = matrix.begin();

				while (it != end()) {
					*it += *it_matrix;
					it_matrix++;
					it++;
				}
			}
			else if (matrix.getRows() == 1 && cols_ == matrix_.getCols()) {
				for (size_t i = 0; i < rows_*cols_; i++) {
					*it += matrix[0][i % matrix.getCols()];
					it++;
				}
			}
			else {
				exitFailure(__FILE__, __LINE__);
			}

			return (*this);
		}

		/**
			Operator -

			@param[in] matrix An instance of class Matrix
		*/
		Matrix<ElementType> operator-(Matrix<ElementType> matrix)
		{
			return (*this) + matrix*(-1);
		}

		/**
			Operator -

			@param[in] matrix An instance of class Matrix
		*/
		Matrix<ElementType>& operator-=(Matrix<ElementType> matrix)
		{
			*this += matrix * (-1);
			
			return (*this);
		}

		/**
			Operator *

			@param[in] matrix An instance of class Matrix
		*/
		Matrix<ElementType> operator*(Matrix<ElementType> matrix)
		{
			if (cols_ != matrix.getRows()) {
				exitFailure(__FILE__, __LINE__);
			}

			Matrix<ElementType> matrix_new(rows_, matrix.getCols());
			size_t index_left = 0, index_right = 0;
			for (Matrix<ElementType>::Iterator it = matrix_new.begin(); it != matrix_new.end(); it++){
				for (size_t i = 0; i < cols_; i++) {
					*it += (*this)[index_left][i] * matrix[i][index_right];
				}
				if (index_right != matrix.getCols() - 1) {
					index_right++;
				}
				else{
					index_right = 0;
					index_left++;
				}
			}

			return matrix_new;

		}

		/**
			Operator *=

			@param[in] matrix An instance of class Matrix
		*/
		Matrix<ElementType>& operator*=(Matrix<ElementType> matrix)
		{
			(*this) = (*this) * matrix;

			return (*this);
		}

		/**
			Operator *

			@param[in] a Scalar
		*/
		Matrix<ElementType> operator*(ElementType a)
		{
			Matrix<ElementType> matrix_new(rows_, cols_);
		
			Matrix<ElementType>::Iterator it = begin();
			Matrix<ElementType>::Iterator it_matrix_new = matrix_new.begin();
			while(it != end()){
				*it_matrix_new = *it*a;
				it++;
				it_matrix_new++;
			}

			return matrix_new;
		}

		/**
			Operator *=

			@param[in] a Scalar
		*/
		Matrix<ElementType>& operator*=(ElementType a)
		{
			Matrix<ElementType>::Iterator it = begin();
			while(it != end()){
				*it *= a;
				it++;
			}
			return (*this);
		}

		/**
			Operator /

			@param[in] a Scalar
		*/
		Matrix<ElementType> operator/(ElementType a)
		{
			return (*this) * ((ElementType)1 / a);
		}

		/**
			Operator *=

			@param[in] a Scalar
		*/
		Matrix<ElementType>& operator/=(ElementType a)
		{
			(*this) *= ((ElementType)1 / a);

			return (*this);
		}

		/**
			Transpose matrix
		*/
		Matrix<ElementType> transpose() const
		{
			Matrix<ElementType> matrix_new(cols_, rows_);
			for (size_t row = 0; row < rows_; row++) {
				for (size_t col = 0; col < cols_; col++) {
					matrix_new[col][row] = (*this)[row][col];
				}
			}
			
			return matrix_new;
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
				out << matrix[i][j] << " ";
			}
			out <<  std::endl;
		}

		return out;
	}

}

#endif /* UTILS_MATRIX_H_ */