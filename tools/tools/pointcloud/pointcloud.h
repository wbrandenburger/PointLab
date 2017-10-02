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

#ifndef POINTCLOUD_POINTCLOUD_H_
#define POINTCLOUD_POINTCLOUD_H_

#include "tools/utils/matrix.h"

namespace pointcloud
{
	typedef unsigned char uchar;

	template<typename ElementType> class Pointcloud 
	{
	
	public:

		/**
			Constructor
		*/
		Pointcloud() : rows(0), cols(0), print_number(10) {}
		
		/**
			Constructor

			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		Pointcloud(size_t rows_, size_t cols_) : rows(rows_), cols(cols_), print_number(10) {}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		Pointcloud(ElementType* points_, size_t rows_, size_t cols_) : rows(rows_), cols(cols_), print_number(10) {}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] normals_ Normals
			@param[in] colors_ Colors
			@param[in] rows_ Rows
			@param[in] cols_ Cols
		*/
		Pointcloud(ElementType* points_, ElementType* normals_, uchar* colors_, size_t rows_, size_t cols_) 
			: rows(rows_), cols(cols_), print_number(10) {}

		/**
			Deconstructor
		*/
		virtual ~Pointcloud() {}

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
			Set the number of printed points
		*/
		void setPrintNumber(size_t print_number_)
		{
			print_number = print_number_;
		}

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
		virtual void setColorsPtr(uchar* colors_) = 0;
		
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
		virtual void setColorPtr(uchar* color_, size_t row_) = 0;

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
		virtual void setColor(uchar color_, size_t row_, size_t col_) = 0;

		/**
			Get Rows
			
			@return Rows
		*/
		size_t getRows() const
		{
			return rows;
		}
		
		/**
			Get Cols
			
			@return Cols
		*/
		size_t getCols() const
		{
			return cols;
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
		virtual uchar* getColorsPtr() const = 0;

		/**
			Get Matrix to points

			@param[in] matrix_ Matrix which shall contain the points
		*/
		virtual void getMatrix(utils::Matrix<ElementType>& matrix_) const = 0;

		/**
			Rows
		*/
		size_t rows;
		
		/**
			Cols
		*/
		size_t cols;

		/**
			Number of printed points
		*/
		size_t print_number;
	};
		
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
		void setPointPtr(ElementType* point_)
		{
			for (size_t i = 0; i < 3; i++) {
				point[i] = point_[i];
			}
		}

		/**
			Set normal

			@param[in] normal_ Normal
		*/
		void setNormalPtr(ElementType* normal_)
		{
			for (size_t i = 0; i < 3; i++) {
				normal[i] = normal_[i];
			}
		}
		
		/**
			Set color

			@param[in] color_ Color
		*/
		void setColorPtr(uchar* color_)
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
		void setPoint(ElementType point_, size_t index_)
		{
			point[index_] = point_;
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
			Set color

			@param[in] color_ Color
			@parma[in] index_ Index
		*/
		void setColor(uchar color_, size_t index_)
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

	/** 
		Forward declaration of class PointcloudSoA without derivation
	*/
	template<typename ElementType> class PointcloudSoA;

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
			Deconstructor
		*/
		~PointcloudAoS()
		{
			clear();
		}

		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudAoS(const PointcloudAoS<ElementType>& pointcloud_)
		{
			rows = pointcloud_.rows;
			cols = pointcloud_.cols;

			pointcloud = new PointcloudNode<ElementType>[rows];

			for (size_t i = 0; i < rows; i++) {
				setPointPtr(pointcloud_.getPointPtr(i), i);
				setNormalPtr(pointcloud_.getNormalPtr(i), i);
				setColorPtr(pointcloud_.getColorPtr(i), i);
			}
		}
		
		/**
			Copy constructor

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudAoS(const PointcloudSoA<ElementType>& pointcloud_)
		{
			rows = pointcloud_.rows;
			cols = pointcloud_.cols;

			pointcloud = new PointcloudNode<ElementType>[rows];

			for (size_t i = 0; i < rows; i++) {
				setPointPtr(pointcloud_.getPointPtr(i), i);
				setNormalPtr(pointcloud_.getNormalPtr(i), i);
				setColorPtr(pointcloud_.getColorPtr(i), i);
			}
		}

		/**
			Operator =

			@param[in] pointcloud_ Pointcloud
		*/
		PointcloudAoS& operator=(const PointcloudAoS<ElementType>& pointcloud_)
		{
			clear();

			rows = pointcloud_.rows;
			cols = pointcloud_.cols;

			pointcloud = new PointcloudNode<ElementType>[rows];

			for (size_t i = 0; i < rows; i++) {
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
		PointcloudAoS& operator=(const PointcloudSoA<ElementType>& pointcloud_)
		{
			clear();

			rows = pointcloud_.rows;
			cols = pointcloud_.cols;

			pointcloud = new PointcloudNode<ElementType>[rows];

			for (size_t i = 0; i < rows; i++) {
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

			pointcloud = new PointcloudNode<ElementType> [rows];
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

			pointcloud = new PointcloudNode<ElementType>[rows];
		}

		/**
			Clear
		*/
		void clear()
		{
			rows = 0;
			cols = 0;

			if (pointcloud) delete[] pointcloud;
		}

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(std::vector<size_t> list_, Pointcloud<ElementType>& subset_) const
		{
			subset_.setPointcloud(list_.size(), cols);
			
			for (size_t i = 0; i < list_.size(); i++) {
				if (list_[i] >= rows) {
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
		void getSubset(std::vector<int> list_, Pointcloud<ElementType>& subset_) const
		{
			subset_.setPointcloud(list_.size(), cols);

			for (size_t i = 0; i < list_.size(); i++) {
				if (list_[i] >= rows) {
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
			size_t number = rows < print_number ? rows : print_number;
			for (size_t i = 0; i < number; i++) {
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
		void setPointsPtr(ElementType* points_)
		{
			for (size_t i = 0; i < rows; i++) {
				pointcloud[i].setPointPtr(&points_[i*cols]);
			}
		}

		/**
			Set normals

			@param[in] normals_ Normals
		*/
		void setNormalsPtr(ElementType* normals_)
		{
			for (size_t i = 0; i < rows; i++) {
				pointcloud[i].setNormalPtr(&normals_[i*cols]);
			}
		}

		/**
			Set normals

			@param[in] colors_ Colors
		*/
		void setColorsPtr(uchar* colors_)
		{
			for (size_t i = 0; i < rows; i++) {
				pointcloud[i].setColorPtr(&colors_[i*cols]);
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
		void setColorPtr(uchar* color_, size_t row_)
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
		void setColor(uchar color_, size_t row_, size_t col_)
		{
			pointcloud[row_].setColor(color_, col_);
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
			Get Pointer to point data

			@return Return pointer to point data
		*/
		ElementType* getPointsPtr() const
		{
			ElementType* new_points = new ElementType[rows*cols];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					new_points[i*cols + j] = getPoint(i, j);
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
			ElementType* new_normals = new ElementType[rows*cols];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					new_normals[i*cols + j] = getNormal(i, j);
				}
			}

			return new_normals;
		}

		/**
			Get Pointer to color information

			@return Return pointer to color information
		*/		
		uchar* getColorsPtr() const
		{
			uchar* new_colors = new uchar[rows*cols];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					new_colors[i*cols + j] = getColor(i, j);
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
			ElementType* data(new ElementType[rows*cols]);
			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					data[i*cols + j] = pointcloud[i][j];
				}
			}

			matrix_.setMatrix(data, rows, cols);
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
			rows = pointcloud_.rows;
			cols = pointcloud_.cols;

			points = new ElementType[rows*cols];
			normals = new ElementType[rows*cols];
			colors = new uchar[rows*cols];

			for (size_t i = 0; i < rows; i++) {
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
			rows = pointcloud_.rows;
			cols = pointcloud_.cols;

			points = new ElementType[rows*cols];
			normals = new ElementType[rows*cols];
			colors = new uchar[rows*cols];

			for (size_t i = 0; i < rows; i++) {
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

			rows = pointcloud_.rows;
			cols = pointcloud_.cols;

			points = new ElementType[rows*cols];
			normals = new ElementType[rows*cols];
			colors = new uchar[rows*cols];

			for (size_t i = 0; i < rows; i++) {
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

			rows = pointcloud_.rows;
			cols = pointcloud_.cols;

			points = new ElementType[rows*cols];
			normals = new ElementType[rows*cols];
			colors = new uchar[rows*cols];

			for (size_t i = 0; i < rows; i++) {
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
			rows = 0;
			cols = 0;

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
			subset_.setPointcloud(list_.size(), cols);

			for (size_t i = 0; i < list_.size(); i++) {
				if (list_[i] >= rows) {
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
			subset_.setPointcloud(list_.size(), cols);

			for (size_t i = 0; i < list_.size(); i++) {
				if (list_[i] >= rows) {
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
			size_t number = rows < print_number ? rows : print_number;
			for (size_t i = 0; i < number; i++) {
				for (size_t j = 0; j < cols; j++) {
					out_ << points[i*cols + j] << " ";
				}
				for (size_t j = 0; j < cols; j++) {
					out_ << normals[i*cols + j] << " ";
				}
				for (size_t j = 0; j < cols; j++) {
					out_ << (int) colors[i*cols + j] << " ";
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
		void setNormalsPtr(ElementType* normals_)
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
		void setColorsPtr(uchar* colors_)
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
		void setPointPtr(ElementType* point_, size_t row_)
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
		void setNormalPtr(ElementType* normal_, size_t row_)
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
		void setColorPtr(uchar* color_, size_t row_)
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
		void setPoint(ElementType point_, size_t row_, size_t col_)
		{
			points[row_*cols + col_] = point_;
		}

		/**
			Set normal

			@param[in] normal_ Normal
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setNormal(ElementType normal_, size_t row_, size_t col_)
		{
			normals[row_*cols + col_] = normal_;
		}

		/**
			Set color information

			@param[in] color_ Color information
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setColor(uchar color_, size_t row_, size_t col_)
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
			Get Pointer to point data

			@return Return pointer to point data
		*/
		ElementType* getPointsPtr() const
		{
			ElementType* new_points = new ElementType[rows*cols];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					new_points[i*cols + j] = points[i*cols + j];
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
			ElementType* new_normals = new ElementType[rows*cols];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					new_normals[i*cols + j] = normals[i*cols + j];
				}
			}

			return new_normals;
		}

		/**
			Get Pointer to color information

			@return Return pointer to color information
		*/		
		uchar* getColorsPtr() const
		{
			uchar* new_colors = new uchar[rows*cols];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					new_colors[i*cols + j] = colors[i*cols + j];
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
			ElementType* data(new ElementType[rows*cols]);
			std::memcpy(data, points, sizeof(ElementType)*rows*cols);

			matrix_.setMatrix(data, rows, cols);
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

#endif /* POINTCLOUD_POINTCLOUD_H_ */