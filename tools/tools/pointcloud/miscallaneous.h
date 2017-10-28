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
	template<typename ElementType> void meshGrid(pointcloud::Pointcloud<ElementType>& pointcloud_,
		ElementType x_left_,ElementType x_right_, ElementType y_left_, ElementType y_right_, 
		ElementType quant_, bool indices = true)
	{
		/** 
			Computation of the number of resulting elements and set the pointcloud
		*/
		size_t number_x = std::ceil((x_right_ - x_left_) / quant_);
		size_t number_y = std::ceil((y_right_ - y_left_) / quant_);

		size_t number_of_elements = number_x * number_y;
		pointcloud_.setPointcloud(number_of_elements);

		/**
			Set the meshgrid
		*/
		size_t index = 0;
		for (ElementType x = x_left_; x <= x_right; x += quant_) {
			for (ElementType y = y_left; y <= y_left; y += quant) {
				pointcloud_.setPoint(x_left, index, 0);
				pointcloud_.setPoint(y_left, index, 1);
				index++;
			}
		}
	}
}

#endif /* POINTCLOUD_MISCELLANEOUS_H_ */