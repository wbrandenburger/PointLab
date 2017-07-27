/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017  Wolfgang Brandenburger. All rights reserved.
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

#ifndef TREES_POINTCLOUD_H_
#define TREES_POINTCLOUD_H_

#include "utils/matrix.h"

namespace trees
{
	typedef unsigned char uchar;

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
		PointcloudNode(ElementType* point_, ElementType* normal_, uchar* color_)
		{
			setPoint(point_);
			setNormal(normal_);
			setColor(color_);
		}

		/**
			Set point
			
			@param[in] point_ Point
		*/
		void setPoint(ElementType* point_)
		{
			for (size_t i = 0; i < 3; i++) {
				point[i] = point_[i];
			}
		}

		/**
			Set normal

			@param[in] normal_ Normal
		*/
		void setNormal(ElementType* normal_)
		{
			for (size_t i = 0; i < 3; i++) {
				normal[i] = normal_[i];
			}
		}
		
		/**
			Set color

			@param[in] color_ Color
		*/
		void setColor(uchar* color_)
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
		void setPointElement(ElementType point_, size_t index_)
		{
			point[index_] = point_;
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@parma[in] index_ Index
		*/
		void setNormalElement(ElementType normal_, size_t index_)
		{
			normal[index_] = normal_;
		}
		
		/**
			Set color

			@param[in] color_ Color
			@parma[in] index_ Index
		*/
		void setColorElement(uchar color_, size_t index_)
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
		uchar getColor(size_t index_)
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
		uchar* getColorPtr()
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
		uchar color[3];
	};


	template<typename ElementType> class Pointcloud 
	{
	
	public:

		/**
			Constructor
		*/
		Pointcloud() : rows(0), cols(0) {}
		
		/**
			Constructor

			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		Pointcloud(size_t rows_, size_t cols_) : rows(rows_), cols(cols_) {}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		Pointcloud(ElementType* points_, size_t rows_, size_t cols_) : rows(rows_), cols(cols_) {}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] normals_ Normals
			@param[in] colors_ Colors
			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		Pointcloud(ElementType* points_, ElementType* normals_, uchar* colors_, size_t rows_, size_t cols_) 
			: rows(rows_), cols(cols_) {}

		/**
			Set dimension

			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		void setDimension(size_t rows_, size_t cols_)
		{
			rows = rows_;
			cols = cols_;
		}
		
		/**
			Set pointcloud
		*/

		virtual void setPointcloud() = 0;
		/**
			Set pointcloud

			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		virtual void setPointcloud(size_t rows_, size_t cols_) = 0;

		/**
			Clear
		*/
		virtual void clear() = 0;

		/**
			Prints the poincloud

			@param[in,out] out_ Outstream
		*/
		virtual void print(std::ostream& out_) const = 0;

		/**
			Set points

			@param[in] points_ Points
		*/
		virtual void setPoints(ElementType* points_) = 0;

		/**
			Set normals

			@param[in] normals_ Normals
		*/
		virtual void setNormals(ElementType* normals_) = 0;

		/**
			Set color information

			@param[in] colors_ Colors
		*/
		virtual void setColors(uchar* colors_) = 0;
		
		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
		*/
		virtual void setPoint(ElementType* point_, size_t row_) = 0;

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
		*/
		virtual void setNormal(ElementType* normal_,size_t row_) = 0;

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
		*/
		virtual void setColor(uchar* color_, size_t row_) = 0;

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		virtual void setPointElement(ElementType point_, size_t row_, size_t col_) = 0;

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		virtual void setNormalElement(ElementType normal_,size_t row_, size_t col_) = 0;

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		virtual void setColorElement(uchar color_, size_t row_, size_t col_) = 0;

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
		virtual uchar getColor(size_t row_, size_t col_) const = 0;

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
		virtual uchar* getColorPtr(size_t row_) const = 0;

		/**
			Rows
		*/
		size_t rows;

		
		/**
			Cols
		*/
		size_t cols;
	};

