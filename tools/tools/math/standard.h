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
		while (angle_ < -pi || angle_ > pi) {
			angle_ += angle_ < -pi ? pi : -pi;
		}

		return angle_;
	}
	/**
		Computation of the mean of an array of data points

		@param[in,out] mean The container whilch holds the mean
		@param[in] data The data points
	*/
	template<typename ElementType> inline void computeMean(utils::Matrix<ElementType>& mean, const utils::Matrix<ElementType>& data)
	{
		mean = utils::Matrix<ElementType>(computeMean<ElementType>(data), 1, data.getCols());
	}

	/**
		Computation of the mean of an array of data points

		@param[in,out] mean The container whilch holds the mean
		@param[in] data The data points
	*/
	template<typename ElementType> inline ElementType* computeMean(const utils::Matrix<ElementType>& data)
	{
		ElementType* mean = new ElementType[data.getCols()];
		std::memset(mean, (ElementType)0, sizeof(ElementType) * data.getCols());

		/**
			Iterate through the data array and sum the values of a specific dimension
		*/
		size_t i = 0;
		for (utils::Matrix<ElementType>::Iterator it = data.begin(); it != data.end(); it++)
		{
			mean[i % data.getCols()] += *it;
			i++;
		}

		for (size_t i = 0; i < data.getCols(); i++) {
			mean[i] /= data.getRows();
		}

		return mean;
	}

	/**	
		Computation of the variances and covariances of data points

		@param[in,out] var Container which holds the variances and covariances
		@param[in] data The data points
	*/
	template<typename ElementType> inline void computeVar(utils::Matrix<ElementType>& var, const utils::Matrix<ElementType>& data)
	{
		/**
			Compute mean
		*/
		utils::Matrix<ElementType> mean;
		computeMean<ElementType>(mean, data);

		/**
			Computation of the variances and covariances
		*/
		var = (data - mean).transpose()*(data - mean);
	}
}

#endif /* MATH_STANDARD_H_ */