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

namespace pointcloud
{
	/**
		Forward declaration of class Pointcloud
	*/
	template<typename ElementType> class Pointcloud;

	/**
		Forward declaration of class PointcloudAoS
	*/
	template<typename ElementType> class PointcloudAoS;

	template<typename ElementType> class PointcloudSoA : public Pointcloud<ElementType>
	{
	
	public:

		/**
			Constructor

			@param[in] flags_ The flags determine which fields has to be set
		*/
		PointcloudSoA (unsigned char flags_) : Pointcloud(flags_) 
		{
			points = nullptr;
			normals = nullptr;
			colors = nullptr;
		}
		/**
			Constructor

			@param[in] number_of_vertices_ Number of vertices
			@param[in] flags_ The flags determine which fields has to be set
		*/
		PointcloudSoA(size_t number_of_vertices_, unsigned char flags_) : Pointcloud(number_of_vertices_, flags_)
		{
			points = new ElementType[number_of_vertices*3];
			normals = new ElementType[number_of_vertices*3];
			colors = new uint8_t[number_of_vertices*3];
		}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of vertices
			@param[in] flags_ The flags determine which fields has to be set
		*/
		PointcloudSoA(ElementType* points_, size_t number_of_vertices_, unsigned char flags_) 
			: Pointcloud(number_of_vertices_, flags_)
		{
			setPoints(points_);
			normals = new ElementType[number_of_vertices*3];
			colors = new uint8_t[number_of_vertices*3];
		}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] normals_ Normals
			@param[in] colors_ Colors
			@param[in] number_of_vertices_ Number of vertices
			@param[in] flags_ The flags determine which fields has to be set
		*/
		PointcloudSoA(ElementType* points_, ElementType* normals_, uint8_t* colors_, size_t number_of_vertices_, unsigned char flags_)
			: Pointcloud(number_of_vertices_, flags) 
		{
			setPoints(points_);
			setNormals(normals_);
			setColors(colors_);
		}

		/**
			Deconstructor
		*/
		~PointcloudSoA()
		{
			clear();
		}

		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA(const PointcloudSoA<ElementType>& pointcloud_)
		{
			number_of_vertices = pointcloud_.number_of_vertices;

			points = new ElementType[number_of_vertices*3];
			normals = new ElementType[number_of_vertices*3];
			colors = new uint8_t[number_of_vertices*3];

			for (size_t i = 0; i < number_of_vertices; i++) {
				setPointPtr(pointcloud_.getPointPtr(i), i);
				setNormalPtr(pointcloud_.getNormalPtr(i), i);
				setColorPtr(pointcloud_.getColorPtr(i), i);
			}
		}
		
		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA(const PointcloudAoS<ElementType>& pointcloud_)
		{
			number_of_vertices = pointcloud_.number_of_vertices;

			points = new ElementType[number_of_vertices*3];
			normals = new ElementType[number_of_vertices*3];
			colors = new uint8_t[number_of_vertices*3];

			for (size_t i = 0; i < number_of_vertices; i++) {
				setPointPtr(pointcloud_.getPointPtr(i), i);
				setNormalPtr(pointcloud_.getNormalPtr(i), i);
				setColorPtr(pointcloud_.getColorPtr(i), i);
			}
		}

		/**
			Operator =

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA& operator=(const PointcloudSoA<ElementType>& pointcloud_)
		{
			clear();

			number_of_vertices = pointcloud_.number_of_vertices;

			points = new ElementType[number_of_vertices*3];
			normals = new ElementType[number_of_vertices*3];
			colors = new uint8_t[number_of_vertices*3];

			for (size_t i = 0; i < number_of_vertices; i++) {
				setPointPtr(pointcloud_.getPointPtr(i), i);
				setNormalPtr(pointcloud_.getNormalPtr(i), i);
				setColorPtr(pointcloud_.getColorPtr(i), i);
			}

			return (*this);
		}
		
		/**
			Operator =

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudSoA& operator=(const PointcloudAoS<ElementType>& pointcloud_)
		{
			clear();

			number_of_vertices = pointcloud_.number_of_vertices;

			points = new ElementType[number_of_vertices*3];
			normals = new ElementType[number_of_vertices*3];
			colors = new uint8_t[number_of_vertices*3];

			for (size_t i = 0; i < number_of_vertices; i++) {
				setPointPtr(pointcloud_.getPointPtr(i), i);
				setNormalPtr(pointcloud_.getNormalPtr(i), i);
				setColorPtr(pointcloud_.getColorPtr(i), i);
			}

			return (*this);
		}

		/**
			Set pointcloud
		*/
		void setPointcloud()
		{
			clear();

			points = new ElementType[number_of_vertices*3];
			normals = new ElementType[number_of_vertices*3];
			colors = new uint8_t[number_of_vertices*3];
		}