	template<typename ElementType> class PointcloudAoS : public Pointcloud<ElementType>
	{
	
	public:

		/**
			Constructor
		*/
		PointcloudAoS () : Pointcloud() 
		{
			pointcloud = nullptr;
		}

		/**
			Constructor

			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		PointcloudAoS(size_t rows_, size_t cols_) : Pointcloud(rows_,cols_)
		{
			pointcloud = new PointcloudNode<ElementType>[rows];
		}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		PointcloudAoS(ElementType* points_, size_t rows_, size_t cols_) 
			: Pointcloud(rows_, cols_)
		{
			pointcloud = new PointcloudNode<ElementType>[rows];
			setPoints(points_);
		}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] normals_ Normals
			@param[in] colors_ Colors
			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		PointcloudAoS(ElementType* points_, ElementType* normals_, uchar* colors_, size_t rows_, size_t cols_)
			: Pointcloud(rows_, cols_) 
		{
			pointcloud = new PointcloudNode<ElementType>[rows];
			setPoints(points_);
			setNormals(normals_);
			setColors(colors_);
		}

		/**
			Set pointcloud
		*/
		void setPointcloud()
		{
			clear();

			pointcloud = new PointCloudNode<ElementType> [rows];
		}
		
		/**
			Set pointcloud
			
			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		void setPointcloud(size_t rows_, size_t cols_)
		{
			clear();

			rows = rows_;
			cols = cols_;

			pointcloud = new PointCloudNode<ElementType >[rows];
		}
		/**
			Clear
		*/
		void clear()
		{
			if (pointcloud) delete[] pointcloud;
		}

		/**
			Prints the pointcloud

			@param[in,out] out_ Outstream
		*/
		void print(std::ostream& out_) const
		{
			for (size_t i = 0; i < rows; i++) {
				out_ << pointcloud[i].getPoint(0) << " "
					<< pointcloud[i].getPoint(1) << " "
					<< pointcloud[i].getPoint(2) << " "
					<< pointcloud[i].getNormal(0) << " "
					<< pointcloud[i].getNormal(1) << " "
					<< pointcloud[i].getNormal(2) << " "
					<< (int) pointcloud[i].getColor(0) << " "
					<< (int) pointcloud[i].getColor(1) << " "
					<< (int) pointcloud[i].getColor(2) << " "
					<< std::endl;
			}
		}

		/**
			Set points

			@param[in] points_ Points
		*/
		void setPoints(ElementType* points_)
		{
			for (size_t i = 0; i < rows; i++) {
				pointcloud[i].setPoint(&points_[i*cols]);
			}
		}

		/**
			Set normals

			@param[in] normals_ Normals
		*/
		void setNormals(ElementType* normals_)
		{
			for (size_t i = 0; i < rows; i++) {
				pointcloud[i].setNormal(&normals_[i*cols]);
			}
		}

		/**
			Set normals

			@param[in] colors_ Colors
		*/
		void setColors(uchar* colors_)
		{
			for (size_t i = 0; i < rows; i++) {
				pointcloud[i].setColor(&colors_[i*cols]);
			}
		}

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
		*/
		void setPoint(ElementType* point_, size_t row_)
		{
			pointcloud[row_].setPoint(point_);
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
		*/
		void setNormal(ElementType* normal_, size_t row_)
		{
			pointcloud[row_].setNormal(normal_);
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
		*/
		void setColor(uchar* color_, size_t row_)
		{
			pointcloud[row_].setColor(color_);
		}

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setPointElement(ElementType point_, size_t row_, size_t col_)
		{
			pointcloud[row_].setPointElement(point_, col_);
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setNormalElement(ElementType normal_, size_t row_, size_t col_)
		{
			pointcloud[row_].setNormalElement(normal_, col_);
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setColorElement(uchar color_, size_t row_, size_t col_)
		{
			pointcloud[row_].setColorElement(color_, col_);
		}

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
		uchar getColor(size_t row_, size_t col_) const
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
		uchar* getColorPtr(size_t row_) const
		{
			return pointcloud[row_].getColorPtr();
		}

		 /**
			Pointcloud
		 */
		 PointcloudNode<ElementType>* pointcloud;

	};

	template<typename ElementType> class PointcloudSoA : public Pointcloud<ElementType>
	{
	
	public:

		/**
			Constructor
		*/
		PointcloudSoA () : Pointcloud() 
		{
			points = nullptr;
			normals = nullptr;
			colors = nullptr;
		}
		/**
			Constructor

			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		PointcloudSoA(size_t rows_, size_t cols_) : Pointcloud(rows_,cols_)
		{
			points = new ElementType[rows*cols];
			normals = new ElementType[rows*cols];
			colors = new uchar[rows*cols];
		}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		PointcloudSoA(ElementType* points_, size_t rows_, size_t cols_) 
			: Pointcloud(rows_, cols_)
		{
			setPoints(points_);
			normals = new ElementType[rows*cols];
			colors = new uchar[rows*cols];
		}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] normals_ Normals
			@param[in] colors_ Colors
			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		PointcloudSoA(ElementType* points_, ElementType* normals_, uchar* colors_, size_t rows_, size_t cols_)
			: Pointcloud(rows_, cols_) 
		{
			setPoints(points_);
			setNormals(normals_);
			setColors(colors_);
		}

		/**
			Set pointcloud
		*/
		void setPointcloud()
		{
			clear();

			points = new ElementType[rows*cols];
			normals = new ElementType[rows*cols];
			colors = new uchar[rows*cols];
		}

		/**
			Set pointcloud

			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		void setPointcloud(size_t rows_, size_t cols_)
		{
			clear();

			rows = rows_;
			cols = cols_;

			points = new ElementType[rows*cols];
			normals = new ElementType[rows*cols];
			colors = new uchar[rows*cols];
		}

		/**
			Clear
		*/
		void clear()
		{
			if (points) { delete[] points; }
			if (normals) { delete[] normals; }
			if (colors) { delete[] colors; }
		}

		/**
			Prints the pointcloud

			@param[in,out] out_ Outstream
		*/
		void print(std::ostream& out_) const
		{
			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					if (points) {
						out_ << points[i*cols + j] << " ";
					}
				}
				for (size_t j = 0; j < cols; j++) {
					if (normals) {
						out_ << normals[i*cols + j] << " ";
					}
				}
				for (size_t j = 0; j < cols; j++) {
					if (colors) {
						out_ << (int) colors[i*cols + j] << " ";
					}
				}
				out_ << std::endl;
			}
		}

		/**
			Set points

			@param[in] points_ Points
		*/
		void setPoints(ElementType* points_)
		{
			points = new ElementType[rows*cols];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					points[i*cols + j] = points_[i*cols + j];
				}
			}
		}

		/**
			Set normals

			@param[in] normals_ Normals
		*/
		void setNormals(ElementType* normals_)
		{
			normals = new ElementType[rows*cols];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					normals[i*cols + j] = normals_[i*cols + j];
				}
			}
		}

		/**
			Set normals

			@param[in] colors_ Colors
		*/
		void setColors(uchar* colors_)
		{
			colors = new uchar[rows*cols];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					colors[i*cols + j] = colors_[i*cols + j];
				}
			}
		}

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
		*/
		void setPoint(ElementType* point_, size_t row_)
		{
			for (size_t i = 0; i < cols; i++) {
				points[row_*cols + i] = point_[i];
			}
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
		*/
		void setNormal(ElementType* normal_, size_t row_)
		{
			for (size_t i = 0; i < cols; i++) {
				normals[row_*cols + i] = normal_[i];
			}
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
		*/
		void setColor(uchar* color_, size_t row_)
		{
			for (size_t i = 0; i < cols; i++) {
				colors[row_*cols + i] = color_[i];
			}
		}

		/**
			Set point

			@param[in] point_ Point
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setPointElement(ElementType point_, size_t row_, size_t col_)
		{
			points[row_*cols + col_] = point_;
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setNormalElement(ElementType normal_, size_t row_, size_t col_)
		{
			normals[row_*cols + col_] = normal_;
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setColorElement(uchar color_, size_t row_, size_t col_)
		{
			colors[row_*cols + col_] = color_;
		}

		/**
			Get point data of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return point data of specified index
		*/
		ElementType getPoint(size_t row_, size_t col_) const
		{
			return points[row_*cols + col_];
		}

		/**
			Get normal information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return normal information of specified index
		*/
		ElementType getNormal(size_t row_, size_t col_) const
		{
			return normals[row_*cols + col_];
		}

		/**
			Get color information of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return color information of specified index
		*/
		uchar getColor(size_t row_, size_t col_) const
		{
			return colors[row_*cols + col_];
		}

		/**
			Operator [] Access on point information

			@param[in] index_ Dimension
			@return Return pointer to point data
		*/
		ElementType* operator[](size_t index_) const
		{
			return &points[index_*cols];
		}

		/**
			Get Pointer to point data

			@param[in] row_ Row
			@return Return pointer to point data
		*/
		ElementType* getPointPtr(size_t row_) const
		{
			return &points[row_*cols];
		}

		/**
			Get Pointer to the normal

			@param[in] row_ Row
			@return Return pointer to the normal
		*/
		ElementType* getNormalPtr(size_t row_) const
		{
			return &normals[row_*cols];
		}

		/**
			Get Pointer to color information

			@param[in] row_ Row
			@return Return pointer to color information
		*/		
		uchar* getColorPtr(size_t row_) const
		{
			return &colors[row_*cols];
		}

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
		 uchar* colors;
	};
	
	/**
		Operator << Prints the values of the heap

		@param[in,out] out_ Outstream in which the heap will be printed
		@param[in] heap_ Heap which values shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const Pointcloud<ElementType>& pointcloud_)
	{
		
		pointcloud_.print(out_);

		return out_;
	}
}

#endif /* TREES_POINTCLOUD_H_ */