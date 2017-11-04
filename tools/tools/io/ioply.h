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

#ifndef IO_IOPLY_H_
#define IO_IOPLY_H_

#include <rply.h>

#include "tools/parameters.h"

#include "pointcloud/pointcloud.h"

namespace io
{
	template<typename PointcloudType> struct PointcloudIterators
	{
	public:

		typedef typename PointcloudType::ElementType ElementType;

		typedef typename PointcloudType::template Iterator<ElementType> IteratorElementType;
		
		typedef typename PointcloudType::template Iterator<uint8_t> Iteratoruint8_t;
		
		typedef typename PointcloudType::template Iterator<size_t> Iteratorsize_t;

		/**
			Constructor 

			@param[in] pointcloud Pointcloud
		*/
		PointcloudIterators(const PointcloudType& pointcloud)
		{
			iterator_point_ = pointcloud.beginPoint();
			if (pointcloud.isColor()) {
				iterator_color_ = pointcloud.beginColor();
			}
			if (pointcloud.isNormal()) {
				iterator_normal_ = pointcloud.beginNormal();
			}
			if (pointcloud.isTriangle()) {
				iterator_triangle_ = pointcloud.beginTriangle();
			}

			end_ = pointcloud.endPoint();
		}

		/**
			Destructor
		*/
		~PointcloudIterators()
		{
		}
		/**
			Copy constructor

			@param[in] class_ An instance of class PointcloudIterators
		*/
		PointcloudIterators(const PointcloudIterators<PointcloudType>& pointcloud_iterators) = delete;

		/**
			Copy constructor
	
			@param[in] class_ An instance of class PointcloudIterators
		*/
		PointcloudIterators(const PointcloudIterators<PointcloudType>&& pointcloud_iterators) = delete;
	
		/**
			Operator =
	
			@param[in] class_ An instance of class PointcloudIterators
			@return Returns reference to the current instance
		*/
		PointcloudIterators& operator=(const PointcloudIterators<PointcloudType>& pointcloud_iterators) = delete;
	
		/**
			Operator =
		
			@param[in] glview_ An instance of class PointcloudIterators
			@return Returns reference to the current instance
		*/
		PointcloudIterators& operator=(const PointcloudIterators<PointcloudType>&& pointcloud_iterators) = delete;

		/**
			Set iterators

			@param[in] pointcloud Pointcloud
		*/
		void setIterators(const PointcloudType& pointcloud)
		{
			iterator_point_ = pointcloud.beginPoint();
			if (pointcloud.isColor()) {
				iterator_color_ = pointcloud.beginColor();
			}
			if (pointcloud.isNormal()) {
				iterator_normal_ = pointcloud.beginNormal();
			}
			if (pointcloud.isTriangle()) {
				iterator_triangle_ = pointcloud.beginTriangle();
			}
		}

		/**
			Set a point and increment pointer
		*/
		void setPoint(ElementType point) 
		{
			*iterator_point_ = point;
			iterator_point_++;
		}

		/**
			Set color and increment pointer
		*/
		void setColor(uint8_t color) 
		{
			*iterator_color_ = color;
			iterator_color_++;
		}

		/**
			Set a normal and increment pointer
		*/
		void setNormal(ElementType normal) 
		{
			*iterator_normal_ = normal;
			iterator_normal_++;
		}

		/**
			Set a triangle and increment pointer
		*/
		void setTriangle(size_t triangle) 
		{
			*iterator_triangle_ = triangle;
			iterator_triangle_++;
		}

		/**
			Get point and increment pointer
		*/
		ElementType getPoint() 
		{
			ElementType point_temp = *iterator_point_;
			iterator_point_++;

			return point_temp;
		}

		/**
			Get color and increment pointer
		*/
		uint8_t getColor() 
		{
			uint8_t color_temp = *iterator_color_;
			iterator_color_++;

			return color_temp;
		}

		/**
			Get normal and increment pointer
		*/
		ElementType getNormal() 
		{
			ElementType normal_temp = *iterator_normal_;
			iterator_normal_++;

			return normal_temp;
		}

		/**
			Get triangle and increment pointer
		*/
		size_t getTriangle() 
		{
			size_t triangle_temp = *iterator_triangle_;
			iterator_triangle_++;

			return triangle_temp;
		}

		/**
			Checks whether the end is reached
		*/
		bool end()
		{
			return iterator_point_ == end_;
		}
	private:

		/**
			Point iterator
		*/
		IteratorElementType iterator_point_;

		/**
			Color iterator
		*/
		Iteratoruint8_t iterator_color_;

		/**
			Normal iterator
		*/
		IteratorElementType iterator_normal_;

		/**
			Triangle iterator
		*/
		Iteratorsize_t iterator_triangle_;

		/**
			End of data
		*/
		ElementType* end_;
	};

