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

#include "tools/utils/matrix.h"


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
			Set points

			@param[in] points_ Points
		*/
		void setPointsPtr(ElementType* points_)
		{
			std::memcpy(points, points_, sizeof(ElementType) * getNumberOfVertices() * 3);
		}

		/**
			Set normals

			@param[in] normals_ Normals
		*/
		void setNormalsPtr(ElementType* normals_)
		{
			std::memcpy(normals, normals_, sizeof(ElementType) * getNumberOfVertices() * 3);
		}

		/**
			Set colors

			@param[in] colors_ Colors
		*/
		void setColorsPtr(uint8_t* colors_)
		{
			std::memcpy(colors, colors_, sizeof(uint8_t) * getNumberOfVertices() * 3);
		}

		/**
			Set colors

			@param[in] colors_ Colors
		*/
		void setColorsPtr(float* colors_)
		{
			for (Iterator<uint8_t> it = beginColor(); it != endColor(); it++) {
				*it = (uint8_t) std::floor(*colors_ * 255);
				colors_++;
			}
		}	

		/**
			Set triangles

			@param[in] triangles_ Triangles
		*/
		void setTrianglesPtr(size_t* triangles_)
		{
			std::memcpy(triangles, triangles_, sizeof(size_t) * getNumberOfTriangles() * 3);
		}

		/**
			Set triangles

			@param[in] triangles_ Triangles
		*/
		void setTrianglesPtr(unsigned int* triangles_)
		{
			for (Iterator<size_t> it = beginTriangle(); it != endTriangle(); it++) {
				*it = (size_t)*triangles_;
				triangles_++;
			}
		}

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
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
		*/
		void setNormalPtr(ElementType* normal_, size_t row_)
		{
			std::memcpy(&normals[row_ * 3], normal_, sizeof(ElementType) * 3);
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
			Get Pointer to the normal

			@param[in] row_ Row
			@return Return pointer to the normal
		*/
		ElementType* getNormalPtr(size_t row_) const
		{
			return &normals[row_ * 3];
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
			Get Pointer to point data

			@return Return pointer to point data
		*/
		ElementType* getPointsPtr() const
		{
			ElementType* new_points = new ElementType[number_of_vertices * 3];
			std::memcpy(new_points, points, sizeof(ElementType) * number_of_vertices * 3);

			return new_points;
		}

		/**
			Get Pointer to point data

			@param[in] new_points Pointer to point data
		*/
		void getPointsPtr(float** new_points) const
		{
			*new_points = new float[number_of_vertices * 3];
			float* new_points_ptr = *new_points;

			for (Iterator<ElementType> it = beginPoint(); it != endPoint(); it++){
				*new_points_ptr = (float)*it;
				new_points_ptr++;
			}
		}

		/**
			Get Pointer to point data

			@param[in] new_points Pointer to point data
		*/
		void getPointsPtr(double** new_points) const
		{
			*new_points = new double[number_of_vertices * 3];
			double* new_points_ptr = *new_points;

			for (Iterator<ElementType> it = beginPoint(); it != endPoint(); it++) {
				*new_points_ptr = (double)*it;
				new_points_ptr++;
			}
		}

		/**
			Get Pointer to the normal

			@return Return pointer to the normal
		*/
		ElementType* getNormalsPtr() const
		{
			ElementType* new_normals = new ElementType[number_of_vertices * 3];
			std::memcpy(new_normals, normals, sizeof(ElementType) * number_of_vertices * 3);

			return new_normals;
		}

		/**
			Get Pointer to the normal

			@param[in] new_normals Pointer to the normal
		*/
		void getNormalsPtr(float** new_normals) const
		{
			*new_normals = new float[number_of_vertices * 3];
			float* new_normals_ptr = *new_normals;

			for (Iterator<ElementType> it = beginNormal(); it != endNormal(); it++) {
				*new_normals_ptr = (float)*it;
				new_normals_ptr++;
			}
		}

		/**
			Get Pointer to the normal

			@param[in] new_normals Pointer to the normal
		*/
		void getNormalsPtr(double** new_normals) const
		{
			*new_normals = new double[number_of_vertices * 3];
			double* new_normals_ptr = *new_normals;

			for (Iterator<ElementType> it = beginNormal(); it != endNormal(); it++) {
				*new_normals_ptr = (double)*it;
				new_normals_ptr++;
			}
		}

		/**
			Get Pointer to color information

			@return Return pointer to color information
		*/		
		uint8_t* getColorsPtr() const
		{
			uint8_t* new_colors = new uint8_t[number_of_vertices * 3];
			std::memcpy(new_colors, colors, sizeof(uint8_t) * number_of_vertices * 3);

			return new_colors;
		}

		/**
			Get Pointer to color information

			@param[in] new_colors Pointer to color information
		*/		
		void getColorsPtr(float** new_colors) const
		{
			*new_colors = new float[number_of_vertices * 3];
			float* new_colors_ptr = *new_colors;

			for (Iterator<uint8_t> it = beginColor(); it != endColor(); it++) {
				*new_colors_ptr = (float)*it / 255.0f;
				new_colors_ptr++;
			}
		}

		/**
			Get Pointer to triangles

			@return Return pointer to the triangles
		*/
		size_t* getTrianglesPtr() const
		{
			size_t* new_triangles = new size_t[number_of_triangles * 3];
			std::memcpy(new_triangles, triangles, sizeof(size_t) * number_of_triangles * 3);

			return new_triangles;
		}

		/**
			Get Pointer to triangles

			@param[in] new_triangles Pointer to the triangles
		*/

		void getTrianglesPtr(unsigned int** new_triangles) const
		{
			*new_triangles = new unsigned int[number_of_triangles * 3];
			unsigned int* new_triangles_ptr = *new_triangles;

			for (Iterator<size_t> it = beginTriangle(); it != endTriangle(); it++) {
				*new_triangles_ptr = (unsigned int)*it;
				new_triangles_ptr++;
			}
		}

		/**
			Get Matrix to points

			@param[in] matrix_ Matrix which shall contain the points
		*/
		void getMatrix(utils::Matrix<ElementType>& matrix_) const
		{
			ElementType* data(new ElementType[number_of_vertices*3]);
			std::memcpy(data, points, sizeof(ElementType) * number_of_vertices * 3);

			matrix_.setMatrix(data, number_of_vertices, 3);
		}

		/**
			Returns a pointer to the first entry of the points

			@return Pointer to the first entry of the points
		*/
		IteratorInitializer<ElementType> beginPoint() const
		{
			return IteratorInitializer<ElementType>(points, PointcloudFlag::POINTS);
		};

		/**
			Returns a pointer to the first entry of colors

			@return Pointer to the first entry of colors
		*/
		IteratorInitializer<uint8_t> beginColor() const
		{
			return IteratorInitializer<uint8_t>(colors, PointcloudFlag::RGB);;
		};
				
		/**
			Returns a pointer to the first entry of normals

			@return Pointer to the first entry of normals
		*/
		IteratorInitializer<ElementType> beginNormal() const
		{
			return IteratorInitializer<ElementType>(normals, PointcloudFlag::NORMALS);
		};

		/**
			Returns a pointer to the last entry + 1 of the points

			@return Pointer to the last entry + 1 of the points
		*/
		ElementType* endPoint() const
		{
			return  &points[(getNumberOfVertices() - 1) * 3 + 2] + 1;
		}
		/**
			Returns a pointer to the last entry + 1 of colors

			@return Pointer to the last entry + 1 of colors
		*/
		uint8_t* endColor() const
		{
			return  &colors[(getNumberOfVertices() - 1) * 3 + 2] + 1;
		}
		/**
			Returns a pointer to the last entry + 1 of normals

			@return Pointer to the last entry + 1 of normals
		*/
		ElementType* endNormal() const
		{
			return  &normals[(getNumberOfVertices() - 1) * 3 + 2] + 1;
		}
	
		/**
			Returns a pointer to the last entry + 1 of triangles

			@return Pointer to the last entry + 1 of triangles
		*/
		size_t* endTriangle() const
		{
			return &triangles[(getNumberOfTriangles() - 1) * 3 + 2] + 1;
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
			Iterator() : iterator_(nullptr)
			{
			}

			/**
				Constructor

				@param[in] iterator_initializer Structure for initialization of the iterators
			*/
			Iterator(const IteratorInitializer<IteratorType>& iterator_initializer) : Iterator()
			{
				iterator_ = iterator_initializer.getBegin();
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
				Copy Constructor 

				@param[in] An instance of class Iterator
			*/
			Iterator(const Iterator&& iterator) = delete;
			
			/**
				Operator = 

				@param[in] iterator_initializer Structure for initialization of the iterators
			*/
			Iterator& operator=(const IteratorInitializer<IteratorType>& iterator_initializer)
			{
				iterator_ = iterator_initializer.getBegin();

				return *this;
			}

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
				iterator_++;

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
				Operator != 

				@param[in] iterator Pointer to an element
				@return Returns reference to the current instance
			*/
			bool operator!=(IteratorType* iterator)
			{
				return iterator_ != iterator;
			}

			/**
				Operator == 

				@param[in] iterator Pointer to an element
				@return Returns reference to the current instance
			*/
			bool operator==(IteratorType* iterator)
			{
				return iterator_ == iterator;
			}
			/**
				Operator *

				@return Content of current location of iterator
			*/
			IteratorType& operator*()
			{
				return *iterator_;
			}
			
			/**
				Pointer to the current element
			*/
			IteratorType* iterator_;
		};

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