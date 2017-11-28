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

#ifndef POINTCLOUD_PLANE_H_
#define POINTCLOUD_PLANE_H_

#include "tools/utils/matrix.h"

#include "tools/math/standard.h"
#include "tools/math/zero.h"

#include "tools/pointcloud/normals.h"

namespace pointcloud
{	
	/**
		Structure which holds tree parameters
	*/
	struct SurfaceParams 
	{
	public:
		/**
			Constructor
		*/
		SurfaceParams() :
			surface_computation_(SurfaceComputation::PLANEMLS),
			roots_approximation_(RootsApproximation::NEWTON),
			weight_function_(WeightFunction::GAUSSIAN),
			eps_(0.001),
			cores_(1)
		{
		}

		/**
			Constructor

			@param[in] normal_computation Method which will be used for computing normals
			@param[in] weight_function Defines the weight function
			@param[in] cores Number of cores
		*/
		SurfaceParams(SurfaceComputation surface_computation,
			RootsApproximation roots_approximation,
			WeightFunction weight_function,
			float eps,
			size_t cores)  : SurfaceParams()
		{
			surface_computation_ = surface_computation;
			roots_approximation_ = roots_approximation;
			weight_function_ = weight_function;
			eps_ = eps;
			cores_ = cores;
		}

		/**
			Set method which will be used for computing surface
		*/
		void setSurfaceComputation(SurfaceComputation surface_computation)
		{
			surface_computation_ = surface_computation;
		}

		/**
			Set the method which will be used for approximating the root
		*/
		void setRootsApproximation(RootsApproximation roots_approximation)
		{
			roots_approximation_ = roots_approximation;
		}

		/**
			Set the weight function
		*/
		void setWeightFunction(WeightFunction weight_function)
		{
			weight_function_ = weight_function;
		}

		/**
			Set accuracy
		*/
		void setAccuracy(float eps) 
		{
			eps_ = eps;
		}

		/**
			Set number of cores
		*/
		void setCores(size_t cores)
		{
			cores_ = cores;
		}

		/**
			Get method which will be used for computing surface

			@return Method which will be used for computing surface
		*/
		SurfaceComputation getSurfaceComputation()
		{
			return surface_computation_;
		}

		/**
			Get method which will be used for approximating the root

			@return Method which will be used for approximating roots
		*/
		RootsApproximation getRootsApproximation()
		{
			return roots_approximation_;
		}
		/**
			Get the weight function

			@return Weight function
		*/
		WeightFunction getWeightFunction()
		{
			return weight_function_;
		}
		
		/**
			Get accuracy
		*/
		float getAccuracy() 
		{
			return eps_;
		}

		/**
			Get number of cores
			
			@return Number of cores
		*/
		size_t getCores()
		{
			return cores_;
		}

	private:
		/**
			Method for computing surface
		*/
		SurfaceComputation surface_computation_;

		/**
			Method for approximating roots
		*/
		RootsApproximation roots_approximation_;

		/**
			WeightFunction weight_function
		*/
		WeightFunction weight_function_;

		/**
			Accuracy of computation of the root
		*/
		float eps_;

		/**
			Cores
		*/
		size_t cores_;
	};

	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
		@param[in] normal Normal of the reference point
		@param[in] surface_params Parameter for computing the surface
	*/
	template<typename ElementType> utils::Matrix<ElementType> planeMLS(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal,
		SurfaceParams surface_params)
	{	

		return planeMLS <ElementType>(point, points, normal, math::computeVar<ElementType>(std::sqrt(math::euclideanDistance(points - point.transpose())).getPtr(), points.getRows()),surface_params);
	}

	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
		@param[in] normal Normal of the reference point
		@param[in] var Variance of the distances from the points of the neighborhood to the reference point
		@param[in] surface_params Parameter for computing the surface
	*/
	template<typename ElementType> utils::Matrix<ElementType> planeMLS(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal,
		ElementType var,
		SurfaceParams surface_params)
	{
		/**
			Intervall in which zero is assumed
		*/
		ElementType h = std::sqrt(var) / 2;
		size_t number_of_elements = std::floor(1/surface_params.getAccuracy());
		ElementType size_of_steps =  2*h / number_of_elements;

		ElementType* y = new ElementType[number_of_elements];
		ElementType* y_end = y + number_of_elements;

		/**
			Structure which holds the computation for the parameter t to be minimized
		*/
		NonLinearPlaneMLSMinimization<ElementType> minimization(point, points, normal, var);

		/**
			Computation the zero in the intervall in -h/2 and h/2
		*/
		ElementType t;
		switch (surface_params.getRootsApproximation()) {
		case RootsApproximation::NEWTON: {
			t = math::NewtonMethod<ElementType, NonLinearPlaneMLSMinimization<ElementType>>(
				minimization, h, -h, h * surface_params.getAccuracy());
			break;
			}
		case RootsApproximation::QUAD: {
			t = math::QuadraticInverseInterpolation<ElementType, NonLinearPlaneMLSMinimization<ElementType>>(
				minimization, h, -h, h * surface_params.getAccuracy());
			break;
			}
		}

		return point + normal*t;
	}

	template<typename ElementType> struct NonLinearPlaneMLSMinimization
	{	
		/**
			Constructor

			@param[in] point Reference point
			@param[in] points Neighborhood of the point
			@param[in] normal Normal of the reference point
			@param[in] var Variance of the distances from the points of the neighborhood to the reference point
		*/
		NonLinearPlaneMLSMinimization(
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
				utils::Matrix<ElementType> q = (points_.getRowMatrix(i) - point_) - normal_ * t;

				/**
					Distance of vector between searched point and data point
				*/
				ElementType qq = std::pow((q.transpose() * q).getValue(), 2);
				ElementType nq = (normal_.transpose() * q).getValue();

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

#endif /* POINTCLOUD_PLANE_H_ */