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

#ifndef MATH_FUNCTION_H_
#define MATH_FUNCTION_H_

#include "tools/utils/matrix.h"

namespace math
{
	/**
		Computes the length of vector a with dimension dim

		@param[in] a Vector
		@param[in] dim Dimension
		@return Length of vector a
	*/
	template<typename ElementType> ElementType euclideanDistance(ElementType* a, size_t dim)
	{
		ElementType result = ElementType();
		ElementType* end = a + dim;
		
		while (a != end) {
			result += *a * *a;
			a++;
		}

		return result;
	}

	/**	
		Computes the length of number_of_elements vectors with dimension dim

		@param[in] a Container with the vectors
		@param[in] number_of_elements Number of vectors
		@param[in] dim Dimension
		@return Array with the length of the vectors
	*/
	template<typename ElementType> ElementType* euclideanDistance(ElementType* a, size_t number_of_elements, size_t dim)
	{
		ElementType* result = new ElementType[number_of_elements];
		ElementType* result_ptr = result;
		ElementType* end = a + number_of_elements * dim;
		
		while (a != end) {
			*result_ptr = euclideanDistance<ElementType>(a, dim);
			a += dim;

			result_ptr++;
		}

		return result;
	}
	
	/**	
		Computes the length of several vectors with dimension dim

		@param[in] matrix Container with the vectors
		@return Container with the length of the vectors
	*/
	template<typename ElementType> utils::Matrix<ElementType> euclideanDistance(const utils::Matrix<ElementType>& matrix)
	{
		return utils::Matrix<ElementType>(euclideanDistance<ElementType>(matrix.getPtr(), matrix.getRows(), matrix.getCols()), matrix.getRows(),1);
	}

	/**
		Computes the distance between vector a and vector b

		@param[in] a Vector
		@param[in] b Vector
		@param[in] dim Dimension
		@return Distance between vector a and vector b
	*/
	template<typename ElementType> ElementType euclideanDistance(ElementType* a, ElementType* b, size_t dim)
	{
		ElementType result = ElementType();
		ElementType* end = a + dim;

		while (a != end) {
			result += (*a - *b)*(*a - *b);
			a++;
			b++;
		}

		return result;
	}

	/**	
		Computes the length of number_of_elements vectors with dimension dim

		@param[in] a Container with the vectors
		@param[in] b Container with the vectors
		@param[in] number_of_elements Number of vectors
		@param[in] dim Dimension
		@return Array with the length of the vectors
	*/
	template<typename ElementType> ElementType* euclideanDistance(ElementType* a, ElementType* b,
		size_t number_of_elements, size_t dim)
	{
		ElementType* result = new ElementType[number_of_elements];
		ElementType* result_ptr = result;
		ElementType* end = a + number_of_elements * dim;
		
		while (a != end) {
			*result_ptr = euclideanDistance<ElementType>(a, b, dim);
			a += dim;
			b += dim;

			result_ptr++;
		}

		return result;
	}

	/**
		Creates a two dimensional polynom of abitrary degree
	*/
	template<typename ElementType> class Polynomial2D 
	{
	public:
		/**
			Constructor

			@param[in] parameter The coefficients of the polynomial
			@param[in] degree Degree of the polynomial
		*/
		Polynomial2D(utils::Matrix<ElementType> parameter, size_t degree) :
			parameter_(parameter), degree_(degree) {}

		/**
			Destructor
		*/
		~Polynomial2D() {}

		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x Function value
			@result Corresponding value
		*/
		ElementType operator()(const ElementType& x) const
		{
			ElementType result = ElementType();
			for (size_t i = 0; i <= degree_; i++) {
				result += parameter_[degree-i][0] * std::pow(x, i);
			}

			return result;
		}

		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x Function values
			@param[in] number_of_elements Number of elements
			@result Corresponding values
		*/
		ElementType operator()(ElementType* x, size_t number_of_elements) const
		{
			ElementType* result = new ElementType[number_of_elements];
			ElementType* result_ptr = result;
			ElementType* end = x + number_of_elements;

			while (x != end) {
				*result_ptr = (*this)(*x);
				result_ptr++;
				x++;
			}

			return result;
		}

		/**
			Operator() Computation of a value corresponding to a function value

			@param[in,out] data Function values
		*/
		void operator()(utils::Matrix<ElementType>& data) const
		{
			 data.setColPtr((*this)(x.getPtr(), x.getRows()), 1);
		}


	private:

		/**
			The coefficients of the polynomial
		*/
		utils::Matrix<ElementType> parameter_;

		/**
			Degree of the polynomial	
		*/
		size_t degree_;
	};


