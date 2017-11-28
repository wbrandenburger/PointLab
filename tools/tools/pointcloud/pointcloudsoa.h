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

#ifndef POINTCLOUD_POINTCLOUDSOA_H_
#define POINTCLOUD_POINTCLOUDSOA_H_

#include <stdint.h>
#include <initializer_list>

#include "tools/pointcloud/pointcloud.h"

namespace pointcloud
{
	/**
		Forward declaration of class PointcloudAoS
	*/
	template<typename ElementType> class PointcloudAoS;

	template<typename ElementType> class PointcloudSoA : public Pointcloud<ElementType>
	{
	
	public:

		typedef PointcloudAoS<ElementType> ClassType;

		/**
			Constructor
		*/
		PointcloudSoA() : Pointcloud(), points(nullptr), colors(nullptr), normals(nullptr)
		{
		}

		/**
			Constructor

			@param[in] flags_ The flags determine which fields has to be set
		*/
		PointcloudSoA (uint8_t flags_) : 
			PointcloudSoA()
		{
			setFlags(flags_);
		}

		/**
			Constructor

			@param[in] number_of_vertices_ Number of vertices
			@param[in] flags_ The flags determine which fields has to be set
		*/
		PointcloudSoA(size_t number_of_vertices_, uint8_t flags_) : 
			PointcloudSoA()
		{
			setFlags(flags_);
			setNumberOfVertices(number_of_vertices_);
		}

		/**
			Constructor

			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_triangles_ Number of triangles
			@param[in] flags_ The flags determine which fields has to be set
		*/
		PointcloudSoA(size_t number_of_vertices_, size_t number_of_triangles_, uint8_t flags_) :
			 PointcloudSoA()
		{
			setFlags(flags_);
			setNumberOfElements(number_of_vertices_, number_of_triangles_);
		}

		/**
			Deconstructor
		*/
		~PointcloudSoA()
		{
			clearMemory();
		}

		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA(const PointcloudSoA<ElementType>& pointcloud_) :
			PointcloudSoA()
		{
			copyPointcloudSoA(pointcloud_);
		}
			
		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA(const PointcloudAoS<ElementType>& pointcloud_) :
			PointcloudSoA()
		{
			copyPointcloudAoS(pointcloud_);
		}	

		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA(const PointcloudSoA<ElementType>&& pointcloud_) = delete;

		/**
			Operator =

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA& operator=(const PointcloudSoA<ElementType>& pointcloud_) 
		{
			clearMemory();

			copyPointcloudSoA(pointcloud_);

			return (*this);
		}

		/**
			Operator =

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA& operator=(const PointcloudAoS<ElementType>& pointcloud_) 
		{
			clearMemory();

			copyPointcloudAoS(pointcloud_);

			return (*this);
		}	

		/**
			Operator =

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA& operator=(const PointcloudSoA<ElementType>&& pointcloud_) = delete;

	private:
		/**
			Copy the pointcloud

			@param[in] pointcloud_ Pointcloud
		*/
		void copyPointcloudSoA(const PointcloudSoA<ElementType>& pointcloud_)
		{
			setNumberOfVertices(pointcloud_.getNumberOfVertices());
			setNumberOfTriangles(pointcloud_.getNumberOfTriangles());

			if (pointcloud_.isColor()) { setColorFlag(); }
			if (pointcloud_.isNormal()) { setNormalFlag(); }
			if (pointcloud_.isTriangle()) { setTriangleFlag(); }

			points = pointcloud_.getPointsPtr();
			if (isColor()) { colors = pointcloud_.getColorsPtrsPtr(); }
			if (isNormal()) { normals = pointcloud_.getNormalsPtr(); }
			if (isTriangle()) { triangle = pointcloud_.getTrianglesPtr(); }
		}

		/**
			Copy the pointcloud

			@param[in] pointcloud_ Pointcloud
		*/
		void copyPointcloudAoS(const PointcloudAoS<ElementType>& pointcloud_)
		{
			setNumberOfVertices(pointcloud_.getNumberOfVertices());
			setNumberOfTriangles(pointcloud_.getNumberOfTriangles());

			if (pointcloud_.isColor()) { setColorFlag(); }
			if (pointcloud_.isNormal()) { setNormalFlag(); }
			if (pointcloud_.isTriangle()) { setTriangleFlag(); }

			points = pointcloud_.getPointsPtr();
			if (isColor()) { colors = pointcloud_.getColorsPtrsPtr(); }
			if (isNormal()) { normals = pointcloud_.getNormalsPtr(); }
			if (isTriangle()) { triangle = pointcloud_.getTrianglesPtr(); }
		}


		/**
			Allocate the memory for the pointcloud
		*/
		void allocateMemoryPointcloud()
		{
			points = new ElementType[number_of_vertices * 3];
			memset(points, (ElementType) 0.0, sizeof(ElementType) * number_of_vertices * 3);
			if (isColor()) {
				colors = new uint8_t[number_of_vertices * 3];
				memset(colors, (uint8_t) 0, sizeof(uint8_t) * number_of_vertices * 3);
			}
			if (isNormal()) {
				normals = new ElementType[number_of_vertices * 3];
				memset(normals, (ElementType) 0.0, sizeof(ElementType) * number_of_vertices * 3);
			}
		}

	public:
		/**
			Set pointcloud
		*/
		void setPointcloud()
		{
			clearMemory();

			allocateMemoryPointcloud();
			allocateMemoryTriangles();
		}

		/**
			Set color flag and allocate memory
		*/
		void setColors()
		{
			color_flag = true;

			if (colors) {
				delete[] colors;
				colors = nullptr;
			}

			colors = new uint8_t[number_of_vertices * 3];
			memset(colors, (uint8_t)0, sizeof(uint8_t) * number_of_vertices * 3);
		}

		/**
			Set normal flag and allocate memory
		*/
		void setNormals()
		{
			normal_flag = true;

			if (normals) {
				delete[] normals;
				normals = nullptr;
			}

			normals = new ElementType[number_of_vertices * 3];
			memset(normals, (ElementType) 0.0, sizeof(ElementType) * number_of_vertices * 3);
		}

	private:
		/**
			Clear memory of the pointcloud and triangles
		*/
		void clearMemory()
		{
			clearMemoryPointcloud();
			clearMemoryTriangles();
		}

		/**
			Clear memory of pointcloud
		*/
		void clearMemoryPointcloud()
		{
			if (points) { 
				delete[] points;
				points = nullptr;
			}
			if (normals) { 
				delete[] normals; 
				normals = nullptr;
			}
			if (colors) { 
				delete[] colors;
				colors = nullptr;
			}
		}

	public:	
		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
		*/
		void setPointPtr(ElementType* point_, size_t row_)
		{
			std::memcpy(&points[row_ * 3], point_, sizeof(ElementType) * 3);
		}

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
		*/
		void setPointPtr(std::initializer_list<ElementType> point_, size_t row_)
		{
			std::memcpy(&points[row_ * 3], point_.begin(), sizeof(ElementType) * 3);
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
		*/
		void setNormalPtr(ElementType* normal_, size_t row_)
		{
			std::memcpy(&normals[row_ * 3], normal_, sizeof(ElementType) * 3);
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
		*/
		void setNormalPtr(std::initializer_list<ElementType> normal_, size_t row_)
		{
			std::memcpy(&normals[row_ * 3], normal_.begin(), sizeof(ElementType) * 3);
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
		*/
		void setColorPtr(uint8_t* color_, size_t row_)
		{
			std::memcpy(&colors[row_ * 3], color_, sizeof(uint8_t) * 3);
		}
	
		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
		*/
		void setColorPtr(std::initializer_list<uint8_t> color_, size_t row_)
		{
			std::memcpy(&colors[row_ * 3], color_.begin(), sizeof(uint8_t) * 3);
		}

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setPoint(ElementType point_, size_t row_, size_t col_)
		{
			points[row_ * 3 + col_] = point_;
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setNormal(ElementType normal_, size_t row_, size_t col_)
		{
			normals[row_ * 3 + col_] = normal_;
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setColor(uint8_t color_, size_t row_, size_t col_)
		{
			colors[row_ * 3 + col_] = color_;
		}

		/**
			Get point data of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return point data of specified index
		*/
		ElementType getPoint(size_t row_, size_t col_) const
		{
			return points[row_ * 3 + col_];
		}

		/**
			Get normal information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return normal information of specified index
		*/
		ElementType getNormal(size_t row_, size_t col_) const
		{
			return normals[row_ * 3 + col_];
		}

		/**
			Get color information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return color information of specified index
		*/
		uint8_t getColor(size_t row_, size_t col_) const
		{
			return colors[row_ * 3 + col_];
		}

		/**
			Get Pointer to point data

			@param[in] row_ Row
			@return Return pointer to point data
		*/
		ElementType* getPointPtr(size_t row_) const
		{
			return &points[row_ * 3];
		}

		/**
			Get Pointer to point data

			@param[in] row_ Row
			@return Return pointer to point data
		*/
		ElementType* getAllocatedPointPtr(size_t row_) const
		{
			ElementType* point = new ElementType[3];
			std::memcpy(point, &points[row_ * 3], sizeof(ElementType) * 3);
			
			return point;
		}

		/**
			Get Pointer to the normal

			@param[in] row_ Row
			@return Return pointer to the normal
		*/
		ElementType* getNormalPtr(size_t row_) const
		{
			return &normals[row_ * 3];
		}

		/**
			Get Pointer to the normal

			@param[in] row_ Row
			@return Return pointer to the normal
		*/
		ElementType* getAllocatedNormalPtr(size_t row_) const
		{
			ElementType* normal = new ElementType[3];
			std::memcpy(normal, &normals[row_ * 3], sizeof(ElementType) * 3);

			return normal;
		}

		/**
			Get Pointer to color information

			@param[in] row_ Row
			@return Return pointer to color information
		*/		
		uint8_t* getColorPtr(size_t row_) const
		{
			return &colors[row_ * 3];
		}

		/**
			Get Pointer to color information

			@param[in] row_ Row
			@return Return pointer to color information
		*/		
		uint8_t* getAllocatedColorPtr(size_t row_) const
		{
			uint8_t* color = new uint8_t[3];
			std::memcpy(color, &colors[row_ * 3], sizeof(uint8_t) * 3);

			return color;
		}

		/**
			Returns a pointer to the first entry of the points

			@return Pointer to the first entry of the points
		*/
		IteratorInitializer<ElementType> beginPoint() const
		{
			return IteratorInitializer<ElementType>(points, PointcloudFlag::POINTS, PointcloudType::SOA);
		};

		/**
			Returns a pointer to the first entry of colors

			@return Pointer to the first entry of colors
		*/
		IteratorInitializer<uint8_t> beginColor() const
		{
			return IteratorInitializer<uint8_t>(colors, PointcloudFlag::RGB, PointcloudType::SOA);;
		};
				
		/**
			Returns a pointer to the first entry of normals

			@return Pointer to the first entry of normals
		*/
		IteratorInitializer<ElementType> beginNormal() const
		{
			return IteratorInitializer<ElementType>(normals, PointcloudFlag::NORMALS, PointcloudType::SOA);
		};

		/**
			Returns a pointer to the last entry + 1 of the points

			@return Pointer to the last entry + 1 of the points
		*/
		ElementType* endPoint() const
		{
			return points + number_of_vertices * 3;
		}
		/**
			Returns a pointer to the last entry + 1 of colors

			@return Pointer to the last entry + 1 of colors
		*/
		uint8_t* endColor() const
		{
			return colors + number_of_vertices * 3;
		}
		/**
			Returns a pointer to the last entry + 1 of normals

			@return Pointer to the last entry + 1 of normals
		*/
		ElementType* endNormal() const
		{
			return normals + number_of_vertices * 3;
		}

	private:
		/**
			Pointcloud
		*/
		ElementType* points;

		/**
			Colors
		*/
		uint8_t* colors;

		/**
			Normals
		*/
		ElementType* normals;
	};
}

#endif /* POINTCLOUD_POINTCLOUDSOA_H_ */