		/**
			Set pointcloud

			@param[in] number_of_vertices_ Number of vertices
		*/
		void setPointcloud(size_t number_of_vertices_)
		{
			clear();

			number_of_vertices = number_of_vertices_;

			points = new ElementType[number_of_vertices*3];
			normals = new ElementType[number_of_vertices*3];
			colors = new uint8_t[number_of_vertices*3];
		}

		/**
			Clear
		*/
		void clear()
		{
			number_of_vertices = 0;

			if (points) { delete[] points; }
			if (normals) { delete[] normals; }
			if (colors) { delete[] colors; }
		}
		
		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(std::vector<size_t> list_, Pointcloud<ElementType>& subset_) const
		{
			subset_.setPointcloud(list_.size());

			for (size_t i = 0; i < list_.size(); i++) {
				if (list_[i] >= number_of_vertices) {
					std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
					std::exit(EXIT_FAILURE);
				}
				subset_.setPointPtr(getPointPtr(list_[i]),i);
				subset_.setNormalPtr(getNormalPtr(list_[i]), i);
				subset_.setColorPtr(getColorPtr(list_[i]), i);
			}
		}

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(std::vector<int> list_, Pointcloud<ElementType>& subset_) const
		{
			subset_.setPointcloud(list_.size());

			for (size_t i = 0; i < list_.size(); i++) {
				if (list_[i] >= number_of_vertices) {
					std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
					std::exit(EXIT_FAILURE);
				}
				subset_.setPointPtr(getPointPtr(list_[i]), i);
				subset_.setNormalPtr(getNormalPtr(list_[i]), i);
				subset_.setColorPtr(getColorPtr(list_[i]), i);
			}
		}

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(std::vector<size_t> list_, utils::Matrix<ElementType>& subset_) const
		{
			for (size_t i = 0; i < list_.size(); i++) {
				//for (size_t j = 0; j < 3; j++) {
				//	subset_[i][j] = getPoint(list_[i], j);
				//}
				std::memcpy(subset_[i],getPointPtr(list_[i]), sizeof(ElementType)*3);
			}
		}

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(std::vector<int> list_, utils::Matrix<ElementType>& subset_) const
		{
			for (size_t i = 0; i < list_.size(); i++) {
				//for (size_t j = 0; j < 3; j++) {
				//	subset_[i][j] = getPoint(list_[i], j);
				//}
				std::memcpy(subset_[i],getPointPtr(list_[i]), sizeof(ElementType)*3);
			}
		}

		/**
			Prints the pointcloud

			@param[in,out] out_ Outstream
		*/
		void print(std::ostream& out_) const
		{
			size_t number = number_of_vertices < 10 ? number_of_vertices : 10;
			for (size_t i = 0; i < number; i++) {
				for (size_t j = 0; j < 3; j++) {
					out_ << points[i*3 + j] << " ";
				}
				for (size_t j = 0; j < 3; j++) {
					out_ << normals[i*3 + j] << " ";
				}
				for (size_t j = 0; j < 3; j++) {
					out_ << (int) colors[i*3 + j] << " ";
				}
				out_ << std::endl;
			}
		}

		/**
			Set points

			@param[in] points_ Points
		*/
		void setPointsPtr(ElementType* points_)
		{
			points = new ElementType[number_of_vertices*3];

			for (size_t i = 0; i < number_of_vertices; i++) {
				for (size_t j = 0; j < 3; j++) {
					points[i*3 + j] = points_[i*3 + j];
				}
			}
		}

		/**
			Set normals

			@param[in] normals_ Normals
		*/
		void setNormalsPtr(ElementType* normals_)
		{
			normals = new ElementType[number_of_vertices*3];

			for (size_t i = 0; i < number_of_vertices; i++) {
				for (size_t j = 0; j < 3; j++) {
					normals[i*3 + j] = normals_[i*3 + j];
				}
			}
		}

