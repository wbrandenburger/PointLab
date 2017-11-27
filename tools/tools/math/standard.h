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

#ifndef MATH_STANDARD_H_
#define MATH_STANDARD_H_

#include "tools/utils/matrix.h"

namespace math
{
	
	/**
		Constant pi
	*/
	template<typename ElementType> ElementType const PI = static_cast<ElementType>(3.14159265359);

	/**
		Conversion radian to degree

		@param[in] angle_ Angle in radian
	*/
	template<typename ElementType> inline ElementType toDeg(ElementType angle_)
	{
		return angle_ * (ElementType)180.0 / PI<ElementType>;
	}

	/**
		Conversion degree to radian

		@param[in] angle_ Angle in degree
	*/
	template<typename ElementType> inline ElementType toRad(ElementType angle_)
	{
		return angle_ / (ElementType)180.0 * PI<ElementType>;
	}

	/**
		Map the angle between -PI : PI

		@param[in] angle_ Angle in radian
	*/
	template<typename ElementType> inline ElementType inPI(ElementType angle_)
	{
		ElementType pi = PI<ElementType>;
		while (angle_ < -pi || angle_ > pi) 
		{
			angle_ += angle_ < -pi ? pi : -pi;
		}

		return angle_;
	}

	/**
		Computation of the maximal value in a array

		@param[in] a Array
		@param[in] number_of_elements Number of elements
		@return Index of the maximum
	*/
	template<typename ElementType> inline size_t max(ElementType* a, size_t number_of_elements)
	{
		size_t index = NULL;
		ElementType max_element = *a;
		for (size_t i = 0; i < number_of_elements; i++)
		{
			if (max_element < *a) { max_element = *a; index = i; }
			a++;
		}

		return index;
	}

	/**
		Computation of the minimal value in a array

		@param[in] a Array
		@param[in] number_of_elements Number of elements
		@return Index of the minimum
	*/
	template<typename ElementType> inline size_t min(ElementType* a, size_t number_of_elements)
	{
		size_t index = NULL;
		ElementType min_element = *a;
		for (size_t i = 0; i < number_of_elements; i++)
		{
			if (min_element > *a) { min_element = *a; index = i; }
			a++;
		}

		return index;
	}

	/**
		Computation of the maximal values in a matrix

		@param[in] matrix Matrix
		return Matrix with indices of the maximums
	*/
	template<typename ElementType> inline utils::Matrix<size_t> max(const utils::Matrix<ElementType>& matrix)
	{
		utils::Matrix<ElementType> matrix_transpose = matrix.transpose();
		utils::Matrix<size_t> matrix_indices(1, matrix_transpose.getRows());
		for (size_t i = 0; i < matrix_transpose.getRows())
		{
			matrix_indices[0][i] = max(matrix_transpose[i], matrix_transpose.getCols());
		}

		return matrix_indices;
	}

	/**
		Computation of the minimum values in a matrix

		@param[in] matrix Matrix
		return Matrix with indices of the maximums
	*/
	template<typename ElementType> inline utils::Matrix<size_t> min(const utils::Matrix<ElementType>& matrix)
	{
		utils::Matrix<ElementType> matrix_transpose = matrix.transpose();
		utils::Matrix<size_t> matrix_indices(1, matrix_transpose.getRows());
		for (size_t i = 0; i < matrix_transpose.getRows()) 
		{
			matrix_indices[0][i] = min(matrix_transpose[i], matrix_transpose.getCols());
		}

		return matrix_indices;
	}

	/**
		Computation of the mean of an array of data points

		@param[in] data The data points
		@return Mean
	*/
	template<typename ElementType> inline utils::Matrix<ElementType> computeMean(const utils::Matrix<ElementType>& data)
	{
		ElementType* mean = new ElementType[data.getCols()];
		std::memset(mean, (ElementType)0, sizeof(ElementType) * data.getCols());

		computeMean<ElementType>(mean, data);

		return utils::Matrix<ElementType>(mean, data.getCols(), 1);
	}

	/**
		Computation of the mean of an array of data points

		@param[in] mean Vector with the respective mean
		@param[in] data The data points
	*/
	template<typename ElementType> inline void computeMean(ElementType* mean, const utils::Matrix<ElementType>& data)
	{
		/**
			Iterate through the data array and sum the values of a specific dimension
		*/
		size_t i = 0;
		for (utils::Matrix<ElementType>::Iterator it = data.begin(); it != data.end(); it++)
		{
			mean[i % data.getCols()] += *it;
			i++;
		}

		for (size_t i = 0; i < data.getCols(); i++) 
		{
			mean[i] /= data.getRows();
		}
	}

	/**
		Computation of the mean of an array of data points

		@param[in] data The data points
		@param[in] number_of_elements Number of elements
		@return Mean
	*/
	template<typename ElementType> inline ElementType computeMean(ElementType* data, size_t number_of_elements)
	{
		ElementType mean = 0;
		ElementType* end = data + number_of_elements;

		/**
			Iterate through the data array and sum the values of a specific dimension
		*/
		while (data != end)
		{
			mean += *data;
			data++;
		}
		mean /= number_of_elements;

		return mean;
	}

	/**	
		Computation of the variances and covariances of data points

		@param[in] mean Mean
		@param[in] data The data points
		@return Variances and covariances
	*/
	template<typename ElementType> inline utils::Matrix<ElementType> computeVar(const utils::Matrix<ElementType>& mean, const utils::Matrix<ElementType>& data)
	{
		/**
			Computation of the variances and covariances
		*/
		return (data - mean.transpose()).transpose()*(data - mean.transpose()) / (data.getRows()-1);
	}

	/**	
		Computation of the variances and covariances of data points

		@param[in] data The data points
	*/
	template<typename ElementType> inline utils::Matrix<ElementType> computeVar(const utils::Matrix<ElementType>& data)
	{
		/**
			Computation of the variances and covariances
		*/
		return computeVar<ElementType>(computeMean<ElementType>(data), data);
	}

	/**	
		Computation of the variances and covariances of data points

		@param[in] mean Mean
		@param[in] data The data points
		@param[in] number_of_elements Number of elements
		@return Variance
	*/
	template<typename ElementType> inline ElementType computeVar(ElementType mean, ElementType* data, size_t number_of_elements)
	{
		ElementType var = 0;
		ElementType* end = data + number_of_elements;

		/**
			Computation of the variances and covariances
		*/
		while (data != end) 
		{
			var += (*data - mean)*(*data - mean);
			data++;
		}
		var /= (number_of_elements - 1);

		return var;
	}

	/**	
		Computation of the variances and covariances of data points

		@param[in] data The data points
		@param[in] number_of_elements Number of elements
		@return Variance
	*/
	template<typename ElementType> inline ElementType computeVar(ElementType* data, size_t number_of_elements)
	{
		/**
			Computation of the variances and covariances
		*/	
		return computeVar<ElementType>(computeMean<ElementType>(data, number_of_elements), data, number_of_elements);
	}
}

#endif /* MATH_STANDARD_H_ */