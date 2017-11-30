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

#ifndef POINTCLOUD_SURFACE_H_
#define POINTCLOUD_SURFACE_H_

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
			roots_approximation_(RootsApproximation::QUAD),
			weight_function_(WeightFunction::GAUSSIAN),
			polynomial_degree_(4),
			eps_(0.001),
			cores_(1)
		{
		}

		/**
			Constructor

			@param[in] surface_computation Method which will be used for computing surfaces
			@param[in] roots_approximation Method which will be used for approximating the root
			@param[in] weight_function Defines the weight function
			@param[in] polynomial_degree Degree of the polynom used to approximate the surface
			@param[in] eps Accuracy for computation of the root
			@param[in] cores Number of cores
		*/
		SurfaceParams(SurfaceComputation surface_computation,
			RootsApproximation roots_approximation,
			WeightFunction weight_function,
			size_t polynomial_degree,
			float eps,
			size_t cores)  : SurfaceParams()
		{
			surface_computation_ = surface_computation;
			roots_approximation_ = roots_approximation;
			weight_function_ = weight_function;
			polynomial_degree_ = polynomial_degree;
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
			Set the degree of the polynom used to approximate the surface
		*/
		void setPolynomialDegree(size_t polynomial_degree)
		{
			polynomial_degree_ = polynomial_degree;
		}

		/**
			Set accuracy for computation of the root
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
		SurfaceComputation getSurfaceComputation() const
		{
			return surface_computation_;
		}

		/**
			Get method which will be used for approximating the root

			@return Method which will be used for approximating roots
		*/
		RootsApproximation getRootsApproximation() const
		{
			return roots_approximation_;
		}
		/**
			Get the weight function

			@return Weight function
		*/
		WeightFunction getWeightFunction() const
		{
			return weight_function_;
		}

		/**
			Get the degree of the polynom used to approximate the surface
		*/
		size_t getPolynomialDegree() const
		{
			return polynomial_degree_;
		}	

		/**
			Get accuracy for computation of the root
		*/
		float getAccuracy() const
		{
			return eps_;
		}

		/**
			Get number of cores
			
			@return Number of cores
		*/
		size_t getCores() const
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
			Degree of the polynom used to approximate the surface
		*/
		size_t polynomial_degree_;

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
		Surface computation switch which depends on the chosen method for computing surfaces
		
		@param[in] point Reference point
		@param[in] points Matrix with the points
		@param[in] surface_params Parameter for computing normals
		@return Normal
	*/
	template<typename ElementType> utils::Matrix<ElementType> computeSurface(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal,
		SurfaceParams surface_params = SurfaceParams())
	{
		utils::Matrix<ElementType> parameter;

		switch (surface_params.getSurfaceComputation()) {
		case SurfaceComputation:PLANE: parameter = plane(point, points, normal, surface_params); break;
		case SurfaceComputation::PLANEMLS: parameter = planeMLS(point, points, normal, surface_params); break;
		case SurfaceComputation::SURF: parameter = surf(point, points, normal, surface_params); break;
		case SurfaceComputation::SRUFMLS: parameter = surfMLS(point, points, normal, surface_params); break;
		}

		return parameter;
	}

	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
		@param[in] normal Normal of the reference point
		@param[in] var Variance of the distances from the points of the neighborhood to the reference point
		@param[in] surface_params Parameter for computing the surface
		@return Reference point of the moving plane
	*/
	template<typename ElementType> utils::Matrix<ElementType> pointMLS(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal,
		SurfaceParams surface_params)
	{
		/**
			Computation of the variance of the disatances from every point to the reference point
		*/
		ElementType var = math::computeVar<ElementType>(std::sqrt(math::euclideanDistance(points - point.transpose())).getPtr(), points.getRows());

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

	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
		@param[in] normal Normal of the reference point
		@param[in] surface_params Parameter for computing the surface
		@return Parameters of the plane
	*/
	template<typename ElementType> utils::Matrix<ElementType> plane(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal,
		SurfaceParams surface_params)
	{
		return utils::Matrix<ElementType>({	normal[0][0] / normal[2][0] * (-1),
											normal[1][0] / normal[2][0] * (-1),
											(point.transpose() * normal).getValue() / normal[2][0]},
											3, 1);
	}

	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
		@param[in] normal Normal of the reference point
		@param[in] surface_params Parameter for computing the surface
		@return Parameters of the plane
	*/
	template<typename ElementType> utils::Matrix<ElementType> planeMLS(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal,
		SurfaceParams surface_params)
	{
		utils::Matrix<ElementType> reference_point = pointMLS(
			point,
			points,
			normal,
			surface_params);
	
		return utils::Matrix<ElementType>({	normal[0][0] / normal[2][0] * (-1),
											normal[1][0] / normal[2][0] * (-1),
											(reference_point.transpose() * normal).getValue() / normal[2][0]},
											3, 1);
	}

	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
		@param[in] normal Normal of the reference point
		@param[in] surface_params Parameter for computing the surface
		@return Parameters of the plane
	*/
	template<typename ElementType> utils::Matrix<ElementType> surf(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal,
		SurfaceParams surface_params)
	{
		return utils::Matrix<ElementType>();
	}

	/**
		Computes the supporting plane of a neighborhood of points

		@param[in] point Reference point
		@param[in] points Neighborhood of the point
		@param[in] normal Normal of the reference point
		@param[in] surface_params Parameter for computing the surface
		@return Parameters of the plane
	*/
	template<typename ElementType> utils::Matrix<ElementType> surfMLS(
		const utils::Matrix<ElementType>& point,
		const utils::Matrix<ElementType>& points,
		const utils::Matrix<ElementType>& normal,
		SurfaceParams surface_params)
	{
		utils::Matrix<ElementType> reference_point = pointMLS(
			point,
			points,
			normal,
			surface_params);

		return utils::Matrix<ElementType>();
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

#endif /* POINTCLOUD_SURFACE_H_ */