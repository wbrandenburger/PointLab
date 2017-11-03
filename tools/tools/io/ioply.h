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
			*iterator_point_ = point;
			iterator_point_++;
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
	};

	class PlyIO 
	{

	public:

		/**
			Constructor
		*/
		PlyIO() : file(nullptr), header(false), instances(0), point_flag(false), normal_flag(false), color_flag(0)
		{
		}

		/**
			Deconstructor
		*/
		~PlyIO() {}

		/**
			Initialize instance
			
			@param[in] file_ Name of file
		*/
		void initialze(char* file_)
		{
			clear();
			file = file_;

			readHeader();

			header = true;
		}

		/**
			Clear
		*/
		void clear()
		{
			file = nullptr;
			header = false;
			instances = 0;
			point_flag = false;
			normal_flag = false;
			color_flag = 0;
		}

		/**
			Get the data type of the points 
		*/
		size_t getDataType() const
		{
			switch (type) {
			case PLY_TYPE_NULL : return 0; break;
			case PLY_TYPE_FLOAT : return 1; break;
			case PLY_TYPE_DOUBLE : return 2; break;
			}

			return 0;
		}

		/**
			Get number of instances
		*/
		size_t getInstances() const
		{
			return instances;
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

			@param[in] type_ Type in rply
			@return Type in this class
		*/
		DataType getDataType(e_ply_type type_) const
		{
			switch (type_) {
			case PLY_FLOAT32 : return PLY_TYPE_FLOAT; break;
			case PLY_FLOAT64 : return PLY_TYPE_DOUBLE; break;
			case PLY_FLOAT : return PLY_TYPE_FLOAT; break;
			case PLY_DOUBLE : return PLY_TYPE_DOUBLE; break;
			}

			return PLY_TYPE_NULL;
		}

		/**
			Sets the meta information

			@param[in] prop_name String of the property
		*/
		void setMetaInformation(const char *prop_name_, DataType type_)
		{
			if (!strcmp(prop_name_, "x")) { type = type_; point_flag = true;}
			if (!strcmp(prop_name_, "nx")) { normal_flag = true;}
			if (!strcmp(prop_name_, "diffuse_red")) { color_flag = 1;}
			if (!strcmp(prop_name_, "red")) { color_flag = 2;}
		}

		/**
			Reads the header and and returns the number of points
		*/
		void readHeader() 
		{
			p_ply ply = ply_open(file, NULL, 0, NULL);
			if (!ply) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}
			
			if (!ply_read_header(ply)) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			p_ply_element elem = NULL;
			elem = ply_get_next_element(ply, elem);

			while (elem) {
				const char *elem_name;
				long elem_instances;
				ply_get_element_info(elem, &elem_name, &elem_instances);
				if (!std::strcmp("vertex", elem_name)) {
					instances = (size_t) elem_instances;
					p_ply_property prop = NULL;
					prop = ply_get_next_property(elem, prop);
					while (prop) {
						const char *prop_name;
						e_ply_type type, length_type, value_type;
						ply_get_property_info(prop, &prop_name, &type, &length_type, &value_type);
						
						setMetaInformation(prop_name,getDataType(type));

						prop = ply_get_next_property(elem, prop);
					}
				}
				elem = ply_get_next_element(ply, elem);
			}

			ply_close(ply);
		}

	private:

		/**
			Callback function which inserts the elements in the pointcloud

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
	public:

		/**
			Read the file and inserts the data in the structure

			@param[in,out] pointcloud Structure in which the elements will be inserted
			@return Returns true if the reading was successful
		*/
		template <typename PointcloudType> bool readPly(PointcloudType& pointcloud_)
		{
			typedef PointcloudType::ElementType ElementType;

			if (!header) {
				exitFailure(__FILE__, __LINE__);
			}

			p_ply ply = ply_open(file, NULL, 0, NULL);

			if (!ply) {
				exitFailure(__FILE__, __LINE__);
			}
			
			if (!ply_read_header(ply)) {
				exitFailure(__FILE__, __LINE__);
			}

			PointcloudIterators<PointcloudType> iterators(pointcloud_);

			if (point_flag) {
				ply_set_read_cb(ply, "vertex", "x", callbackPointcloudIterator<PointcloudType>, &iterators, 1);
				ply_set_read_cb(ply, "vertex", "y", callbackPointcloudIterator<PointcloudType>, &iterators, 1);
				ply_set_read_cb(ply, "vertex", "z", callbackPointcloudIterator<PointcloudType>, &iterators, 1);
			}
			else{
				exitFailure(__FILE__, __LINE__);
			}

			if (color_flag == 1) {
				ply_set_read_cb(ply, "vertex", "diffuse_red", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
				ply_set_read_cb(ply, "vertex", "diffuse_green", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
				ply_set_read_cb(ply, "vertex", "diffuse_blue", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
			}

			if (color_flag == 2) {
				ply_set_read_cb(ply, "vertex", "red", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
				ply_set_read_cb(ply, "vertex", "green", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
				ply_set_read_cb(ply, "vertex", "blue", callbackPointcloudIterator<PointcloudType>, &iterators, 2);
			}

			if (normal_flag) {
				ply_set_read_cb(ply, "vertex", "nx", callbackPointcloudIterator<PointcloudType>, &iterators, 3);
				ply_set_read_cb(ply, "vertex", "ny", callbackPointcloudIterator<PointcloudType>, &iterators, 3);
				ply_set_read_cb(ply, "vertex", "nz", callbackPointcloudIterator<PointcloudType>, &iterators, 3);
			}

		
			if (!ply_read(ply)) {
				return 0;
			}
		
			ply_close(ply);
		
			return true;
		}

		/**
			Write ply file with given structure

			@param[in] file_ Name of file
			@param pointcloud_ Structure which elements will be written in the file
			@return Returns true if writing was successful
		*/
		template <typename ElementType> bool writePly(char *file_, pointcloud::Pointcloud<ElementType>& pointcloud_)
		{
			p_ply ply = ply_create(file_, PLY_DEFAULT, NULL, 0, NULL);
		
			ply_add_element(ply, "vertex", (long)pointcloud_.rows);
		
			e_ply_type ply_data_type = type == PLY_TYPE_FLOAT ? PLY_FLOAT32 : PLY_FLOAT64;
			if (point_flag) {
				ply_add_property(ply, "x", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "y", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "z", ply_data_type, ply_data_type, ply_data_type);
			}
			else {
				exitFailure(__FILE__, __LINE__);
			}

			if (normal_flag) {
				ply_add_property(ply, "nx", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "ny", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "nz", ply_data_type, ply_data_type, ply_data_type);
			}
		
			if (color_flag) {
				e_ply_type ply_color_type = PLY_UCHAR;
				ply_add_property(ply, "red", ply_color_type, ply_color_type, ply_color_type);
				ply_add_property(ply, "green", ply_color_type, ply_color_type, ply_color_type);
				ply_add_property(ply, "blue", ply_color_type, ply_color_type, ply_color_type);
			}

			ply_write_header(ply);
		
			for (int i = 0; i < pointcloud_.rows; i++) {
				ElementType* point = pointcloud_.getPointPtr(i);
				ply_write(ply, point[0]);
				ply_write(ply, point[1]);
				ply_write(ply, point[2]);
				
				if (normals) {
					ElementType* normal = pointcloud_.getNormalPtr(i);
					ply_write(ply, normal[0]);
					ply_write(ply, normal[1]);
					ply_write(ply, normal[2]);
				}

				if (colors) {
					uint8_t* color = pointcloud_.getColorPtr(i);
					ply_write(ply, color[0]);
					ply_write(ply, color[1]);
					ply_write(ply, color[2]);
				}
			}
		
			ply_close(ply);
		
			return true;
		}

		/**
			Returns true if points are set

			@return True if points are set
		*/
		bool isPoints() const
		{
			return point_flag;
		}

		/**
			Returns true if colors are set

			@return True if colors are set 
		*/
		bool isColor() const
		{
			return (bool)color_flag;
		}

		/**
			Returns true if normals are set

			@return True if normals are set 
		*/
		bool isNormal() const
		{
			return normal_flag;
		}

	private:
		/**
			Filename
		*/
		char* file;

		/**
			Flag which determines whether the header was read
		*/
		bool header;

		/**
			Data type of the pointcloud
		*/
		DataType type;

		/** 
			Instances of vertex
		*/
		size_t instances;

		/**
			Flag whether points are contained in file
		*/
		bool point_flag;

		/**
			Flag whether normals are contained in file
		*/
		bool normal_flag;
		
		/**
			Flag whether colors are contained in file
		*/
		size_t color_flag;
	};
}

#endif /* IO_IOPLY_H_ */