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
#include <vector>

#include "tools/parameters.h"

#include "tools/utils/matrix.h"

#include "tools/pointcloud/pointcloudnodes.h"

namespace pointcloud
{
	/** 
		Providing a class which stores pointclouds:
		-	point information
		-	color information
		-	normal information
		-	triangles
		
		There are two derived classes of the abstract class pointcloud
		-	PointcloudAoS, which stores the information as an array of structures
		-	PointcloudSoA, which stores the information as several arrays of structures
		The triangles are stored separately.

		An instance of a pointcloud can be called with
			- pointcloud::Pointcloud<ElementType> pointcloud()
			- pointcloud::Pointcloud<ElementType> pointcloud(flags)
			- pointcloud::Pointcloud<ElementType> pointcloud(number_of_vertices, flags)
			- pointcloud::Pointcloud<ElementType> pointcloud(number_of_vertices, number_of_triangles, flags)
		The flags specify which information has to be stored in the pointcloud instance:
			- Vertex::RGB
			- Vertex::NORMALS
			- Vertex::TRIANGLES
			- Vertex::RGBNORMALS
			- Vertex::MESH
		It is assumed that points are always stored in the pointcloud.

		To set the arrays in the pointlcoud you have to call
			- pointcloud.setPointcloud()
			- pointcloud.setPointcloud(number_of_vertices)
			- pointcloud.setPointcloud(number_of_vertices, number_of_triangles)
			
	*/

	template<typename ElementType> class Pointcloud 
	{
	
	public:

		typedef ElementType ElementType;

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

			number_of_vertices = number_of_vertices_;
			number_of_triangles = number_of_triangles_;
		}

		/**
			Set the flags
		*/
		void setFlags(uint8_t flags_)
		{
			color_flag = (flags_ & PointcloudFlag::RGB) > 0 || (flags_ & PointcloudFlag::RGBNORMALS) > 0 || (flags_ & PointcloudFlag::MESH) > 0;
			normal_flag = (flags_ & PointcloudFlag::NORMALS) > 0 || (flags_ & PointcloudFlag::RGBNORMALS) > 0 || (flags_ & PointcloudFlag::MESH) > 0;
			triangle_flag = (flags_ & PointcloudFlag::TRIANGLES) > 0 || (flags_ & PointcloudFlag::MESH) > 0;
		}

	protected:
		/**
			Allocte the meory for the triangles
		*/
		void allocateMemoryTriangles()
		{
			if (isTriangle()) {
				triangles = new size_t[number_of_triangles * 3];
				memset(triangles, (size_t) 0, sizeof(size_t) * number_of_triangles * 3);
			}
		}
	
	public:
		/**
			Set pointcloud
		*/

		virtual void setPointcloud() = 0;
		
		/**
			Set color flag and allocate memory
		*/
		virtual void setColors() = 0;

		/**
			Set normal flag and allocate memory
		*/
		virtual void setNormals() = 0;
		
		/**
			Set triangles
		*/
		void setTriangles()
		{
			triangle_flag = true;

			clearMemoryTriangles();

			allocateMemoryTriangles();
		}

