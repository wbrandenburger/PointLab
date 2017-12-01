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

#ifndef MATH_ADJUSTMENT_H_
#define MATH_ADJUSTMENT_H_

#include "tools/utils/matrix.h"

namespace math
{
	/**
		Build the desgin matrix for a two dimensional polynom
		x^n + x^n-1 + ... x^3 + x^2 + x + 1

		@param[in] data A matrix with the data array
		@param[in,out] design_matrix The resulting design matrix
		@param[in] degree The polynomial degree
	*/
	template<typename ElementType> void buildDesignMatrixPolynomial2D(
		const utils::Matrix<ElementType>& data,
		utils::Matrix<ElementType>& design_matrix,
		size_t degree)
	{
		design_matrix.setMatrix(data.getRows(), degree + 1);
		for (size_t i = 0; i < data.getRows(), i++) {
			for (size_t j = 0; j <= degree_; j++) {
				design_matrix[i][degree-j] = std::pow(data[i][0], j);
			}
		}
	}

	/**
		Build the desgin matrix for a three dimensional polynom
		 x^n +x^n-1y + ... + xy^n-1+ y^n + ... + x^2 + xy + y^2 + x + y + 1

		@param[in] data A matrix with the data array
		@param[in,out] design_matrix The resulting design matrix
		@param[in] degree The polynomial degree
	*/
	template<typename ElementType> void buildDesignMatrixPolynomial3D(
		const utils::Matrix<ElementType>& data,
		utils::Matrix<ElementType>& design_matrix,
		size_t degree)
	{
		size_t number_of_parameter = ((degree + 2) * (degree + 1)) / 2;
		design_matrix.setMatrix(data.getRows(), number_of_parameter);
		for (size_t i = 0; i < data.getRows(); i++) {
			size_t index = number_of_parameter - 1;
			for (size_t j = 0; j <= degree; j++) {
				for (size_t k = 0; k <= j; k++) {
					size_t l = j - k;
					design_matrix[i][index] = std::pow(data[i][0], k) * std::pow(data[i][1], l);
					index--;
				}
			}
		}
	}
}

#endif /* MATH_ADJUSTMENT_H_ */