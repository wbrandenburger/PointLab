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

#ifndef POINTCLOUD_POINTCLOUDAOS_H_
#define POINTCLOUD_POINTCLOUDAOS_H_

#include <stdint.h>

#include "tools/utils/matrix.h"

#include "tools/pointcloud/pointcloud.h"

namespace pointcloud
{
	/**
		Forward declaration of class PointcloudAoS
	*/
	template<typename ElementType> class PointcloudSoA;

	template<typename ElementType> class PointcloudAoS : public Pointcloud<ElementType>
	{
	
	public:

		typedef PointcloudAoS<ElementType> ClassType;

		/**
			Constructor
		*/
		PointcloudAoS() : Pointcloud(), pointcloud(nullptr)
		{
		}

		/**
			Constructor

			@param[in] flags_ The flags determine which fields has to be set
		*/
		PointcloudAoS (uint8_t flags_) : 
			PointcloudAoS()
		{
			setFlags(flags_);
		}

		/**
			Constructor

			@param[in] number_of_vertices_ Number of vertices
			@param[in] flags_ The flags determine which fields has to be set
		*/
		PointcloudAoS(size_t number_of_vertices_, uint8_t flags_) : 
			PointcloudAoS()
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
		PointcloudAoS(size_t number_of_vertices_, size_t number_of_triangles_, uint8_t flags_) :
			 PointcloudAoS()
		{
			setFlags(flags_);
			setNumberOfElements(number_of_vertices_, number_of_triangles_);
		}
		
		/**
			Deconstructor
		*/
		~PointcloudAoS()
		{
			clearMemory();
		}

		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudAoS(const PointcloudAoS<ElementType>& pointcloud_) :
			PointcloudAoS()
		{
			copyPointcloudAoS(pointcloud_);
		}

		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudAoS(const PointcloudSoA<ElementType>& pointcloud_) :
			PointcloudAoS()
		{
			copyPointcloudSoA(pointcloud_);
		}
		
		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudAoS(const PointcloudAoS<ElementType>&& pointcloud_) = delete;

		/**
			Operator =

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudAoS& operator=(const PointcloudAoS<ElementType>& pointcloud_) 
		{
			clearMemory();

			copyPointcloud(pointcloud_);

			return (*this);
		}

		/**
			Operator =

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudAoS& operator=(const PointcloudSoA<ElementType>& pointcloud_) 
		{
			clearMemory();

			copyPointcloudSoA(pointcloud_);

			return (*this);
		}

		/**
			Operator =

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudAoS& operator=(const PointcloudAoS<ElementType>&& pointcloud_) = delete;

	private:
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

			pointcloud = pointcloud_.getPointcloud();
			if (isTriangle()) { triangles = pointcloud_.getTrianglesPtr(); }
		}

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

			allocateMemoryPointcloud();

			PointcloudSoA<ElementType>::Iterator<ElementType> it_copy = pointcloud_.beginPoint();
			PointcloudAoS<ElementType>::Iterator<ElementType> it_new = beginPoint();
			while (it_copy != pointcloud_.endPoint()) {
				*it_new = *it_copy;
				it_new++;
				it_copy++;
			}
			if (isColor()) { 
				PointcloudSoA<ElementType>::Iterator<uint8_t> it_copy = pointcloud_.beginColor();
				PointcloudAoS<ElementType>::Iterator<uint8_t> it_new = beginColor();
				while (it_copy != pointcloud_.endColor()) {
					*it_new = *it_copy;
					it_new++;
					it_copy++;
				}
			}
			if (isNormal()) { 
				PointcloudSoA<ElementType>::Iterator<ElementType> it_copy = pointcloud_.beginNormal();
				PointcloudAoS<ElementType>::Iterator<ElementType> it_new = beginNormal();
				while (it_copy != pointcloud_.endNormal()) {
					*it_new = *it_copy;
					it_new++;
					it_copy++;
				}
			}

			if (isTriangle()) { triangles = pointcloud_.getTrianglesPtr(); }
		}

	
		/**
			Allocate the memory for the pointcloud
		*/
		void allocateMemoryPointcloud()
		{
			pointcloud = new PointcloudNode<ElementType>[number_of_vertices];
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
		}

		/**
			Set normal flag and allocate memory
		*/
		void setNormals()
		{
			normal_flag = true;
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
			if (pointcloud) {
				delete[] pointcloud;
				pointcloud = nullptr;
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
			pointcloud[row_].setPointPtr(point_);
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
		*/
		void setNormalPtr(ElementType* normal_, size_t row_)
		{
			pointcloud[row_].setNormalPtr(normal_);
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
		*/
		void setColorPtr(uint8_t* color_, size_t row_)
		{
			pointcloud[row_].setColorPtr(color_);
		}

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setPoint(ElementType point_, size_t row_, size_t col_)
		{
			pointcloud[row_].setPoint(point_, col_);
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setNormal(ElementType normal_, size_t row_, size_t col_)
		{
			pointcloud[row_].setNormal(normal_, col_);
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setColor(uint8_t color_, size_t row_, size_t col_)
		{
			pointcloud[row_].setColor(color_, col_);
		}

	private:
		/** 
			Get pointcloud

			@return Pointer to the pointcloud
		*/
		PointcloudNode<ElementType>* getPointcloud()
		{
			PointcloudNode<ElementType>* new_pointcloud = new PointcloudNode<ElementType>[number_of_vertices];
			std::memcpy(new_pointcloud, pointcloud, sizeof(PointcloudNode<ElementType>) * number_of_vertices);
		}

	public:

		/**
			Get point data of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return point data of specified index
		*/
		ElementType getPoint(size_t row_, size_t col_) const
		{
			return pointcloud[row_].getPoint(col_);
		}

		/**
			Get normal information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return normal information of specified index
		*/
		ElementType getNormal(size_t row_, size_t col_) const
		{
			return pointcloud[row_].getNormal(col_);
		}

		/**
			Get color information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return color information of specified index
		*/
		uint8_t getColor(size_t row_, size_t col_) const
		{
			return pointcloud[row_].getColor(col_);
		}

		/**
			Get Pointer to point data

			@param[in] row_ Row
			@return Return pointer to point data
		*/
		ElementType* getPointPtr(size_t row_) const
		{
			return pointcloud[row_].getPointPtr();
		}

		/**
			Get Pointer to the normal

			@param[in] row_ Row
			@return Return pointer to the normal
		*/
		ElementType* getNormalPtr(size_t row_) const
		{
			return pointcloud[row_].getNormalPtr();
		}

		/**
			Get Pointer to color information

			@param[in] row_ Row
			@return Return pointer to color information
		*/		
		uint8_t* getColorPtr(size_t row_) const
		{
			return pointcloud[row_].getColorPtr();
		}

		/**
			Get Matrix to points

			@param[in] matrix_ Matrix which shall contain the points
		*/
		void getMatrix(utils::Matrix<ElementType>& matrix_) const
		{
			ElementType* data = new ElementType[number_of_vertices*3];
			
			size_t index = 0;
			for (Iterator<ElementType> it = beginPoint(); it != endPoint(); it++) {
				data[index++] = *it;
			}

			matrix_.setMatrix(data, number_of_vertices, 3);
		}

		/**
			Returns a pointer to the first entry of the points

			@return Pointer to the first entry of the points
		*/
		IteratorInitializer<ElementType> beginPoint() const
		{
			return IteratorInitializer<ElementType>(
				reinterpret_cast<ElementType*>((char*)&pointcloud[0] + 0), 
				PointcloudFlag::POINTS, PointcloudType::AOS);
		}

		/**
			Returns a pointer to the first entry of colors

			@return Pointer to the first entry of colors
		*/
		IteratorInitializer<uint8_t> beginColor() const
		{
			return IteratorInitializer<uint8_t>(
				reinterpret_cast<uint8_t*>((char*)&pointcloud[0] + sizeof(ElementType) * 3),
				PointcloudFlag::RGB, PointcloudType::AOS);
		}
				
		/**
			Returns a pointer to the first entry of normals

			@return Pointer to the first entry of normals
		*/
		IteratorInitializer<ElementType> beginNormal() const
		{
			return IteratorInitializer<ElementType>(
				reinterpret_cast<ElementType*>((char*)&pointcloud[0] + sizeof(ElementType) * 3 + 4),
				PointcloudFlag::NORMALS, PointcloudType::AOS);
		}

		/**
			Returns a pointer to the last entry + 1 of the points

			@return Pointer to the last entry + 1 of the points
		*/
		ElementType* endPoint() const
		{
			return reinterpret_cast<ElementType*>((char*)&pointcloud[getNumberOfVertices() - 1]  + sizeof(PointcloudNode<ElementType>));
		}
		/**
			Returns a pointer to the last entry + 1 of colors

			@return Pointer to the last entry + 1 of colors
		*/
		uint8_t* endColor() const
		{
			return reinterpret_cast<uint8_t*>((char*)&pointcloud[getNumberOfVertices() - 1]  + sizeof(PointcloudNode<ElementType>) + sizeof(ElementType) * 3 );
		}
		/**
			Returns a pointer to the last entry + 1 of normals

			@return Pointer to the last entry + 1 of normals
		*/
		ElementType* endNormal() const
		{
			return reinterpret_cast<ElementType*>((char*)&pointcloud[getNumberOfVertices() - 1] + sizeof(PointcloudNode<ElementType>) + sizeof(ElementType) * 3 + 4);
		}

		private:

		/**
			Pointcloud
		*/
		PointcloudNode<ElementType>* pointcloud;

	};
}

#endif /* POINTCLOUD_POINTCLOUDAOS_H_ */