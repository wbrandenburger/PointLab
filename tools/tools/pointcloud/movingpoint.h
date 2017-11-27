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

#ifndef POINTCLOUD_MOVINGPOINT_H_
#define POINTCLOUD_MOVINGPOINT_H_

#include "tools/utils/matrix.h"

#include "tools/math/standard.h"

#include "tools/pointcloud/normals.h"

namespace pointcloud
{
	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
	*/
	template<typename ElementType> utils::Matrix<ElementType> plane(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points)
	{
		pointcloud::NormalParams();

		return plane<ElementType>(point, points, pointcloud::computeNormal<ElementType>(points, normal_params));
	}

	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
		@param[in] normal Normal of the reference point
	*/
	template<typename ElementType> utils::Matrix<ElementType> plane(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal)
	{	

		return plane<ElementType>(point, points, normal, math::computeVar<ElementType>(std::sqrt(math::euclideanDistance(points - point.transpose())).getPtr(), points_.getRows()));
	}

	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
		@param[in] normal Normal of the reference point
		@param[in] var Variance of the distances from the points of the neighborhood to the reference point
	*/
	template<typename ElementType> utils::Matrix<ElementType> plane(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal,
		ElementType var,
		size_t eps)
	{
		/**
			Intervall in which the zero is assumed
		*/
		ElementType h = std::sqrt(var) / 2;
		size_t number_of_elements = std::round(2 * h / eps);

		ElementType* x = new ElementType[number_of_elements];
		ElementType* x_ptr = x;

		/**
			Structure which holds the computation for the parameter t which shall be minimized
		*/
		NonLinearPlaneMinimization minimization(point, points, normal, var);

		for (size_t i = 0; i < number_of_elements)
		{

		}
		return utils::Matrix(3, 1);
	}

	template<typename ElementType> struct NonLinearPlaneMinimization
	{	
		/**
			Constructor

			@param[in] point Reference point
			@param[in] points Neighborhood of the point
			@param[in] normal Normal of the reference point
			@param[in] var Variance of the distances from the points of the neighborhood to the reference point
		*/
		NonLinearPlaneMinimization(
			utils::Matrix<ElementType> point,
			utils::Matrix<ElementType> points,
			utils::Matrix<ElementType> normal,
			ElementType var)
		{
			point_ = point;
			points_ = points;
			normal_ = normal;
			var_ = var;
		}

		ElementType operator()(ElementType t)
		{
			ElementType result = ElementType();

			for (size_t i = 0; i < points_.getRows(); i++)
			{
				/**
				Vector between searched point and data point
				*/
				utils::Matrix<ElementType> q = (points_.getRowMatrix(i) - point)_ - normal_ * t;

				/**
				Distance of vector between searched point and data point
				*/
				ElementType qq = std::pow((q.transpose()*q).getValue(), 2);
				ElementType nq = (normal_.transpose()* q).getValue();

				/**
				Computing and Rendering Point Set Surfaces, equation (6)
				*/
				result += nq * (1 + std::pow(nq, 2) / (2 * var_)) * std::exp(-qq / (2 * var_));
			}
			result *= 2;

			return result;
		}

		/**
			Reference point
		*/
		utils::Matrix<ElementType> point_;

		/**
			Neighborhood of the point
		*/
		utils::Matrix<ElementType> points_;

		/**
			Normal of the reference point
		*/
		utils::Matrix<ElementType> normal_;

		/**
			Variance of the distances from the points of the neighborhood to the reference point
		*/
		ElementType var_;
	};
}

#endif /* POINTCLOUD_MOVINGPOINT_H_ */