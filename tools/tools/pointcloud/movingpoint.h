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
	template<typename ElementType> class MovingPoint
	{
	public:

		/**
			Constructor

			@param[in] point Reference point
			@param[in] points Neighborhood of the point
		*/
		MovingPoint(utils::Matrix<ElementType> point,
			utils::Matrix<ElementType> points)
		{
			point_ = point;

			points_ = points;

			pointcloud::NormalParams();

			/**
				Computation of the normal
			*/
			ElementType normal[3];
			pointcloud::computeNormal<ElementType>(normal, points, normal_params);
			normal_.setMatrix(normal, 3, 1);

			/**
				Computation of the variance of the distances
			*/
			var = math::computeVar<ElementType>(std::sqrt(math::euclideanDistance(points_)).getPtr(), points_.getRows());

		}

		/**
			Constructor

			@param[in] point Reference point
			@param[in] points Neighborhood of the point
			@param[in] normal Normal of the reference point
		*/
		MovingPoint(utils::Matrix<ElementType> point,
			utils::Matrix<ElementType> points,
			utils::Matrix<ElementType> normal)
		{
			point_ = point;

			points_ = points;

			normal_ = normal;
			
			/**
				Computation of the variance of the distances
			*/
			var = math::computeVar<ElementType>(std::sqrt(math::euclideanDistance(points_)).getPtr(), points_.getRows());
		}

		/**
			Constructor

			@param[in] point Reference point
			@param[in] points Neighborhood of the point
			@param[in] normal Normal of the reference point
			@param[in] var Variance of the distances from the points of the neighborhood to the reference point
		*/
		MovingPoint(utils::Matrix<ElementType> point,
			utils::Matrix<ElementType> points,
			utils::Matrix<ElementType> normal
			ElementType var)
		{
			point_ = point;

			points_ = points;

			normal_ = normal;

			var_ = var;
		}

		/**
			Destructor
		*/
		~MovingPoint()
		{
		}
		
		/**
			Copy constructor

			@param[in] moving_point An instance of class MovingPoint
		*/
		MovingPoint(const  MovingPoint<ElementType>& moving_point) = delete;

		/**
			Copy constructor
	
			@param[in] moving_point An instance of class MovingPoint
		*/
		MovingPoint(const  MovingPoint<ElementType>&& moving_point) = delete;
	
		/**
			Operator =
	
			@param[in] moving_point An instance of class MovingPoint
			@return Returns reference to the current instance
		*/
		MovingPoint& operator=(const  MovingPoint<ElementType>& moving_point) = delete;
	
		/**
			Operator =
		
			@param[in] moving_point An instance of class MovingPoint
			@return Returns reference to the current instance
		*/
		MovingPoint& operator=(const  MovingPoint<ElementType>&& moving_point) = delete;
		
		/**
			Set MovingPoint
		*/
		void setMovingPoint()
		{
			
		}

	private:
		/**
			Point
		*/
		utils::Matrix<ElementType> point_;

		/**
			Neighborhood
		*/
		utils::Matrix<ElementType> points_;

		/**
			Normal
		*/
		utils::Matrix<ElementType> normal_;

		/**
			Variance
		*/
		ElementType var_;
	};
}

#endif /* POINTCLOUD_MOVINGPOINT_H_ */