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


#ifndef IO_IOPLY_H_
#define IO_IOPLY_H_

#include <rply.h>

#include "utils/matrix.h"
#include "pointcloud/pointcloud.h"

namespace io
{
	typedef unsigned char uchar;

	/**
		Static counter for reading points
	*/
	static size_t counter;

	/**
		After which element counter has to be incremented
	*/
	static size_t point_break;

	class PlyIO 
	{

	public:

		/**
			Constructor
		*/
		PlyIO() : file(nullptr), header(false), instances(0), points(0), normals(0), colors(0) 
		{
			point_break = 0;
			counter =  0;
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
			point_break = 0;
			instances = 0;
			points = 0;
			normals = 0;
			colors = 0;
			counter = 0;
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
			if (!strcmp(prop_name_, "x")) { type = type_; points = 1;}
			if (!strcmp(prop_name_, "z")) { point_break = 3;}
			if (!strcmp(prop_name_, "nx")) { normals = 1;}
			if (!strcmp(prop_name_, "nz")) { point_break = 6; }
			if (!strcmp(prop_name_, "diffuse_red")) { colors = 1;}
			if (!strcmp(prop_name_, "diffuse_blue")) { point_break = 9; }
			if (!strcmp(prop_name_, "red")) { colors = 2;}
			if (!strcmp(prop_name_, "blue")) { point_break = 9;}
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
		template <typename ElementType> static int callbackPointcloud(p_ply_argument argument_)
		{
			long index;
			trees::Pointcloud<ElementType> *pointcloud;
			ply_get_argument_user_data(argument_, (void**)&pointcloud, &index);
		
			switch (index) {
			case 1:
				(*pointcloud).setPoint((ElementType)ply_get_argument_value(argument_), counter, 0);
				break;
			case 2:
				(*pointcloud).setPoint((ElementType)ply_get_argument_value(argument_), counter, 1);
				break;
			case 3:
				(*pointcloud).setPoint((ElementType)ply_get_argument_value(argument_), counter, 2);
				if (point_break == 3) { counter++; }
				break;
			case 4:
				(*pointcloud).setNormal((ElementType)ply_get_argument_value(argument_), counter, 0);
				break;
			case 5:
				(*pointcloud).setNormal((ElementType)ply_get_argument_value(argument_), counter, 1);
				break;
			case 6:
				(*pointcloud).setNormal((ElementType)ply_get_argument_value(argument_), counter, 2);
				if (point_break == 6) { counter++;}
				break;
			case 7:
				(*pointcloud).setColor((uchar)ply_get_argument_value(argument_), counter, 0);
				break;
			case 8:
				(*pointcloud).setColor((uchar)ply_get_argument_value(argument_), counter, 1);
				break;
			case 9:
				(*pointcloud).setColor((uchar)ply_get_argument_value(argument_), counter, 2);
				if (point_break == 9) { counter++; }
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
		template <typename ElementType> bool readPly(trees::Pointcloud<ElementType>& pointcloud_)
		{
			if (!header) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			p_ply ply = ply_open(file, NULL, 0, NULL);

			if (!ply) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}
			
			if (!ply_read_header(ply)) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}
		
			if (points) {
				ply_set_read_cb(ply, "vertex", "x", callbackPointcloud<ElementType>, &pointcloud_, 1);
				ply_set_read_cb(ply, "vertex", "y", callbackPointcloud<ElementType>, &pointcloud_, 2);
				ply_set_read_cb(ply, "vertex", "z", callbackPointcloud<ElementType>, &pointcloud_, 3);
			}
			else{
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			if (normals) {
				ply_set_read_cb(ply, "vertex", "nx", callbackPointcloud<ElementType>, &pointcloud_, 4);
				ply_set_read_cb(ply, "vertex", "ny", callbackPointcloud<ElementType>, &pointcloud_, 5);
				ply_set_read_cb(ply, "vertex", "nz", callbackPointcloud<ElementType>, &pointcloud_, 6);
			}
		
			if (colors == 1) {
				ply_set_read_cb(ply, "vertex", "diffuse_red", callbackPointcloud<ElementType>, &pointcloud_, 7);
				ply_set_read_cb(ply, "vertex", "diffuse_green", callbackPointcloud<ElementType>, &pointcloud_, 8);
				ply_set_read_cb(ply, "vertex", "diffuse_blue", callbackPointcloud<ElementType>, &pointcloud_, 9);
			}
			
			if (colors == 2) {
				ply_set_read_cb(ply, "vertex", "red", callbackPointcloud<ElementType>, &pointcloud_, 7);
				ply_set_read_cb(ply, "vertex", "green", callbackPointcloud<ElementType>, &pointcloud_, 8);
				ply_set_read_cb(ply, "vertex", "blue", callbackPointcloud<ElementType>, &pointcloud_, 9);
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
		template <typename ElementType> bool writePly(char *file_, trees::Pointcloud<ElementType>& pointcloud_)
		{
			p_ply ply = ply_create(file_, PLY_DEFAULT, NULL, 0, NULL);
		
			ply_add_element(ply, "vertex", (long)pointcloud_.rows);
		
			e_ply_type ply_data_type = type == PLY_TYPE_FLOAT ? PLY_FLOAT32 : PLY_FLOAT64;
			if (points) {
				ply_add_property(ply, "x", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "y", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "z", ply_data_type, ply_data_type, ply_data_type);
			}
			else {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			if (normals) {
				ply_add_property(ply, "nx", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "ny", ply_data_type, ply_data_type, ply_data_type);
				ply_add_property(ply, "nz", ply_data_type, ply_data_type, ply_data_type);
			}
		
			if (colors) {
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
					uchar* color = pointcloud_.getColorPtr(i);
					ply_write(ply, color[0]);
					ply_write(ply, color[1]);
					ply_write(ply, color[2]);
				}
			}
		
			ply_close(ply);
		
			return true;
		}

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
		int points;

		/**
			Flag whether normals are contained in file
		*/
		int normals;
		
		/**
			Flag whether colors are contained in file
		*/
		int colors;
	};
}

#endif /* IO_IOPLY_H_ */