		/**
			Set normals

			@param[in] colors_ Colors
		*/
		void setColorsPtr(uint8_t* colors_)
		{
			colors = new uint8_t[number_of_vertices*3];

			for (size_t i = 0; i < number_of_vertices; i++) {
				for (size_t j = 0; j < 3; j++) {
					colors[i*3 + j] = colors_[i*3 + j];
				}
			}
		}

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
		*/
		void setPointPtr(ElementType* point_, size_t row_)
		{
			for (size_t i = 0; i < 3; i++) {
				points[row_*3 + i] = point_[i];
			}
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
		*/
		void setNormalPtr(ElementType* normal_, size_t row_)
		{
			for (size_t i = 0; i < 3; i++) {
				normals[row_*3 + i] = normal_[i];
			}
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
		*/
		void setColorPtr(uint8_t* color_, size_t row_)
		{
			for (size_t i = 0; i < 3; i++) {
				colors[row_*3 + i] = color_[i];
			}
		}

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setPoint(ElementType point_, size_t row_, size_t col_)
		{
			points[row_*3 + col_] = point_;
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setNormal(ElementType normal_, size_t row_, size_t col_)
		{
			normals[row_*3 + col_] = normal_;
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setColor(uint8_t color_, size_t row_, size_t col_)
		{
			colors[row_*3 + col_] = color_;
		}

		/**
			Get point data of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return point data of specified index
		*/
		ElementType getPoint(size_t row_, size_t col_) const
		{
			return points[row_*3 + col_];
		}

		/**
			Get normal information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return normal information of specified index
		*/
		ElementType getNormal(size_t row_, size_t col_) const
		{
			return normals[row_*3 + col_];
		}

		/**
			Get color information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return color information of specified index
		*/
		uint8_t getColor(size_t row_, size_t col_) const
		{
			return colors[row_*3 + col_];
		}

		/**
			Operator [] Access on point information

			@param[in] index_ Dimension
			@return Return pointer to point data
		*/
		ElementType* operator[](size_t index_) const
		{
			return &points[index_*3];
		}

		/**
			Get Pointer to point data

			@param[in] row_ Row
			@return Return pointer to point data
		*/
		ElementType* getPointPtr(size_t row_) const
		{
			return &points[row_*3];
		}

		/**
			Get Pointer to the normal

			@param[in] row_ Row
			@return Return pointer to the normal
		*/
		ElementType* getNormalPtr(size_t row_) const
		{
			return &normals[row_*3];
		}

		/**
			Get Pointer to color information

			@param[in] row_ Row
			@return Return pointer to color information
		*/		
		uint8_t* getColorPtr(size_t row_) const
		{
			return &colors[row_*3];
		}

		/**
			Get Pointer to point data

			@return Return pointer to point data
		*/
		ElementType* getPointsPtr() const
		{
			ElementType* new_points = new ElementType[number_of_vertices*3];

			for (size_t i = 0; i < number_of_vertices; i++) {
				for (size_t j = 0; j < 3; j++) {
					new_points[i*3 + j] = points[i*3 + j];
				}
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

			for (size_t i = 0; i < number_of_vertices; i++) {
				for (size_t j = 0; j < 3; j++) {
					new_normals[i*3 + j] = normals[i*3 + j];
				}
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

			for (size_t i = 0; i < number_of_vertices; i++) {
				for (size_t j = 0; j < 3; j++) {
					new_colors[i*3 + j] = colors[i*3 + j];
				}
			}

			return new_colors;
		}

		/**
			Get Matrix to points

			@param[in] matrix_ Matrix which shall contain the points
		*/
		void getMatrix(utils::Matrix<ElementType>& matrix_) const
		{
			ElementType* data(new ElementType[number_of_vertices*3]);
			std::memcpy(data, points, sizeof(ElementType)*number_of_vertices*3);

			matrix_.setMatrix(data, number_of_vertices, 3);
		}

	private:
		/**
			Pointcloud
		*/
		ElementType* points;
		 
		/**
			Normals
		*/
		ElementType* normals;

		/**
			Colors
		*/
		uint8_t* colors;
	};
}

#endif /* POINTCLOUD_POINTCLOUDSOA_H_ */