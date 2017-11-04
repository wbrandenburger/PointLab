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

#ifndef POINTCLOUD_MISCELLANEOUS_H_
#define POINTCLOUD_MISCELLANEOUS_H_

#include "tools/pointcloud/pointcloud.h"

#include "tools/parameters.h"

namespace pointcloud
{
	/**
		Computes a 2D-meshgrid of a specific range

		@param[in,out] pointcloud_ Reference to a empty container where the mehsgrid will be saved
		@param[in] x_left_ Left border in x-direction
		@param[in] x_right_ Right border in x-direction
		@param[in] y_left_ Left border in y-direction
		@param[in] y_right_ Right borger in y-direction
		@param[in] quant_ Difference between two adjacent points
		@param[in] indices_ True if a trinagulated meshgrid should be created
	*/
	template<typename ElementType, typename PointcloudType> void meshGrid(PointcloudType& pointcloud,
		ElementType x_left,ElementType x_right, ElementType y_left, ElementType y_right, 
		ElementType quant)
	{
		/** 
			Computation of the number of resulting elements and set the pointcloud
		*/
		size_t number_x = std::floor((x_right - x_left) / quant) + 1;
		size_t number_y = std::floor((y_right - y_left) / quant) + 1;

		size_t number_of_elements = number_x * number_y;
		size_t number_of_triangles = (number_x - 1)*(number_y - 1) * 2;
		
		/**
			Set pointcloud and allocate memory 
		*/
		pointcloud.setNumberOfElements(number_of_elements, number_of_triangles);
		pointcloud.setTriangles();

		pointcloud.setPointcloud();

		/**
			Set the pointcloud
		*/
		typedef typename PointcloudType::template Iterator<ElementType> IteratorElementType;
		typedef typename PointcloudType::template Iterator<size_t> Iteratorsize_t;
		IteratorElementType iterator_points = pointcloud.beginPoint();
		Iteratorsize_t iterator_triangles = pointcloud.beginTriangle();

		for (ElementType x = x_left; x <= x_right; x += quant) {
			for (ElementType y = y_left; y <= y_right; y += quant) {
				/**
					Assign the x- and y-values
				*/
				*iterator_points = x; iterator_points++;
				*iterator_points = y; iterator_points++;
				/**
					Skip z-value
				*/
				iterator_points++;
			}
		}

		size_t index = 0;
		for (size_t i = 0; i < number_of_triangles / 2; i++) {
			
			if ((index + 1) % number_x == 0){
				index++;
			}

			/**
				Top	left, top right and bottom left corner
			*/
			*iterator_triangles = index; iterator_triangles++;
			*iterator_triangles = index + 1; iterator_triangles++;
			*iterator_triangles = index + number_x; iterator_triangles++;
			/**
				Top	left, top right and bottom left corner
			*/
			*iterator_triangles = index + 1; iterator_triangles++;
			*iterator_triangles = index + number_x + 1; iterator_triangles++;
			*iterator_triangles = index + number_x; iterator_triangles++;

			index++;
		}
	}
}

#endif /* POINTCLOUD_MISCELLANEOUS_H_ */