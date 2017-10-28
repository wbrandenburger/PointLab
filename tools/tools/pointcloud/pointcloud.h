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

#ifndef POINTCLOUD_POINTCLOUD_H_
#define POINTCLOUD_POINTCLOUD_H_

#include <stdint.h>

#include "tools/parameters.h"

#include "tools/utils/matrix.h"

#include "tools/pointcloud/pointcloudnodes.h"
#include "tools/pointcloud/pointcloudsoa.h"
#include "tools/pointcloud/pointcloudaos.h"

namespace pointcloud
{
	template<typename ElementType> class Pointcloud 
	{

	public:

		/**
			Constructor
		*/
		Pointcloud() : number_of_vertices(0), number_of_triangles(0), 
			triangles(nullptr),
			color_flag(false), normal_flag(false), triangle_flag(false)
		{
		}

		/**
			Constructor
			
			@param[in] flags_ The flags determine which fields has to be set
		*/
		Pointcloud(uint8_t flags_) : Pointcloud()
		{
			setFlags(flags_);
		}
		
		/**
			Constructor

			@param[in] number_of_vertices_ Number of vertices
			@param[in] flags_ The flags determine which fields has to be set
		*/
		Pointcloud(size_t number_of_vertices_, uint8_t flags_) : Pointcloud()
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
		Pointcloud(size_t number_of_vertices_, size_t number_of_triangles_, uint8_t flags_) : Pointcloud()
		{
			setFlags(flags_);
			setNumberOfElements(number_of_vertices_, number_of_triangles_);
		}

		/**
			Deconstructor
		*/
		virtual ~Pointcloud() {}

		/**
			Set number of vertices

			@param[in] number_of_vertices_ Number of vertices
		*/
		void setNumberOfVertices(size_t number_of_vertices_)
		{
			if (number_of_vertices) {
				exitFailure(__FILE__, __LINE__);
			}
			number_of_vertices = number_of_vertices_;
		}

		/**
			Set number of triangles

			@param[in] number_of_triangles_ Number of triangles
		*/
		void setNumberOfTriangles(size_t number_of_triangles_)
		{
			if (number_of_triangles) {
				exitFailure(__FILE__, __LINE__);
			}
			else {
				isTriangle = true;
			}

			number_of_triangles = number_of_triangles_;
		}	

		/**
			Set number of vertices and triangles

			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_triangles_ Number of triangles
		*/
		void setNumberOfElements(size_t number_of_vertices_, size_t number_of_triangles_)
		{
			if (number_of_vertices || number_of_triangles) {
				exitFailure(__FILE__, __LINE__);
			}
			else if(!isTriangle()) {
				triangle_flag = true;
			}

			number_of_vertices = number_of_vertices_;
			number_of_triangles = number_of_triangles_;
		}
	
	protected:
		/**
			Set the flags
		*/
		void setFlags(uint8_t flags_)
		{
			color_flag = (flags_ & 1 << 1) > 0 || (flags_ & 1 << 6) > 0 || (flags_ & 1 << 7) > 0;
			normal_flag = (flags_ & 1 << 2) > 0 || (flags_ & 1 << 6) > 0 || (flags_ & 1 << 7) > 0;
			triangle_flag = (flags_ & 1 << 3) > 0 || (flags_ & 1 << 7) > 0;
		}

	protected:
		/**
			Allocte the meory for the triangles
		*/
		void allocateMemoryTriangles()
		{
			if (isTriangle()) {
				triangles = new size_t[number_of_triangles * 3];
			}
		}
	
	public:
		/**
			Set pointcloud
		*/

		virtual void setPointcloud() = 0;
		/**
			Set pointcloud

			@param[in] number_of_vertices_ Number of vertices
		*/
		virtual void setPointcloud(size_t number_of_vertices_) = 0;

		/**
			Set pointcloud

			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_triangles_ Number of triangles
		*/
		virtual void setPointcloud(size_t number_of_vertices_, size_t number_of_triangles_) = 0;

		/**
			Set triangles

			@param[in] number_of_triangles_ Number of triangles
		*/
		void setTriangles(size_t number_of_triangles_)
		{
			setNumberOfTriangles(number_of_triangles_);

			clearMemoryTriangles();

			allocateMemoryTriangles();
		}
	
	protected:
		/**
			Clear
		*/
		virtual void clearMemoryPointcloud() = 0;

		/**
			Clear triangles
		*/
		void clearMemoryTriangles()
		{
			std::cout << "4" << std::endl;
			if (triangles) {
				delete[] triangles;
				triangles = nullptr;
			}
		}

	public:
		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		virtual void getSubset(std::vector<size_t> list_, Pointcloud<ElementType>& subset_) const = 0;

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		virtual void getSubset(std::vector<int> list_, Pointcloud<ElementType>& subset_) const  = 0;
		
		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		virtual void getSubset(std::vector<size_t> list_, utils::Matrix<ElementType>& subset_) const = 0;

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		virtual void getSubset(std::vector<int> list_, utils::Matrix<ElementType>& subset_) const  = 0;
		
		/**
			Set points

			@param[in] points_ Points
		*/
		virtual void setPointsPtr(ElementType* points_) = 0;

		/**
			Set normals

			@param[in] normals_ Normals
		*/
		virtual void setNormalsPtr(ElementType* normals_) = 0;

		/**
			Set color information

			@param[in] colors_ Colors
		*/
		virtual void setColorsPtr(uint8_t* colors_) = 0;
		
		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
		*/
		virtual void setPointPtr(ElementType* point_, size_t row_) = 0;

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
		*/
		virtual void setNormalPtr(ElementType* normal_,size_t row_) = 0;

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
		*/
		virtual void setColorPtr(uint8_t* color_, size_t row_) = 0;

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		virtual void setPoint(ElementType point_, size_t row_, size_t col_) = 0;

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		virtual void setNormal(ElementType normal_,size_t row_, size_t col_) = 0;

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		virtual void setColor(uint8_t color_, size_t row_, size_t col_) = 0;

		/**
			Get Number of vertices
			
			@return Number of vertices
		*/
		size_t getNumberOfVertices() const
		{
			return number_of_vertices;
		}

		/**
			Get Number of triangles
		*/
		size_t getNumberOfTriangles() const
		{
			return number_of_triangles;
		}

		/**
			Get point data of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return point data of specified index
		*/
		virtual ElementType getPoint(size_t row_, size_t col_) const = 0;

		/**
			Get normal information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return normal information of specified index
		*/
		virtual ElementType getNormal(size_t row_, size_t col_) const = 0;

		/**
			Get color information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return color information of specified index
		*/
		virtual uint8_t getColor(size_t row_, size_t col_) const = 0;

		/**
			Operator [] Access on point information

			@param[in] index_ Dimension
			@return Return pointer to point data
		*/
		virtual ElementType* operator[](size_t index_) const = 0;

		/**
			Get Pointer to point data

			@param[in] row_ Row
			@return Return pointer to point data
		*/
		virtual ElementType* getPointPtr(size_t row_) const = 0;

		/**
			Get Pointer to the normal

			@param[in] row_ Row
			@return Return pointer to the normal
		*/
		virtual ElementType* getNormalPtr(size_t row_) const = 0;

		/**
			Get Pointer to color information

			@param[in] row_ Row
			@return Return pointer to color information
		*/		
		virtual uint8_t* getColorPtr(size_t row_) const = 0;

		/**
			Get Pointer to point data

			@return Return pointer to point data
		*/
		virtual ElementType* getPointsPtr() const = 0;

		/**
			Get Pointer to the normal

			@return Return pointer to the normal
		*/
		virtual ElementType* getNormalsPtr() const = 0;

		/**
			Get Pointer to color information

			@return Return pointer to color information
		*/		
		virtual uint8_t* getColorsPtr() const = 0;

		/**
			Get Matrix to points

			@param[in] matrix_ Matrix which shall contain the points
		*/
		virtual void getMatrix(utils::Matrix<ElementType>& matrix_) const = 0;

		/**
			Returns true if colors are set

			@return True if colors are set 
		*/
		bool isColor() const
		{
			return color_flag;
		}

		/**
			Returns true if normals are set

			@return True if normals are set 
		*/
		bool isNormal() const
		{
			return normal_flag;
		}

		/**
			Returns true if triangles are set

			@return True if triangles are set 
		*/
		bool isTriangle() const
		{
			return triangle_flag;
		}

	protected:
		/**
			Set color flag
		*/
		void setColorFlag()
		{
			color_flag = true;
		}

		/**
			Set normal flag 
		*/
		bool setNormalFlag()
		{
			normal_flag = true;
		}

		/**
			Set triangle flag 
		*/
		bool setTriangleFlag()
		{
			triangle_flag = true;
		}

	protected:
		/**
			Number of vertices
		*/
		size_t number_of_vertices;

		/**
			Number of triangles
		*/
		size_t number_of_triangles;

		/**
			Indices which specifiy triangles
		*/
		size_t* triangles;

		/**
			Flag for colors
		*/
		bool color_flag;

		/**
			Flag for normals
		*/
		bool normal_flag;

		/**
			Flag for triangles
		*/
		bool triangle_flag;

	};

	
	/**
		Operator << Prints the values of the quaterion

		@param[in,out] out_ Outstream in which the pointcloud will be printed
		@param[in] quaterion_ Pointcloud which shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const pointcloud::Pointcloud<ElementType>& pointcloud_)
	{
		size_t number = pointcloud_.getNumberOfVertices() > 10 ? 10 : pointcloud_.getNumberOfVertices();

		out_ << number << std::endl;

		return out_;
	}
}

#endif /* POINTCLOUD_POINTCLOUD_H_ */