	class PlyIO 
	{

	public:

		/**
			Constructor
		*/
		PlyIO() : file_(nullptr), number_of_vertices_(0), number_of_triangles_(0), 
			normal_flag_(false), color_flag_(0), triangle_flag_(false), pointcloud_flag_(0)
		{
		}

		/**
			Deconstructor
		*/
		~PlyIO() {}

		/**
			Initialize instance
			
			@param[in] file Name of file
		*/
		void initialze(char* file)
		{
			reset();
			file_ = file;

			readHeader();
		}

		/**
			Clear
		*/
		void reset()
		{
			file_ = nullptr;
			number_of_vertices_ = 0;
			number_of_triangles_ = 0;
			normal_flag_ = false;
			color_flag_ = 0;
			triangle_flag_ = false;
		}

		/**
			Get the data type of the pointcloud 

			@return Type of the pointcloud
		*/
		size_t getDataType() const
		{
			switch (type_) {
			case PLY_TYPE_NULL : return 0; break;
			case PLY_TYPE_FLOAT : return 1; break;
			case PLY_TYPE_DOUBLE : return 2; break;
			}

			return 0;
		}

		/**
			Get number of vertices

			@return Number of vertices
		*/
		size_t getNumberOfVertices() const
		{
			return number_of_vertices_;
		}

		/**
			Get number of Triangles

			@return Number of triangles
		*/
		size_t getNumberOfTriangles() const
		{
			return number_of_vertices_;
		}

	private:

		/**
			Type of pointcloud
		*/
		enum DataType
		{
			PLY_TYPE_NULL = 0,
			PLY_TYPE_FLOAT = 1,
			PLY_TYPE_DOUBLE = 2
		};

		/**
			Get the data type of the pointcloud

			@param[in] type Type in rply
			@return Type in this class
		*/
		DataType getDataType(e_ply_type type) const
		{
			switch (type) {
			case PLY_FLOAT32 : return PLY_TYPE_FLOAT; break;
			case PLY_FLOAT64 : return PLY_TYPE_DOUBLE; break;
			case PLY_FLOAT : return PLY_TYPE_FLOAT; break;
			case PLY_DOUBLE : return PLY_TYPE_DOUBLE; break;
			}

			return PLY_TYPE_NULL;
		}

