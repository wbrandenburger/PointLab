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
	template<typename ElementType> class PointNode 
	{
	public:
		/**
			Constructor
		*/
		PointNode() : point_(nullptr)
		{
			point_ = new ElementType[3];
			memset(point_, (ElementType) 0.0, sizeof(ElementType)*3);
		}

		/**
			Destructor
		*/
		~PointNode()
		{
			clearMemory();
		}

		/**
			Clear memory
		*/
		void clearMemory()
		{
			if (point_) {
				delete[] point_;
				point_ = nullptr;
			}
		}

		/**
			Constructor
			
			@param[in] point Point
		*/
		PointNode(ElementType* point) : PointNode()
		{
			setPointPtr(point);
		}

		/**
			Constructor
			
			@param[in] x x-value
			@param[in] y y-value
			@param[in] z z-value
		*/
		PointNode(ElementType x, ElementType y, ElementType z) : PointNode()
		{
			point_[0] = x;
			point_[1] = y;
			point_[2] = z;
		}

		/**
			Copy constructor

			@param[in] point_node An instance of class PointNode
		*/
		PointNode(const PointNode<ElementType>& point_node) : PointNode()
		{
			clearMemory();

			setPointPtr(point_node.getPointPtr());

		}

		/**
			Copy constructor
	
			@param[in] point_node An instance of class PointNode
		*/
		PointNode(const PointNode<ElementType>&& point_node) : PointNode()
		{
			clearMemory();

			*this = point_node;

		}
	
		/**
			Operator =
	
			@param[in] point_node An instance of class PointNode
			@return Returns reference to the current instance
		*/
		PointNode& operator=(const PointNode<ElementType>& point_node)
		{
			clearMemory();

			setPointPtr(point_node.getPointPtr());

			return *this;
		}
	
		/**
			Operator =
		
			@param[in] point_node An instance of class PointNode
			@return Returns reference to the current instance
		*/
		PointNode& operator=(const PointNode<ElementType>&& point_node)
		{
			clearMemory();

			*this = point_node;

			return *this;
		}
		
		/**
			Set point
			
			@param[in] point_ Point
		*/
		void setPointPtr(ElementType* point)
		{
			clearMemory();

			point_ = point;
		}

		/**
			Set point
			
			@param[in] point_ Point
			@parma[in] index_ Index
		*/
		void setPoint(ElementType point, size_t index)
		{
			point_[index] = point;
		}

		/**
			Set point
			
			@param[in] x_ x-value
			@param[in] y_ y-value
			@param[in] z_ z-value
		*/
		void setPoint(ElementType x, ElementType y, ElementType z)
		{
			point_[0] = x;
			point_[1] = y;
			point_[2] = z;
		}

		/**
			Get point data of specified index

			@param[in] index_ Dimension
			@return Return point data of specified index
		*/
		ElementType getPoint(size_t index) const
		{
			return point_[index];
		}

		/**
			Get pointer to point data

			@return Return pointer to point data
		*/
		ElementType* getPointPtr() const
		{
			ElementType* point_new = new ElementType[3];
			std::memcpy(point_new, point_, sizeof(ElementType) * 3);
			
			return point_new;
		}

	protected: 
		/**
			Point
		*/
		ElementType* point_;
	};

	class RGBNode : public PointNode<uint8_t>
	{
	public:
		/**
			Constructor
		*/
		RGBNode() : PointNode()
		{
		}

		/**
			Destructor
		*/
		~RGBNode()
		{
			clearMemory();
		}

		/**
			Constructor
			
			@param[in] point Point
		*/
		RGBNode(uint8_t* point) : RGBNode()
		{
			clearMemory();

			setPointPtr(point);
		}

		/**
			Constructor
			
			@param[in] point Point
		*/
		RGBNode(float* point) : RGBNode()
		{
			clearMemory();

			uint8_t* point_new = new uint8_t[3];
			for (size_t i = 0; i < 3; i++){
				point_new[i] = (uint8_t) std::floor(point[i] * 255.0f);
				
			}
			setPointPtr(point_new);
		}

		/**
			Constructor
			
			@param[in] x x-value
			@param[in] y y-value
			@param[in] z z-value
		*/
		RGBNode(uint8_t x, uint8_t y, uint8_t z) : RGBNode()
		{
			point_[0] = x;
			point_[1] = y;
			point_[2] = z;
		}

		/**
			Constructor
			
			@param[in] x x-value
			@param[in] y y-value
			@param[in] z z-value
		*/
		RGBNode(float x, float y, float z) : RGBNode()
		{
			point_[0] = (uint8_t) std::floor(x * 255.0f);
			point_[1] = (uint8_t) std::floor(y * 255.0f);
			point_[2] = (uint8_t) std::floor(z * 255.0f);
		}

		/**
			Copy constructor

			@param[in] point_node An instance of class RGBNode
		*/
		RGBNode(const RGBNode& rgb_node) : RGBNode()
		{
			clearMemory();

			setPointPtr(rgb_node.getPointPtr());
		}

		/**
			Copy constructor
	
			@param[in] point_node An instance of class PointNode
		*/
		RGBNode(const RGBNode&& rgb_node) : RGBNode()
		{
			clearMemory();

			*this = rgb_node;
		}
	
		/**
			Operator =
	
			@param[in] point_node An instance of class RGBNode
			@return Returns reference to the current instance
		*/
		RGBNode& operator=(const RGBNode& rgb_node)
		{
			clearMemory();

			setPointPtr(rgb_node.getPointPtr());
			
			return *this;
		}
	
		/**
			Operator =
		
			@param[in] point_node An instance of class RGBNode
			@return Returns reference to the current instance
		*/
		RGBNode& operator=(const RGBNode&& rgb_node)
		{
			clearMemory();

			*this = rgb_node;
			
			return *this;
		}

		/**
			Set point
			
			@param[in] point_ Point
			@parma[in] index_ Index
		*/
		void setPoint(float point, size_t index)
		{
			point_[index] = (uint8_t) std::floor(point * 255.0f);;
		}

		/**
			Set point
			
			@param[in] x_ x-value
			@param[in] y_ y-value
			@param[in] z_ z-value
		*/
		void setPoint(float x, float y, float z)
		{
			point_[0] = (uint8_t)std::floor(x * 255.0f);
			point_[1] = (uint8_t)std::floor(y * 255.0f);
			point_[2] = (uint8_t)std::floor(z * 255.0f);
		}

	private:
		/**
			Padding
		*/
		uint8_t padding;
	};

	template<typename ElementType> struct PointcloudNode 
	{

		/**
			Constructor
		*/
		PointcloudNode() 
		{
			memset(&point, (ElementType) 0.0, sizeof(ElementType)*3);
			memset(&color, (uint8_t) 0, sizeof(uint8_t)*4);
			memset(&normal, (ElementType) 0.0, sizeof(ElementType)*3);
		}

		/**
			Constructor
			
			@param[in] point_ Point
			@param[in] normal_ Normal
			@param[in] color_ Color
		*/
		PointcloudNode(ElementType* point_, uint8_t* color_, ElementType* normal_)
		{
			setPoint(point_);
			setColor(color_);
			setNormal(normal_);
		}

		/**
			Set point
			
			@param[in] point_ Point
		*/
		void setPointPtr(ElementType* point_)
		{
			std::memcpy(&point[0], point_, sizeof(ElementType) * 3);
		}
		
		/**
			Set color

			@param[in] color_ Color
		*/
		void setColorPtr(uint8_t* color_)
		{
			std::memcpy(&color[0], color_, sizeof(uint8_t) * 3);
		}

		/**
			Set normal

			@param[in] normal_ Normal
		*/
		void setNormalPtr(ElementType* normal_)
		{
			std::memcpy(&normal[0], normal_, sizeof(ElementType) * 3);
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
			Set color

			@param[in] color_ Color
			@parma[in] index_ Index
		*/
		void setColor(uint8_t color_, size_t index_)
		{
			color[index_] = color_;
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
			Get color information of specified index

			@param[in] index_ Dimension
			@return Return color information of specified index
		*/
		uint8_t getColor(size_t index_)
		{
			return color[index_];
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
			Get pointer to point data

			@return Return pointer to point data
		*/
		ElementType* getPointPtr()
		{
			return &point[0];
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
			Get pointer to the normal

			@return Return pointer to the normal
		*/
		ElementType* getNormalPtr()
		{
			return &normal[0];
		}

		/**
			Point
		*/
		ElementType point[3];

		/**
			Color
		*/
		uint8_t color[4];

		/**
			Normal
		*/
		ElementType normal[3];
	};
}

#endif /* POINTCLOUD_POINTCLOUDNODE_H_ */