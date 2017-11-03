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
	///**
	//	Forward declaration of class Pointcloud
	//*/
	//template<typename ElementType> class Pointcloud;

	/**
		Forward declaration of class PointcloudAoS
	*/
	template<typename ElementType> class PointcloudSoA;

	template<typename ElementType> class PointcloudAoS : public Pointcloud<ElementType>
	{
	
	public:

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
			Set pointcloud
			
			@param[in] number_of_vertices_ Number of vertices
		*/
		void setPointcloud(size_t number_of_vertices_)
		{
			setNumberOfVertices(number_of_vertices_);

			clearMemory();

			allocateMemoryPointcloud();
			allocateMemoryTriangles();
		}

		/**
			Set pointcloud
			
			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_triangles Number of triangles
		*/
		void setPointcloud(size_t number_of_vertices_, size_t number_of_triangles_)
		{
			setNumberOfElements(number_of_vertices_, number_of_triangles);

			clearMemory();

			allocateMemoryPointcloud();
			allocateMemoryTriangles();
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
			Set points

			@param[in] points_ Points
		*/
		void setPointsPtr(ElementType* points_)
		{
			for (size_t i = 0; i < number_of_vertices; i++) {
				pointcloud[i].setPointPtr(&points_[i*3]);
			}
		}

		/**
			Set normals

			@param[in] normals_ Normals
		*/
		void setNormalsPtr(ElementType* normals_)
		{
			for (size_t i = 0; i < number_of_vertices; i++) {
				pointcloud[i].setNormalPtr(&normals_[i*3]);
			}
		}

		/**
			Set normals

			@param[in] colors_ Colors
		*/
		void setColorsPtr(uint8_t* colors_)
		{
			for (size_t i = 0; i < number_of_vertices; i++) {
				pointcloud[i].setColorPtr(&colors_[i*3]);
			}
		}

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
			Operator [] Access on point information

			@param[in] index_ Dimension
			@return Return pointer to point data
		*/
		ElementType* operator[](size_t index_) const
		{
			return pointcloud[index_].getPointPtr();
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
			Get Pointer to point data

			@return Return pointer to point data
		*/
		ElementType* getPointsPtr() const
		{
			ElementType* new_points = new ElementType[number_of_vertices*3];

			size_t index = 0;
			for (PointcloudAoS<ElementType>::Iterator<ElementType> it = beginPoint();
				it != endPoint(); it++) {
				new_points[index++] = *it;
			}

			return new_points;
		}

		/**
			Get Pointer to the normal

			@return Return pointer to the normal
		*/
		ElementType* getNormalsPtr() const
		{
			ElementType* new_normals = new ElementType[number_of_vertices*3];

			size_t index = 0;
			for (PointcloudAoS<ElementType>::Iterator<ElementType> it = beginNormal();
				it != endNormal(); it++) {
				new_normals[index++] = *it;
			}

			return new_normals;
		}

		/**
			Get Pointer to color information

			@return Return pointer to color information
		*/		
		uint8_t* getColorsPtr() const
		{
			uint8_t* new_colors = new uint8_t[number_of_vertices*3];

			size_t index = 0;
			for (PointcloudAoS<ElementType>::Iterator<uint8_t> it = beginColor();
				it != endColor(); it++) {
				new_colors[index++] = *it;
			}

			return new_colors;
		}
				
		/**
			Get Matrix to points

			@param[in] matrix_ Matrix which shall contain the points
		*/
		void getMatrix(utils::Matrix<ElementType>& matrix_) const
		{
			ElementType* data = new ElementType[number_of_vertices*3];
			
			size_t index = 0;
			for (PointcloudAoS<ElementType>::Iterator<ElementType> it = beginPoint();
				it != endPoint(); it++) {
				data[index++] = *it;
			}

			matrix_.setMatrix(data, number_of_vertices, 3);
		}

		/**
			Get stride for triangles

			return Stride for triangles
		*/
		size_t getStrideTriangle() const
		{
			return 0;
		}

		/**
			Returns a pointer to the first entry of the points

			@return Pointer to the first entry of the points
		*/
		ElementType* beginPoint() const
		{
			return reinterpret_cast<ElementType*>((char*)&pointcloud[0] + 0);
		}

		/**
			Returns a pointer to the first entry of colors

			@return Pointer to the first entry of colors
		*/
		uint8_t* beginColor() const
		{
			return reinterpret_cast<uint8_t*>((char*)&pointcloud[0] + sizeof(ElementType) * 3);
		}
				
		/**
			Returns a pointer to the first entry of normals

			@return Pointer to the first entry of normals
		*/
		ElementType* beginNormal() const
		{
			return reinterpret_cast<ElementType*>((char*)&pointcloud[0] + sizeof(ElementType) * 3 + 4);
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

		/**
			Structure of a iterator for points, colors, normals and triangles
		*/
		template<typename IteratorType> class Iterator
		{
		public:
			/**
				Constructor
			*/
			Iterator() : iterator_(nullptr), stride_(0), index_(0)
			{
				if (sizeof(IteratorType) == 1) {
					stride_ = sizeof(ElementType) * 6 + 2;
				}
				else {
					stride_ = sizeof(ElementType) * 4 + 4;
				}
			}
			
			/**
				Constructor

				@param[in] begin Pointer to an element
			*/
			Iterator(IteratorType* begin) : Iterator()
			{
				iterator_ = begin;
			}

			/**
				Destructor
			*/
			~Iterator()
			{
			}
			
			/**
				Copy Constructor

				@param[in] An instance of class Iterator
			*/
			Iterator(const Iterator& iterator) = delete;

			/**
				Operator = 

				@param[in] An instance of class Iterator
			*/
			Iterator(const Iterator&& iterator) = delete;

			/**
				Operator = 

				@param[in] An instance of class Iterator
				@return Returns reference to the current instance
			*/
			Iterator& operator=(const Iterator& iterator) = delete;

			/**
				Operator = 

				@param[in] iterator An instance of class Iterator
				@return Returns reference to the current instance
			*/
			Iterator& operator=(const Iterator&& iterator) = delete;
	
			/**
				Operator ++

				@param[in] Increment
				@return Returns reference to the current instance
			*/
			Iterator& operator++(int)
			{
				if (index_ % 3 == 2){
					iterator_ = reinterpret_cast<IteratorType*>((char*)iterator_ + stride_);
					index_ = 0;
				}
				else {
					iterator_++;
					index_++;
				}

				return *this;
			}
						/**
				Operator = 

				@param[in] iterator Pointer to an element
				@return Returns reference to the current instance
			*/
			Iterator& operator=(IteratorType* iterator)
			{
				iterator_ = iterator;

				return *this;
			}

			/**
				Operator = 

				@param[in] iterator Pointer to an element
				@return Returns reference to the current instance
			*/
			bool operator!=(IteratorType* iterator)
			{
				return iterator_ != iterator;
			}

			/**
				Operator *

				@return Content of current location of iterator
			*/
			IteratorType& operator*()
			{
				return *iterator_;
			}

		private:		
			/**
				Pointer to the current element
			*/
			IteratorType* iterator_;

			/**
				Stride which defines the bytes between two points
			*/
			size_t stride_;

			/**
				Current index
			*/
			size_t index_;
		};

		/**
			Get type of derived class

			@return Type of derived class
		*/
		PointcloudIdentifier getPointcloudType()
		{
			return PointcloudIdentifier::AoS;
		}

		private:

		/**
			Pointcloud
		*/
		PointcloudNode<ElementType>* pointcloud;

	};
}

#endif /* POINTCLOUD_POINTCLOUDAOS_H_ */