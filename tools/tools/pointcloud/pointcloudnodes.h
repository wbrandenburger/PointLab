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

#ifndef POINTCLOUD_POINTCLOUDNODE_H_
#define POINTCLOUD_POINTCLOUDNODE_H_

#include <stdint.h>

#include "tools/utils/matrix.h"

namespace pointcloud
{		
	template<typename ElementType> struct PointcloudNode 
	{

		/**
			Constructor
		*/
		PointcloudNode()
		{
			clear();
		}

		/**
			Constructor

			@param[in] point_ Point
		*/
		PointcloudNode(ElementType* point_) 
		{
			for (size_t i = 0; i < 3; i++) {
				point[i] = point_[i];
			}
		}

		/**
			Constructor
			
			@param[in] point_ Point
			@param[in] normal_ Normal
			@param[in] color_ Color
		*/
		PointcloudNode(ElementType* point_, ElementType* normal_, uint8_t* color_)
		{
			setPoint(point_);
			setNormal(normal_);
			setColor(color_);
		}

		/**
			Set point
			
			@param[in] point_ Point
		*/
		void setPointPtr(ElementType* point_)
		{
			for (size_t i = 0; i < 3; i++) {
				point[i] = point_[i];
			}
		}

		/**
			Set normal

			@param[in] normal_ Normal
		*/
		void setNormalPtr(ElementType* normal_)
		{
			for (size_t i = 0; i < 3; i++) {
				normal[i] = normal_[i];
			}
		}
		
		/**
			Set color

			@param[in] color_ Color
		*/
		void setColorPtr(uint8_t* color_)
		{
			for (size_t i = 0; i < 3; i++) {
				color[i] = color_[i];
			}
		}

		/**
			Set point
			
			@param[in] point_ Point
			@parma[in] index_ Index
		*/
		void setPoint(ElementType point_, size_t index_)
		{
			point[index_] = point_;
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@parma[in] index_ Index
		*/
		void setNormal(ElementType normal_, size_t index_)
		{
			normal[index_] = normal_;
		}
		
		/**
			Set color

			@param[in] color_ Color
			@parma[in] index_ Index
		*/
		void setColor(uint8_t color_, size_t index_)
		{
			color[index_] = color_;
		}
		/**
			Clear
		*/
		void clear()
		{
			for (size_t i = 0; i < 3; i++){
				point[i] = NULL;
				normal[i] = NULL;
				color[i] = NULL;
			}
		}

		/**
			Operator [] Access on point information

			@param[in] index_ Dimension
			@return Return data element in this dimension
		*/
		ElementType operator[](size_t index_)
		{
			return point[index_];
		}

		/**
			Get point data of specified index

			@param[in] index_ Dimension
			@return Return point data of specified index
		*/
		ElementType getPoint(size_t index_)
		{
			return point[index_];
		}

		/**
			Get normal information of specified index

			@param[in] index_ Dimension
			@return Return normal information of specified index
		*/
		ElementType getNormal(size_t index_)
		{
			return normal[index_];
		}
	
		/**
			Get color information of specified index

			@param[in] index_ Dimension
			@return Return color information of specified index
		*/
		uint8_t getColor(size_t index_)
		{
			return color[index_];
		}

		/**
			Get pointer to point data

			@return Return pointer to point data
		*/
		ElementType* getPointPtr()
		{
			return &point[0];
		}

		/**
			Get pointer to the normal

			@return Return pointer to the normal
		*/
		ElementType* getNormalPtr()
		{
			return &normal[0];
		}
	
		/**
			Get pointer to color information

			@return Return pointer to color information
		*/
		uint8_t* getColorPtr()
		{
			return &color[0];
		}
		/**
			Point
		*/
		ElementType point[3];

		/**
			Normal
		*/
		ElementType normal[3];

		/**
			Color
		*/
		uint8_t color[3];
	};
}

#endif /* POINTCLOUD_POINTCLOUDNODE_H_ */