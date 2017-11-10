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
	template<typename ElementType> struct XYZNode 
	{

		/**
			Constructor
		*/
		XYZNode() : point_(nullptr)
		{
			point_ = new ElementType[3];
			memset(point_, (ElementType) 0.0, sizeof(ElementType)*3);
		}

		/**
			Destructor
		*/
		~XYZNode()
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
		XYZNode(const ElementType* point) : XYZNode()
		{
			setPointPtr(point);
		}

		/**
			Constructor
			
			@param[in] x x-value
			@param[in] y y-value
			@param[in] z z-value
		*/
		XYZNode(const ElementType x, ElementType y, ElementType z) : XYZNode()
		{
			point_[0] = x;
			point_[1] = y;
			point_[2] = z;
		}

		/**
			Copy constructor

			@param[in] xyz_node An instance of class XYZNode
		*/
		XYZNode(const XYZNode<ElementType>& xyz_node) : XYZNode()
		{
			setPointPtr(xyz_node.getPointPtr());
		}

		/**
			Copy constructor
	
			@param[in] xyz_node An instance of class XYZNode
		*/
		XYZNode(const XYZNode<ElementType>&& xyz_node) : XYZNode()
		{
			*this = xyz_node;
		}
	
		/**
			Operator =
	
			@param[in] xyz_node An instance of class XYZNode
			@return Returns reference to the current instance
		*/
		XYZNode& operator=(const XYZNode<ElementType>& xyz_node)
		{
			setPointPtr(xyz_node.getPointPtr());
			
			return *this;
		}
	
		/**
			Operator =
		
			@param[in] xyz_node An instance of class XYZNode
			@return Returns reference to the current instance
		*/
		XYZNode& operator=(const XYZNode<ElementType>&& xyz_node)
		{
			*this = xyz_node;
			
			return *this;
		}
		
		/**
			Set point
			
			@param[in] point_ Point
		*/
		void setPointPtr(ElementType* point)
		{
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
			
			return point_;
		}

		/**
			Point
		*/
		ElementType* point_;
	};

	class RGBNode
	{
	public:
		/**
			Constructor
		*/
		RGBNode() : color_(nullptr)
		{
			color_ = new uint8_t[4];
			memset(&color_, 0, sizeof(uint8_t)*4);
		}

		/**
			Destructor
		*/
		~RGBNode()
		{
			clearMemory();
		}

		/**
			Clear memory
		*/
		void clearMemory()
		{
			if (color_) {
				delete[] color_;
				color_ = nullptrM
			}
		}

		/**
			Constructor
			
			@param[in] color Color
		*/
		RGBNode(uint8_t* color) : RGBNode()
		{
			setColorPtr(color);
		}

		/**
			Constructor
			
			@param[in] color Color
		*/
		RGBNode(float* color) : RGBNode()
		{
			setColorPtr(color);
		}

		/**
			Constructor
			
			@param[in] r r-value
			@param[in] g g-value
			@param[in] b b-value
		*/
		RGBNode(uint8_t r, uint8_t g, uint8_t b) : RGBNode()
		{
			color_[0] = r;
			color_[1] = g;
			color_[2] = b;
		}

		/**
			Constructor
			
			@param[in] r r-value
			@param[in] g g-value
			@param[in] b b-value
		*/
		RGBNode(float r, float g, float b) : RGBNode()
		{
			color_[0] = (uint8_t) std::floor(r * 255);
			color_[1] = (uint8_t) std::floor(g * 255);
			color_[2] = (uint8_t) std::floor(b * 255);
		}

		/**
			Copy constructor

			@param[in] rgb_node An instance of class RGBNode
		*/
		RGBNode(const RGBNode& rgb_node) : RGBNode()
		{
			setColorPtr(rgb_node.getColorPtr());
		}

		/**
			Copy constructor
	
			@param[in] rgb_node An instance of class RGBNode
		*/
		RGBNode(const RGBNode&& rgb_node) : RGBNode()
		{
			*this = rgb_node;
		}
	
		/**
			Operator =
	
			@param[in] rgb_node An instance of class RGBNode
			@return Returns reference to the current instance
		*/
		RGBNode& operator=(const RGBNode& rgb_node)
		{
			setColorPtr(rgb_node.getColorPtr());
			
			return *this;
		}
	
		/**
			Operator =
		
			@param[in] xyz_node An instance of class XYZNode
			@return Returns reference to the current instance
		*/
		RGBNode& operator=(const RGBNode&& rgb_node)
		{
			*this = rgb_node;
			
			return *this;
		}
		
		/**
			Set color
			
			@param[in] color_ Color
		*/
		void setColorPtr(uint8_t* color)
		{
			std::memcpy(&color_[0], color, sizeof(uint8_t) * 3);
		}

		/**
			Set color
			
			@param[in] color_ Color
		*/
		void setColorPtr(float* color)
		{
			for (size_t i = 0; i < 3; i++) {
				color_[i] = (uint8_t) std::floor(color[i] * 255);
			}
		}

		/**
			Set color
			
			@param[in] color_ Color
			@parma[in] index_ Index
		*/
		void setColor(uint8_t color, size_t index)
		{
			color_[index] = color;
		}

		/**
			Set color
			
			@param[in] color_ Color
			@parma[in] index_ Index
		*/
		void setColor(float color, size_t index)
		{
			color_[index] = (uint8_t) std::floor(color * 255);
		}

		/**
			Set color
			
			@param[in] r r-value
			@param[in] g g-value
			@param[in] b b-value
		*/
		void setColor(uint8_t r, uint8_t g, uint8_t b)
		{
			color_[0] = r;
			color_[1] = g;
			color_[2] = b;
		}

		/**
			Set color
			
			@param[in] r r-value
			@param[in] g g-value
			@param[in] b b-value
		*/
		void setColor(float r, float g, float b)
		{
			color_[0] = (uint8_t) std::floor(r);
			color_[1] = (uint8_t) std::floor(g);
			color_[2] = (uint8_t) std::floor(b);
		}

		/**
			Get color data of specified index

			@param[in] index_ Dimension
			@return Return color data of specified index
		*/
		uint8_t getColor(size_t index) const
		{
			return color_[index];
		}

		/**
			Get pointer to color data

			@return Return pointer to point data
		*/
		uint8_t* getColorPtr() const
		{
			uint8_t* color_new = new uint8_t[4];
			std::memcpy(color_new, color_, sizeof(color_) * 4);

			return color_new;
		}

		/**
			Get pointer to color data

			@return Return pointer to point data
		*/
		template<typename ElementType> ElementType* getColorPtr() const
		{
			uint8_t* color_new = new uint8_t[4];
			for (size_t i = 0; i < 3; i++) {
				color_new[i] = (ElementType) color[i] / (ElementType) 255.0;
			}

			return color_new;
		}

		/**
			Color
		*/
		uint8_t* color_;
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