	/**
		Creates a three dimensional polynom of abitrary degree
	*/
	template<typename ElementType> class Polynomial3D
	{
	public:
		/**
			Constructor

			@param[in] parameter The coefficients of the polynomial
			@param[in] degree Degree of the polynomial
		*/
		Polynomial3D(utils::Matrix<ElementType> parameter, size_t degree) :
			parameter_(parameter), degree_(degree) {}

		/**
			Destructor
		*/
		~Polynomial3D() {}


		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x Function value
			@param[in] y Function value
			@result Corresponding value
		*/
		ElementType operator()(const ElementType& x, const ElementType& y) const
		{
			ElementType result = ElementType();
			size_t index = ((degree_ + 2) * (degree_ + 1)) / 2 - 1;
			for (size_t i = 0; i <= degree_; i++) {
				for (size_t j = 0; j <= i; j++) {
					for (size_t k = 0; k <= i; k++) {
						if (j + k == i) {
							result += parameter_[index][0] * std::pow(x, j) * std::pow(y, k);
							index--;
						}
					}
				}
			}

			return result;
		}

		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x Function values
			@param[in] y Function values
			@param[in] number_of_elements Number of elements
			@result Corresponding values
		*/
		ElementType* operator()(ElementType* x, ElementType* y, size_t number_of_elements) const
		{
			ElementType* result = new ElementType[number_of_elements];
			ElementType* result_ptr = result;
			ElementType* end = x + number_of_elements;

			while (x != end) {
				*result_ptr = (*this)(*x,*y);
				result_ptr++;
				x++;
				y++;
			}

			return result;
		}

		/**
			Operator() Computation of a value corresponding to a function value

			@param[in,out] data Function values
		*/
		void operator()(utils::Matrix<ElementType>& data) const
		{
			utils::Matrix<ElementType> data_transpose = data.transpose();
			ElementType* result = (*this)(data_transpose[0], data_transpose[1], data.getRows());

			data.setColPtr(result,2);
			delete[] result;
		}

	private:
		
		/**
			The coefficients of the polynomial
		*/
		utils::Matrix<ElementType> parameter_;

		/**
			Degree of the polynomial
		*/
		size_t degree_;
	};

	/**
		Creates a three dimensional polynom of abitrary degree, 
		which depends on two dimensional functions
	*/
	template<typename ElementType> class Polynomial3DExpand 
	{
		/**
			Constructor

			@param[in] polynomial3D The basical polynom
			@param[in] function_x The function for x
			@param[in] function_y The function for y
		*/
		Polynomial3DExpand(Polynomial3D polynomial3D,
			Polynomial2D<ElementType> function_x, Polynomial2D<ElementType> function_y) :
			polynomial3D_(polynomial3D),
			function_x_(function_x), function_y_(function_y) {}

		/**
			Constructor

			@param[in] parameter The coefficients of the polynomial
			@param[in] degree Degree of the polynomial
			@param[in] function_x The function for x
			@param[in] function_y The function for y
		*/
		Polynomial3DExpand(utils::Matrix<ElementType> parameter, size_t degree,
			Polynomial2D<ElementType> function_x, Polynomial2D<ElementType> function_y) :
			polynomial3D_(Polynomial3D<ElementType>(parameter, degree)),
			function_x_(function_x), function_y_(function_y){}
		
		/**
			Destructor
		*/
		~Polynomial3DExpand() {}
		
		/**
			Operator() Computation of a value corresponding to a function value
			
			@param[in] value Function value
			@return Corresponding value 
		*/
		ElementType operator()(const ElementType& value) const
		{
			return polynomial3D_(function_x_(value),function_y_(value));
		}

		/**
			Operator() Computation of a value corresponding to a function value
			
			@param[in] values Function values
			@param[in] number_of_elements Number of elements
			@return Corresponding values
		*/
		ElementType* operator()(ElementType* values, size_t number_of_elements) const
		{
			return polynomial3D_(function_x_(values),function_y_(values), number_of_elements);
		}

		/**
			Operator() Computation of a value corresponding to a function value
			
			@param[in,out] data Function values
		*/
		void operator()(utils::Matrix<ElementType>& data) const
		{
			utils::Matrix<ElementType> data_transpose = data.transpose();
			ElementType* result = polynomial3D_(function_x_(data_transpose[0], data.getRows()),
				function_y_(data_transpose[0], data.getRows()), number_of_elements);

			data.setColPtr(result,2);
			delete[] result;
		}

		/**
			Polynom
		*/
		Polynomial3D<ElementType> polynomial3D_;

		/**
			Function in x
		*/
		Polynomial2D<ElementType> function_x_;

		/**
			Function in y
		*/
		Polynomial2D<ElementType> function_y_;
	};
}

#endif /* MATH_FUNCTION_H_ */