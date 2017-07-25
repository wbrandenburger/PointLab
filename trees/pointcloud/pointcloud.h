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
			Set color

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
		PointcloudAoS () : Pointcloud() {}

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
			Clear
		*/
		void clear()
		{
			delete pointcloud;
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
					<< pointcloud[i].getColor(0) << " "
					<< pointcloud[i].getColor(1) << " "
					<< pointcloud[i].getColor(2) << " "
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

	////template <typename ElementType>
	////class Pointcloud {

	////public:

	////	/**
	////		Constructor
	////	*/
	////	Pointcloud(void) : rows(0), cols(0)
	////	{
	////	}

	////	/**
	////		Constructor
	////	
	////		@param[in] rows_ Number of points
	////		@param[in] cols_ Number of dimension
	////	*/
	////	Pointcloud(size_t rows_, size_t cols_)
	////	{
	////		points = Matrix<ElementType>(new ElementType[rows_*cols_], rows_, cols_);

	////		rows = rows_;
	////		cols = cols_;
	////	}

	////	/**
	////		Deconstructor
	////	*/
	////	~Pointcloud(void)
	////	{
	////	}

	////	/**
	////		Deletes the point and color array
	////	*/
	////	void clear() const
	////	{
	////		points.clear();
	////		colors.clear();
	////	}

	////	/**
	////		Returns the pointer of the data array
	////	
	////		@return Pointer of pointcloud matrix
	////	*/
	////	ElementType* getPointsPtr() const
	////	{
	////		return points.getPtr();
	////	}

	////	/**
	////		Sets the data array for the points of the pointcloud
	////	
	////		@param[in] rows_ Number of points
	////		@param[in] cols_ Number of dimensions
	////	*/
	////	void setPoints(size_t rows_, size_t cols_)
	////	{
	////		points = Matrix<ElementType>(new ElementType[rows_*cols_], rows_, cols_);

	////		rows = rows_;
	////		cols = cols_;
	////	}

	////	/**
	////		Sets the data array for the colorinformation
	////		
	////		@param[in] rows_ Number of points
	////		@param[in] cols_ Number of colorchannels
	////	*/
	////	void setColors(size_t rows_, size_t cols_)
	////	{
	////		colors = Matrix<uchar>(new uchar[rows_*cols_], rows_, cols_);
	////	}
	////	
	////public:

	////	/**
	////		Pointcloud
	////	*/
	////	Matrix<ElementType> points;
	////	
	////	/**
	////		Color of points
	////	*/
	////	Matrix<uchar> colors;

	////	/**
	////		Number of points
	////	*/
	////	size_t rows;
	////	
	////	/**
	////		Number of dimensions
	////	*/
	////	size_t cols;

	////};
	
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