		/**
			Reads the header and and returns the number of points
		*/
		void readHeader() 
		{
			

			p_ply ply = ply_open(file_, NULL, 0, NULL);
			if (!ply) {
				exitFailure(__FILE__, __LINE__);
			}
			
			if (!ply_read_header(ply)) {
				exitFailure(__FILE__, __LINE__);
			}

			/**
				Get the next element in the header
			*/
			p_ply_element elem = NULL;
			elem = ply_get_next_element(ply, elem);

			/**
				Iterate over all elements in the header
			*/
			while (elem) {
				/**
					Get information of the current element
				*/
				const char *elem_name;
				long elem_instances;
				ply_get_element_info(elem, &elem_name, &elem_instances);
				
				/**
					Process the vertices
				*/
				if (std::strcmp("vertex", elem_name) == 0) {
					number_of_vertices_ = (size_t) elem_instances;

				}
				if (std::strcmp("face", elem_name) == 0) {
					number_of_triangles_ = (size_t)elem_instances;
					triangle_flag_ = true; pointcloud_flag_ |= PointcloudFlag::TRIANGLES;
				}
				/**
					Get the next property
				*/
				p_ply_property prop = NULL;
				prop = ply_get_next_property(elem, prop);
				
				/**
					Iterate over all properties of the current element
				*/
				while (prop) {
					/**
						Get information of the current property
					*/
					const char *prop_name;
					e_ply_type type, length_type, value_type;
					ply_get_property_info(prop, &prop_name, &type, &length_type, &value_type);
					
					if (!strcmp(prop_name, "x")) { type_ = getDataType(type); }
					if (!strcmp(prop_name, "nx")) { normal_flag_ = true; pointcloud_flag_ |= PointcloudFlag::NORMALS; }
					if (!strcmp(prop_name, "diffuse_red")) { color_flag_ = 1; pointcloud_flag_ |= PointcloudFlag::RGB; }
					if (!strcmp(prop_name, "red")) { color_flag_ = 2; pointcloud_flag_ |= PointcloudFlag::RGB; }

					prop = ply_get_next_property(elem, prop);
				}

				/**
					Get the next element in the header
				*/
				elem = ply_get_next_element(ply, elem);
			}

			ply_close(ply);
		}

	private:

		/**
			Callback function which inserts points, color and normals in the pointcloud

			@param[in] argument Argument which contains the structure in which the elements will be inserted
			@return Returns true if insertion was successful
		*/
		template <typename PointcloudType> static int callbackPointcloudIterator(p_ply_argument argument_)
		{
			typedef PointcloudType::ElementType ElementType;

			long index;
			PointcloudIterators<PointcloudType>* iterator;

			ply_get_argument_user_data(argument_, (void**)&iterator, &index);
		
			switch (index) {
			case 1:
				iterator->setPoint((ElementType)ply_get_argument_value(argument_));
				break;
			case 2:
				iterator->setColor((uint8_t)ply_get_argument_value(argument_));
				break;
			case 3:
				iterator->setNormal((ElementType)ply_get_argument_value(argument_));
				break;
			default:
				break;
			}
		
			return 1;
		}	

		/**
			Callback function which inserts triangles in the pointcloud

			@param[in] argument Argument which contains the structure in which the elements will be inserted
			@return Returns true if insertion was successful
		*/
		template <typename PointcloudType> static int callbackFaceIterator(p_ply_argument argument_)
		{
			long length, value_index, index;
			ply_get_argument_property(argument_, NULL, &length, &value_index);
		

			PointcloudIterators<PointcloudType>* iterator;
			ply_get_argument_user_data(argument_, (void**)&iterator, &index);

			if (value_index != -1) {
				iterator->setTriangle((size_t)ply_get_argument_value(argument_));
			}

			return 1;
		}	

	public:

		/**
			Read the file and inserts the data in the structure

			@param[in,out] pointcloud Structure in which the elements will be inserted
			@return Returns true if the reading was successful
		*/
		template <typename PointcloudType> bool readPly(PointcloudType& pointcloud_)
		{
			typedef PointcloudType::ElementType ElementType;

			p_ply ply = ply_open(file_, NULL, 0, NULL);

			if (!ply) {
				exitFailure(__FILE__, __LINE__);
			}
			
			if (!ply_read_header(ply)) {
				exitFailure(__FILE__, __LINE__);
			}

			PointcloudIterators<PointcloudType> iterators(pointcloud_);

			
			ply_set_read_cb(ply, "vertex", "x", callbackPointcloudIterator<PointcloudType>, &iterators, 1);
			ply_set_read_cb(ply, "vertex", "y", callbackPointcloudIterator<PointcloudType>, &iterators, 1);
			ply_set_read_cb(ply, "vertex", "z", callbackPointcloudIterator<PointcloudType>, &iterators, 1);
			

			if ( (pointcloud_flag_ & PointcloudFlag::RGB) > 0 && color_flag_ == 1) {
				ply_set_read_cb(ply, "vertex", "diffuse_red", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
				ply_set_read_cb(ply, "vertex", "diffuse_green", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
				ply_set_read_cb(ply, "vertex", "diffuse_blue", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
			}

			if ((pointcloud_flag_ & PointcloudFlag::RGB) > 0  && color_flag_ == 2) {
				ply_set_read_cb(ply, "vertex", "red", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
				ply_set_read_cb(ply, "vertex", "green", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
				ply_set_read_cb(ply, "vertex", "blue", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
			}

			if ((pointcloud_flag_ & PointcloudFlag::NORMALS) > 0) {
				ply_set_read_cb(ply, "vertex", "nx", callbackPointcloudIterator<PointcloudType>, &iterators, 3);
				ply_set_read_cb(ply, "vertex", "ny", callbackPointcloudIterator<PointcloudType>, &iterators, 3);
				ply_set_read_cb(ply, "vertex", "nz", callbackPointcloudIterator<PointcloudType>, &iterators, 3);
			}

			//if (triangle_flag_) {
			//	ply_set_read_cb(ply, "face", "vertex_indices", callbackFaceIterator<PointcloudType>, &iterators, 0);
			//}

			if (!ply_read(ply)) {
				return 0;
			}
		
			ply_close(ply);
		
			return true;
		}

		/**
			Write ply file with given structure

			@param[in] file Name of file
			@param pointcloud_ Structure which elements will be written in the file
			@return Returns true if writing was successful
		*/
		template <typename PointcloudType> bool writePly(char *file, const PointcloudType& pointcloud_)
		{
			p_ply ply = ply_create(file, PLY_DEFAULT, NULL, 0, NULL);
		
			ply_add_element(ply, "vertex", (long)pointcloud_.getNumberOfVertices());
		
			e_ply_type ply_data_type = type_ == PLY_TYPE_FLOAT ? PLY_FLOAT32 : PLY_FLOAT64;
			
			ply_add_property(ply, "x", ply_data_type, ply_data_type, ply_data_type);
			ply_add_property(ply, "y", ply_data_type, ply_data_type, ply_data_type);
			ply_add_property(ply, "z", ply_data_type, ply_data_type, ply_data_type);

			if ((pointcloud_flag_ & PointcloudFlag::RGB) > 0) {
				e_ply_type ply_color_type = PLY_UCHAR;
				ply_add_property(ply, "red", ply_color_type, ply_color_type, ply_color_type);
				ply_add_property(ply, "green", ply_color_type, ply_color_type, ply_color_type);
				ply_add_property(ply, "blue", ply_color_type, ply_color_type, ply_color_type);
			}

			if ((pointcloud_flag_ & PointcloudFlag::NORMALS) > 0) {
				ply_add_property(ply, "nx", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "ny", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "nz", ply_data_type, ply_data_type, ply_data_type);
			}

			ply_write_header(ply);
		
			PointcloudIterators<PointcloudType> iterators(pointcloud_);

			while (!iterators.end()) {
				ply_write(ply, iterators.getPoint());
				ply_write(ply, iterators.getPoint());
				ply_write(ply, iterators.getPoint());

				if ((pointcloud_flag_ & PointcloudFlag::RGB) > 0) {
					ply_write(ply, iterators.getColor());
					ply_write(ply, iterators.getColor());
					ply_write(ply, iterators.getColor());
				}
				if ((pointcloud_flag_ & PointcloudFlag::NORMALS) > 0) {
					ply_write(ply, iterators.getNormal());
					ply_write(ply, iterators.getNormal());
					ply_write(ply, iterators.getNormal());
				}
			}
		
			ply_close(ply);
		
			return true;
		}

	private:
		/**
			Filename
		*/
		char* file_;

		/**
			Data type of the pointcloud
		*/
		DataType type_;

		/** 
			Number of vertices
		*/
		size_t number_of_vertices_;

		/**
			Number of triangles
		*/
		size_t number_of_triangles_;

		/**
			Flag whether normals are inside the file
		*/
		bool normal_flag_;
		
		/**
			Flag whether colors are inside the file
		*/
		size_t color_flag_;

		/**
			Flag whether triangles are inside the file
		*/
		bool triangle_flag_;

		uint8_t pointcloud_flag_;
	};
}

#endif /* IO_IOPLY_H_ */