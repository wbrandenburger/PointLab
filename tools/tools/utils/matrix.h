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

#include <initializer_list>

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
			Constructor

			@param[in] data_ Initializer list
			@param[in] rows_ Rows of the matrix
			@param[in] cols_ Columns of the matrix
		*/
		Matrix(std::initializer_list<ElementType> data, size_t rows, size_t cols) : Matrix()
		{
			rows_ = rows;
			cols_ = cols;

			data_ = new ElementType[rows_ * cols_];
			Matrix<ElementType>::Iterator it = begin();
			auto it_data = data.begin();
			while (it != end()) {
				*it = *it_data;
				it++;
				it_data++;
			}
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
			Set Matrix

			@param[in] data_ Initializer list
			@param[in] rows_ Rows of the matrix
			@param[in] cols_ Columns of the matrix
		*/
		void setMatrix(std::initializer_list<ElementType> data, size_t rows, size_t cols)
		{
			clearMemory();

			rows_ = rows;
			cols_ = cols;

			data_ = new ElementType[rows_ * cols_];
			Matrix<ElementType>::Iterator it = begin();
			auto it_data = data.begin();
			while (it != end()) {
				*it = *it_data;
				it++;
				it_data++;
			}
		}

		/**
			Set row

			@param[in] ptr Pointer to the row
			@param[in] row Row
		*/
		void setRowPtr(ElementType* ptr, size_t row)
		{
			std::memcpy(data_ + row * cols_, ptr, sizeof(ElementType) * cols_);
		}

		/**
			Set column

			@param[in] ptr Pointer to the column
			@param[in] col Column
		*/
		void setColPtr(ElementType* ptr, size_t col)
		{
			for(size_t i = 0; i < rows_; i++)
			{
				(*this)[i][col] = ptr[i];
			}
		}

		/**
			Set row

			@param[in] matrix Matrix with the row
			@param[in] row Row
		*/
		void setRowMatrix(const Matrix<ElementType> matrix, size_t row)
		{
			setRowPtr(matrix.getPtr(), row);
		}

		/**
			Set column

			@param[in] matrix An instance of class Matrix
			@param[in] col Column
		*/
		void setColMatrix(const Matrix<ElementType> matrix, size_t col)
		{
			setColPtr(matrix.getPtr(), col);
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
			ElementType* data_ptr = new ElementType[rows_ * cols_];
			std::memcpy(data_ptr, data_, sizeof(ElementType) * rows_ * cols_);

			return data_ptr;
		}

		/**
			Returns the pointer to a row

			@param[in] row Row
			@return Return pointer to the row
		*/
		ElementType* getAllocatedRowPtr(size_t row) const
		{
			ElementType* row_ptr = new ElementType[cols_];
			std::memcpy(row_ptr, (*this)[row], sizeof(ElementType) * cols_);

			return row_ptr;
		}

		/**
			Returns the pointer to a column

			@param[in] col Column
			@return Return pointer to the column
		*/
		ElementType* getAllocatedColPtr(size_t col) const
		{
			ElementType* col_ptr = new ElementType[rows_];
			for (size_t i = 0; i < rows_; i++) {
				col_ptr[i] = (*this)[i][col];
			}

			return col_ptr;
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
			Returns the number of columns

			@return Number of columns
		*/
		size_t getCols() const
		{
			return cols_;
		}

		/**
			Returns the first value
		*/
		ElementType getValue() const
		{
			return data_[0];
		}

		/**
			Get a specific row as matrix

			@param[in] matrix An instance of class Matrix
			@param[in] row Row
		*/
		Matrix<ElementType> getRowMatrix(size_t row) const
		{
			return Matrix<ElementType>(getAllocatedRowPtr(row), cols_, 1);
		}

		/**
			Get a specific column as matrix

			@param[in] matrix An instance of class Matrix
			@param[in] col Column
		*/
		Matrix<ElementType> getColMatrix(size_t col) const
		{
			return Matrix<ElementType>(getAllocatedColPtr(col), rows_, 1);
		}

		/**
			Get matrix of another type
		*/
		template<typename ResultType> Matrix<ResultType> getType() const
		{
			Matrix<ResultType> matrix(rows_, cols_);
			Matrix<ResultType>::Iterator it_matrix = matrix.begin();
			Matrix<ElementType>::Iterator it = begin();

			while (it != end())
			{
				*it_matrix = (ResultType)*it;
				it_matrix++;
				it++;
			}

			return matrix;
		}

		/**
			Concatenate two matrices rowwise

			@param[in] matrix An instance of class Matrix
			@return Concatenated matrices
		*/
		Matrix<ElementType> conctatenateRow(Matrix<ElementType> matrix)
		{
			if (cols_ != matrix.getCols())
			{
				exitFailure(__FILE__, __LINE__);
			}

			ElementType* data_new = new ElementType[rows_ * cols_ + matrix.getRows() * matrix.getCols()];
			std:memcpy(data_new, data_, sizeof(ElementType) * rows_ * cols_);

			ElementType* data_new_ptr = data_new + rows_ * cols_;
			std::memcpy(data_new_ptr, matrix.getPtr(), sizeof(ElementType) * matrix.getRows() * matrix.getCols());

			return Matrix<ElementType>(data_new, rows_ + matrix.getRows(), cols_);
		}

		/**
			Concatenate two matrices columnwise

			@param[in] matrix An instance of class Matrix
			@return Concatenated matrices
		*/
		Matrix<ElementType> conctatenateCol(Matrix<ElementType> matrix)
		{
			if (rows_ != matrix.getRows())
			{
				exitFailure(__FILE__, __LINE__);
			}

			Matrix<ElementType> matrix_left = transpose();
			Matrix<ElementType> matrix_right = matrix.transpose();

			return matrix_left.conctatenateRow(matrix_right).transpose();
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
			Operator+ Adds a scalar to the matrix

			@param[in] a Scalar
			@return Solution
		*/
		Matrix<ElementType> operator+(ElementType a) const
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
			Operator+= Add a scalar to the matrix

			@param[in] a Scalar
			@return Solution
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
			Operator- Subtract a scalar from the matrix

			@param[in] a Scalar
			@return Solution
		*/
		Matrix<ElementType> operator-(ElementType a) const
		{
			return (*this) + (-1) * a;
		}	
				
		/**
			Operator- Subtract a scalar from th matrix

			@param[in] a Scalar
			@return Solution
		*/
		Matrix<ElementType>& operator-=(ElementType a)
		{
			(*this) += (-1) * a;

			return (*this);
		}

		/**
			Operator+ Add a matrix to the matrix

			@param[in] matrix An instance of class Matrix
			@return Solution
		*/
		Matrix<ElementType> operator+(const Matrix<ElementType>& matrix) const
		{
			Matrix<ElementType> matrix_new(rows_, cols_);
			
			Matrix<ElementType>::Iterator it = begin();
			Matrix<ElementType>::Iterator it_matrix_new = matrix_new.begin();
			/**
				Add a mxn-matrix to the mxn matrix
			*/
			if (rows_ == matrix.getRows() && cols_ == matrix.getCols()) {
				Matrix<ElementType>::Iterator it_matrix = matrix.begin();

				while (it != end()) {
					*it_matrix_new = *it + *it_matrix;
					it_matrix_new++;
					it_matrix++;
					it++;
				}
			}
			/**
				Add 1xn-matrix to the mxn-matrix
			*/
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
			Operator+= Add a matrix to the matrix

			@param[in] matrix An instance of class Matrix
			@return Solution
		*/
		Matrix<ElementType>& operator+=(const Matrix<ElementType>& matrix)
		{
			Matrix<ElementType>::Iterator it = begin();
			/**
				Add a mxn-matrix to the mxn matrix
			*/
			if (rows_ == matrix.getRows() && cols_ == matrix.getCols()) {
				Matrix<ElementType>::Iterator it_matrix = matrix.begin();

				while (it != end()) {
					*it += *it_matrix;
					it_matrix++;
					it++;
				}
			}
			/**
				Add 1xn-matrix to the mxn-matrix
			*/
			else if (matrix.getRows() == 1 && cols_ == matrix.getCols()) {
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
			Operator- Subtract a matrix from the matrix

			@param[in] matrix An instance of class Matrix
			@return Solution
		*/
		Matrix<ElementType> operator-(const Matrix<ElementType>& matrix) const
		{
			return (*this) + matrix*(-1);
		}

	private:

		/**
			Add a row to the matrix

			@param[in] matrix An instance of class matrix
			@param[in] row Row
		*/
		void addRow(const Matrix<ElementType>& matrix, size_t row)
		{
			for (size_t i = 0; i < cols_; i++)
			{
				(*this)[row][i] += matrix[0][i];
			}
		}

		/**
			Subtract a row from the matrix

			@param[in] matrix An instance of class matrix
			@param[in] row Row
		*/
		void subtractRow(const Matrix<ElementType>& matrix, size_t row)
		{
			addRow(matrix * (-1), row);
		}

		/**
			Add a column to the matrix

			@param[in] matrix An instance of class matrix
			@param[in] col Column
		*/
		void addCol(const Matrix<ElementType>& matrix, size_t col)
		{
			for (size_t i = 0; i < rows_; i++)
			{
				(*this)[i][col] += matrix[i][col];
			}
		}

		/**
			Subtract a column from the matrix

			@param[in] matrix An instance of class matrix
			@param[in] col Column
		*/
		void subtractCol(const Matrix<ElementType>& matrix, size_t col)
		{
			addRow(matrix * (-1), col);
		}

	public:

		/**
			Operator- Subtract a matrix from the matrix

			@param[in] matrix An instance of class Matrix
			@return Solution
		*/
		Matrix<ElementType>& operator-=(const Matrix<ElementType>& matrix)
		{
			*this += matrix * (-1);
			
			return (*this);
		}

		/**
			Operator* Multiply two matrices

			@param[in] matrix An instance of class Matrix
			@return Solution
		*/
		Matrix<ElementType> operator*(const Matrix<ElementType>& matrix) const
		{
			Matrix<ElementType> matrix_new(rows_, matrix.getCols());
			/**
				Multiply a nxm-matrix with a mxo-matrix
			*/
			if (cols_ == matrix.getRows()) {
				size_t index_left = 0, index_right = 0;
				for (Matrix<ElementType>::Iterator it = matrix_new.begin(); it != matrix_new.end(); it++) {
					for (size_t i = 0; i < cols_; i++) {
						*it += (*this)[index_left][i] * matrix[i][index_right];
					}
					if (index_right != matrix.getCols() - 1) {
						index_right++;
					}
					else {
						index_right = 0;
						index_left++;
					}
				}
			}
			/**
				Multiply a nx1-vector with a nxm-matrix
			*/
			else if (rows_ == matrix.getRows() && cols_ == 1) {
				Matrix<ElementType>::Iterator it = begin();
				Matrix<ElementType>::Iterator it_matrix = matrix.begin();
				Matrix<ElementType>::Iterator it_matrix_new = matrix_new.begin();
				size_t index = 0;	
				while (it_matrix != matrix.end()) {
					*it_matrix_new = *it_matrix *  *it;
					if (index == matrix.getCols() - 1) {
						index = 0;
						it++;
					}
					else {
						index++;
					}
					it_matrix_new++;
					it_matrix++;
				}
			}
			else {
				exitFailure(__FILE__, __LINE__);
			}

			return matrix_new;

		}

		/**
			Operator*= Multiply two matrices

			@param[in] matrix An instance of class Matrix
			@return Solution
		*/
		Matrix<ElementType>& operator*=(const Matrix<ElementType>& matrix)
		{
			(*this) = (*this) * matrix;

			return (*this);
		}

		/**
			Operator* Multiply a matrix with a scalar

			@param[in] a Scalar
			@return Solution
		*/
		Matrix<ElementType> operator*(ElementType a) const
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
			Operator*= Multiply a matrix with a scalar

			@param[in] a Scalar
			@return Solution
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
			Operator/ Divide the matrix with a scalar

			@param[in] a Scalar
			@return Solution
		*/
		Matrix<ElementType> operator/(ElementType a) const
		{
			return (*this) * ((ElementType)1 / a);
		}

		/**
			Operator/= Divide the matrix with a scalar

			@param[in] a Scalar
			@return Solution
		*/
		Matrix<ElementType>& operator/=(ElementType a)
		{
			(*this) *= ((ElementType)1 / a);

			return (*this);
		}

		/**
			Transpose the matrix

			@return Transpose of the matrix
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

		/**
			Invert the matrix

			@return Inverse of the matrix
		*/
		Matrix<ElementType> inverse() const
		{
			if (rows_ != cols_) {
				exitFailure(__FILE__, __LINE__);
			}

			Matrix<ElementType> inv(rows_, cols_);
			Matrix<ElementType> equation_left = (*this);
			for (size_t i = 0; i < cols_; i++) {
				Matrix<ElementType> equation_right(rows_, 1);
				equation_right[i][0] = (ElementType) 1.0;

				Matrix<ElementType> equation = equation_left.conctatenateCol(equation_right);

				inv.setColMatrix(equation.gaussJordanElimination(), i);
			}

			return inv;
		}

		/**
			Solves a system of linear equations 

			@return Solution of the system of linear equations
		*/
		Matrix<ElementType> gaussJordanElimination() const
		{
			if (rows_ != cols_ - 1) {
				exitFailure(__FILE__, __LINE__);
			}
			
			Matrix<ElementType> matrix = (*this);

			/**
				Eliminiate the coefficents under the diagonal
			*/
			for (size_t i = 0; i < rows_; i++)
			{
				Matrix<ElementType> row = matrix.getRowMatrix(i) / matrix[i][i];
				matrix.setRowMatrix(row, i);
				for (size_t j = i + 1; j < rows_; j++)
				{
					matrix.subtractRow(row * matrix[j][i], j);
				}
			}

			/**
				Eliminate the coefficents above the diagonal
			*/
			for (size_t i = rows_ - 1; i > 0; i--)
			{
				Matrix<ElementType> row = matrix.getRowMatrix(i);
				for (size_t j = 0; j < i; j++)
				{
					matrix.subtractRow(row * matrix[j][i], j);
				}
			}

			return matrix.getColMatrix(cols_ - 1);
		}

		/**
			Get the diagonal elements of a quadratic matrix

			@return the diagonal elements of a quadratic matrix
		*/
		ElementType* getDiag() const
		{
			if (rows_ != cols_) {
				exitFailure(__FILE__,__LINE__);
			}
			
			ElementType* diag = new ElementType[rows_];
			for (size_t i = 0; i < rows_; i++) {
				diag[i] = (*this)[i][i];
			}

			return diag;
		}

		/**
			Get the diagonal elements of a quadratic matrix

			@param[in] matrix Container which includes the diagonal elements
		*/
		void getDiag(Matrix<ElementType>& matrix) const
		{
			if (rows_ != cols_) {
				exitFailure(__FILE__, __LINE__);
			}
			
			matrix.setMatrix(1, rows_);
			for (size_t i = 0; i < rows_; i++) {
				matrix[0][i] = (*this)[i][i];
			}
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

namespace std 
{
	template<typename ElementType> utils::Matrix<ElementType>& pow(utils::Matrix<ElementType>& matrix, ElementType scalar) 
	{
		for (utils::Matrix<ElementType>::Iterator it = matrix.begin(); it != matrix.end(); it++) {
			*it = std::pow(*it, scalar);
		}

		return matrix;
	}

	template<typename ElementType> utils::Matrix<ElementType>& sqrt(utils::Matrix<ElementType>& matrix)
	{
		for (utils::Matrix<ElementType>::Iterator it = matrix.begin(); it != matrix.end(); it++) {
			*it = std::sqrt(*it);
		}

		return matrix;
	}
}

#endif /* UTILS_MATRIX_H_ */