		/**
			Set triangles

			@param[in] number_of_triangles_ Number of triangles
		*/
		void setTriangles(size_t number_of_triangles_)
		{
			triangle_flag = true;

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
		void getSubset(const std::vector<size_t>& list, Pointcloud<ElementType>& subset) const
		{
			getSubset(list.data(), list.size(), subset);
		}

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in] number_of_elements_in_list_ Number of elements in list
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(size_t* list,size_t number_of_elements, Pointcloud<ElementType>& subset) const
		{
			uint8_t flags = 0;
			if (isColor()) {
				flags |= PointcloudFlag::RGB;
			}
			if (isNormal()) {
				flags |= PointcloudFlag::NORMALS;
			}
			subset.setFlags(flags);
			subset.setNumberOfVertices(number_of_elements);
			subset.setPointcloud();

			for (size_t i = 0; i < number_of_elements; i++) {
				if (list[i] >= number_of_vertices) {
					exitFailure(__FILE__, __LINE__);
				}
				subset.setPointPtr(getPointPtr(list[i]),i);
				if (isColor()) {
					subset.setColorPtr(getColorPtr(list[i]), i);
				}
				if (isNormal()) {
					subset.setNormalPtr(getNormalPtr(list[i]), i);
				}
			}
		}

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(const std::vector<size_t>& list, utils::Matrix<ElementType>& subset) const
		{
			getSubset(list.data(), list.size(), subset);
		}

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in] number_of_elements_in_list_ Number of elements in list
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(size_t* list,size_t number_of_elements, utils::Matrix<ElementType>& subset) const
		{
			for (size_t i = 0; i <  number_of_elements; i++) {
				if (list[i] >= number_of_vertices) {
					exitFailure(__FILE__, __LINE__);
				}
				std::memcpy(subset[i],getPointPtr(list[i]), sizeof(ElementType)*3);
			}
		}

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(const std::vector<size_t>& list_, ElementType* subset_) const
		{
			getSubset(list_.data(), list_.size(), subset_);
		}

		/**
			Generate a subset of the pointcloud

			@param[in] list_ List with indices to elements of the subset
			@param[in] number_of_elements_in_list_ Number of elements in list
			@param[in,out] subset_ Reference to the pointcloud with the subset
		*/
		void getSubset(size_t* list_,size_t number_of_elements_in_list_, ElementType* subset_) const
		{
			for (size_t i = 0; i <  number_of_elements_in_list_; i++) {
				if (list_[i] >= number_of_vertices) {
					exitFailure(__FILE__, __LINE__);
				}
				std::memcpy(subset_[i*3],getPointPtr(list_[i]), sizeof(ElementType)*3);
			}
		}

		/**
			Set points

			@param[in] points_ Points
		*/
		void setPointsPtr(ElementType* points_)
		{
			for (Iterator<ElementType> it = beginPoint(); it != endPoint(); it++) {
				*it = *points_;
				points_++;
			}
		}

		/**
			Set normals

			@param[in] normals_ Normals
		*/
		void setNormalsPtr(ElementType* normals_)
		{
			for (Iterator<ElementType> it = beginNormal(); it != endNormal(); it++) {
				*it = *normals_;
				normals_++;
			}
		}

		/**
			Set colors

			@param[in] colors_ Colors
		*/
		void setColorsPtr(uint8_t* colors_)
		{
			for (Iterator<uint8_t> it = beginColor(); it != endColor(); it++) {
				*it = *colors_;
				colors_++;
			}
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
				*it = (size_t) triangles_;
				triangles_++;
			}
		}

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
			Set triangle

			@param[in] triangle_ Triangle
			@param[in] row_ Row
		*/
		void setTrianglePtr(size_t* triangle_, size_t row_)
		{
			std::memcpy(&triangles[row_ * 3], triangle_, sizeof(size_t) * 3);
		}

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
			Set triangle

			@param[in] triangle_ Triangle
			@param[in] row_ Row
			@param[in] col_ Col
		*/
		void setTriangle(size_t triangle, size_t row_, size_t col_)
		{
			triangles[row_ * 3 + col_] = triangle;
		}

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
			Get triangle data of specified index

			@param[in] row_ Row
			@param[in] col_ Col
			@return Return triangle data of specified index
		*/
		size_t getTriangle(size_t row_, size_t col_) const
		{
			return triangles[row_ * 3 + col_];
		}

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
			Get Pointer to triangle

			@param[in] row_ Row
			@return Return pointer to triangle
		*/	
		size_t* getTrianglePtr(size_t row_) const
		{
			return &triangles[row_ * 3]
		}

		/**
			Get Pointer to point data

			@return Return pointer to point data
		*/
		ElementType* getPointsPtr() const
		{
			ElementType* new_points = new ElementType[number_of_vertices * 3];
			ElementType* new_points_ptr = new_points;

			for (Iterator<ElementType> it = beginPoint(); it != endPoint(); it++) {
				*new_points_ptr = *it;
				new_points_ptr++;
			}

			return new_points;
		}

		/**
			Get Pointer to point data

			@param[in] new_points Pointer to point data
		*/
		template<typename ReturnType> ReturnType* getPointsPtr() const
		{
			ReturnType* new_points = new ReturnType[number_of_vertices * 3];
			ReturnType* new_points_ptr = new_points;

			for (Iterator<ElementType> it = beginPoint(); it != endPoint(); it++){
				*new_points_ptr = (ReturnType)*it;
				new_points_ptr++;
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
			ElementType* new_normals_ptr = new_normals;

			for (Iterator<ElementType> it = beginNormal(); it != endNormal(); it++) {
				*new_normals_ptr = *it;
				new_normals_ptr++;
			}

			return new_normals;
		}

		/**
			Get Pointer to the normal

			@return Return pointer to the normal
		*/
		template<typename ReturnType> ReturnType* getNormalsPtr() const
		{
			ReturnType* new_normals = new ReturnType[number_of_vertices * 3];
			ReturnType* new_normals_ptr = new_normals;

			for (Iterator<ElementType> it = beginNormal(); it != endNormal(); it++) {
				*new_normals_ptr = (ReturnType)*it;
				new_normals_ptr++;
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
			uint8_t* new_colors_ptr = new_colors;

			for (Iterator<uint8_t> it = beginColor(); it != endColor(); it++) {
				*new_colors_ptr = *it;
				new_colors_ptr++;
			}

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
			Get Pointer to color information

			@return Return pointer to color information
		*/		
		template<typename ReturnType> ReturnType* getColorsPtr() const
		{
			ReturnType* new_colors = new ReturnType[number_of_vertices * 3];
			ReturnType* new_colors_ptr = new_colors;

			for (Iterator<uint8_t> it = beginColor(); it != endColor(); it++) {
				*new_colors_ptr = (ReturnType)*it;
				new_colors_ptr++;
			}

			return new_colors;
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

			@return Return pointer to the triangles
		*/
		template<typename ReturnType> ReturnType* getTrianglesPtr() const
		{
			ReturnType* new_triangles = new ReturnType[number_of_triangles * 3];
			ReturnType* new_triangles_ptr = new_triangles;

			for (Iterator<size_t> it = beginTriangle(); it != endTriangle(); it++) {
				*new_triangles_ptr = (ReturnType)*it;
				new_triangles_ptr++;
			}

			return new_triangles;
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
			Structure for initialization of the iterators
		*/
		template<typename IteratorType> class IteratorInitializer
		{
		public:
			/**
				Constructor

				@param[in] begin Pointer to the first element
				@param[in] pointcloud_flag Specifier which defines the element to be iterated
			*/
			IteratorInitializer(IteratorType* begin, PointcloudFlag pointcloud_flag, PointcloudType pointcloud_type) :
				begin_(begin), pointcloud_flag_(pointcloud_flag), pointcloud_type_(pointcloud_type)
			{
			}

			/**
				Destructor
			*/
			~IteratorInitializer()
			{
			}

			/**
				Get the pointer to the first element

				@return Pointer to the first element
			*/
			IteratorType* getBegin() const 
			{ 
				return begin_;
			}

			/**
				Get the element specifier

				@return the element specifier
			*/
			PointcloudFlag getPointcloudFlag() const
			{
				return pointcloud_flag_;
			}

			/**
				Get the type of pointcloud container
			*/
			PointcloudType getPointcloudType() const
			{
				return pointcloud_type_;
			}

		private:
			/**
				Pointer to the first element of the array
			*/
			IteratorType* begin_;

			/**
				Specifies the element to be iterated 
			*/
			PointcloudFlag pointcloud_flag_;

			/**
				Specify the type of pointcloud container
			*/
			PointcloudType pointcloud_type_;
		};

	public:
		/**
			Returns a pointer to the first entry of the points

			@return Pointer to the first entry of the points
		*/
		virtual IteratorInitializer<ElementType> beginPoint() const = 0;

		/**
			Returns a pointer to the first entry of colors

			@return Pointer to the first entry of colors
		*/
		virtual IteratorInitializer<uint8_t> beginColor() const = 0;
				
		/**
			Returns a pointer to the first entry of normals

			@return Pointer to the first entry of normals
		*/
		virtual IteratorInitializer<ElementType> beginNormal() const = 0;

		/**
			Returns a pointer to the first entry of triangles

			@return Pointer to the first entry of triangles
		*/
		IteratorInitializer<size_t> beginTriangle() const
		{
			return IteratorInitializer<size_t>(triangles,PointcloudFlag::TRIANGLES, PointcloudType::NONE);
		};

		/**
			Returns a pointer to the last entry + 1 of the points

			@return Pointer to the last entry + 1 of the points
		*/
		virtual ElementType* endPoint() const = 0;

		/**
			Returns a pointer to the last entry + 1 of colors

			@return Pointer to the last entry + 1 of colors
		*/
		virtual uint8_t* endColor() const = 0;

		/**
			Returns a pointer to the last entry + 1 of normals

			@return Pointer to the last entry + 1 of normals
		*/
		virtual ElementType* endNormal() const = 0;
	
		/**
			Returns a pointer to the last entry + 1 of triangles

			@return Pointer to the last entry + 1 of triangles
		*/
		size_t* endTriangle() const
		{
			return triangles + number_of_triangles * 3;
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
			Iterator() : iterator_(nullptr), stride_(0), index_(0), 
				pointcloud_type_(PointcloudType::NONE)
			{
			}
			
			/**
				Constructor

				@param[in] iterator_initializer Structure for initialization of the iterators
			*/
			Iterator(const IteratorInitializer<IteratorType>& iterator_initializer) : Iterator()
			{
				iterator_ = iterator_initializer.getBegin();

				pointcloud_type_ = iterator_initializer.getPointcloudType();

				setStride(iterator_initializer.getPointcloudFlag());
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
				
				@param[in] iterator_initializer Structure for initialization of the iterators
			*/
			Iterator& operator=(const IteratorInitializer<IteratorType>& iterator_initializer)
			{
				iterator_ = iterator_initializer.getBegin();

				pointcloud_type_ = iterator_initializer.getPointcloudType();
				
				setStride(iterator_initializer.getPointcloudFlag());

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
				if (pointcloud_type_ == PointcloudType::AOS && index_ == 2) {
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
				Set stride

				@param[in] pointcloud_flag Specifies the element to be iterated 
			*/
			void setStride(const PointcloudFlag& pointcloud_flag)
			{
				if (pointcloud_type_ == PointcloudType::AOS) {
					switch (pointcloud_flag) {
					case PointcloudFlag::POINTS: stride_ = sizeof(ElementType) * 4 + 4; break;
					case PointcloudFlag::RGB: stride_ = sizeof(ElementType) * 6 + 2; break;
					case PointcloudFlag::NORMALS: stride_ = sizeof(ElementType) * 4 + 4; break;
					}
				}
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
				Current column
			*/
			size_t index_;

			/**
				Specify the type of pointcloud container
			*/
			PointcloudType pointcloud_type_;
		};

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
		void setNormalFlag()
		{
			normal_flag = true;
		}

		/**
			Set triangle flag 
		*/
		void setTriangleFlag()
		{
			triangle_flag = true;
		}
	
	public:
		/**
			Get Pointcloud flag

			@return Pointcloud flag
		*/
		uint8_t getPointcloudFlag() const
		{
			uint8_t flags = 0;
			if (isColor()) {
				flags |= PointcloudFlag::RGB;
			}
			if (isNormal()) {
				flags |= PointcloudFlag::NORMALS;
			}
			if (isTriangle()) {
				flags |= PointcloudFlag::TRIANGLES;
			}

			return flags;
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

		for (size_t i = 0; i < number; i++) {
			out_ << pointcloud_.getPoint(i, 0) << " " << pointcloud_.getPoint(i, 1) << " " << pointcloud_.getPoint(i, 2) << " ";

			if (pointcloud_.isColor()) {
				out_ << (int)pointcloud_.getColor(i, 0) << " " << (int)pointcloud_.getColor(i, 1) << " " << (int)pointcloud_.getColor(i, 2) << " ";
			}

			if (pointcloud_.isNormal()) {
				out_ << pointcloud_.getNormal(i, 0) << " " << pointcloud_.getNormal(i, 1) << " " << pointcloud_.getNormal(i, 2) << " ";
			} 
			
			out_ << std::endl;
		}

		return out_;
	}
}

#endif /* POINTCLOUD_POINTCLOUD_H_ */