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
	template<typename ElementType> struct PointcloudIterators
	{
	public:
		/**
			Constructor 

			@param[in] pointcloud Pointcloud
		*/
		PointcloudIterators(const pointcloud::Pointcloud<ElementType>& pointcloud)
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

			end_vertices_ = pointcloud.endPoint();

			end_triangles_ = pointcloud.endTriangle();
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
		PointcloudIterators(const PointcloudIterators<ElementType>& pointcloud_iterators) = delete;

		/**
			Copy constructor
	
			@param[in] class_ An instance of class PointcloudIterators
		*/
		PointcloudIterators(const PointcloudIterators<ElementType>&& pointcloud_iterators) = delete;
	
		/**
			Operator =
	
			@param[in] class_ An instance of class PointcloudIterators
			@return Returns reference to the current instance
		*/
		PointcloudIterators& operator=(const PointcloudIterators<ElementType>& pointcloud_iterators) = delete;
	
		/**
			Operator =
		
			@param[in] glview_ An instance of class PointcloudIterators
			@return Returns reference to the current instance
		*/
		PointcloudIterators& operator=(const PointcloudIterators<ElementType>&& pointcloud_iterators) = delete;

		/**
			Set iterators

			@param[in] pointcloud Pointcloud
		*/
		void setIterators(const pointcloud::Pointcloud<ElementType>& pointcloud)
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
		void setPoint(const ElementType& point) 
		{
			*iterator_point_ = point;
			iterator_point_++;
		}

		/**
			Set color and increment pointer
		*/
		void setColor(const uint8_t& color) 
		{
			*iterator_color_ = color;
			iterator_color_++;
		}

		/**
			Set a normal and increment pointer
		*/
		void setNormal(const ElementType& normal) 
		{
			*iterator_normal_ = normal;
			iterator_normal_++;
		}

		/**
			Set a triangle and increment pointer
		*/
		void setTriangle(const size_t& triangle) 
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
			Checks whether the end of vertices is reached
		*/
		bool endVertices()
		{
			return iterator_point_ == end_vertices_;
		}

		/**
			Checks whether the end of triangles is reached
		*/
		bool endTriangles()
		{
			return iterator_triangle_ == end_triangles_;
		}

	private:

		typedef pointcloud::Pointcloud<ElementType> Pointcloud;

		/**
			Point iterator
		*/
		Pointcloud::Iterator<ElementType> iterator_point_;

		/**
			Color iterator
		*/
		Pointcloud::Iterator<uint8_t> iterator_color_;

		/**
			Normal iterator
		*/
		Pointcloud::Iterator<ElementType> iterator_normal_;

		/**
			Triangle iterator
		*/
		Pointcloud::Iterator<size_t> iterator_triangle_;

		/**
			End of vertices
		*/
		ElementType* end_vertices_;

		/**
			End of triangles
		*/
		size_t* end_triangles_;
	};


	/**
		Callback function which inserts points, color and normals in the pointcloud

		@param[in] argument Argument which contains the structure in which the elements will be inserted
		@return Returns true if insertion was successful
	*/
	template <typename ElementType> static int callbackPointcloudIterator(p_ply_argument argument)
	{
		long index;
		PointcloudIterators<ElementType>* iterator;

		ply_get_argument_user_data(argument, (void**)&iterator, &index);
		
		switch (index) {
		case 1:
			iterator->setPoint((ElementType)ply_get_argument_value(argument));
			break;
		case 2:
			iterator->setColor((uint8_t)ply_get_argument_value(argument));
			break;
		case 3:
			iterator->setNormal((ElementType)ply_get_argument_value(argument));
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
	template <typename ElementType> static int callbackFaceIterator(p_ply_argument argument)
	{
		long length, value_index, index;
		ply_get_argument_property(argument, NULL, &length, &value_index);
		
		PointcloudIterators<ElementType>* iterator;
		ply_get_argument_user_data(argument, (void**)&iterator, &index);

		if (value_index != -1) {
			iterator->setTriangle((size_t)ply_get_argument_value(argument));
		}
	
		return 1;
	}	

	/**
		Read the file and inserts the data in the structure

		@param[in,out] pointcloud Structure in which the elements will be inserted			
		@return Returns true if the reading was successful
	*/
	template <typename ElementType> bool readPly(char* file, 
		pointcloud::Pointcloud<ElementType>& pointcloud)
	{
		p_ply ply = ply_open(file, NULL, 0, NULL);
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
		
		size_t number_of_vertices = 0;
		size_t number_of_triangles = 0;

		uint8_t pointcloud_flag = 0;
		uint8_t color_spec = 0;
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
				number_of_vertices = (size_t) elem_instances;
			}
			if (std::strcmp("face", elem_name) == 0) {
				number_of_triangles = (size_t) elem_instances;
				pointcloud_flag |= PointcloudFlag::TRIANGLES;
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

				if (!strcmp(prop_name, "nx")) { pointcloud_flag |= PointcloudFlag::NORMALS;}
				if (!strcmp(prop_name, "diffuse_red")) { color_spec = 1; pointcloud_flag |= PointcloudFlag::RGB; }
				if (!strcmp(prop_name, "red")) { color_spec = 2; pointcloud_flag |= PointcloudFlag::RGB; }

				prop = ply_get_next_property(elem, prop);
			}

			/**
				Get the next element in the header
			*/
			elem = ply_get_next_element(ply, elem);
		}

		/**
			Set the pointcloud
		*/
		pointcloud.setFlags(pointcloud_flag);
		pointcloud.setNumberOfVertices(number_of_vertices);
		if ((pointcloud_flag & PointcloudFlag::TRIANGLES) > 0){
			pointcloud.setNumberOfTriangles(number_of_triangles);
		}
		pointcloud.setPointcloud();

		/**
			Set the itertors of the pointcloud
		*/
		PointcloudIterators<ElementType> iterators(pointcloud);

		/**
			Set the callback functions
		*/
		ply_set_read_cb(ply, "vertex", "x", callbackPointcloudIterator<ElementType>, &iterators, 1);
		ply_set_read_cb(ply, "vertex", "y", callbackPointcloudIterator<ElementType>, &iterators, 1);
		ply_set_read_cb(ply, "vertex", "z", callbackPointcloudIterator<ElementType>, &iterators, 1);
			
		if ( color_spec == 1) {
			ply_set_read_cb(ply, "vertex", "diffuse_red", callbackPointcloudIterator<ElementType>, &iterators, 2);
			ply_set_read_cb(ply, "vertex", "diffuse_green", callbackPointcloudIterator<ElementType>, &iterators, 2);
			ply_set_read_cb(ply, "vertex", "diffuse_blue", callbackPointcloudIterator<ElementType>, &iterators, 2);
		}

		if ( color_spec == 2) {
			ply_set_read_cb(ply, "vertex", "red", callbackPointcloudIterator<ElementType>, &iterators, 2);
			ply_set_read_cb(ply, "vertex", "green", callbackPointcloudIterator<ElementType>, &iterators, 2);
			ply_set_read_cb(ply, "vertex", "blue", callbackPointcloudIterator<ElementType>, &iterators, 2);
		}

		if ((pointcloud_flag & PointcloudFlag::NORMALS) > 0) {
			ply_set_read_cb(ply, "vertex", "nx", callbackPointcloudIterator<ElementType>, &iterators, 3);
			ply_set_read_cb(ply, "vertex", "ny", callbackPointcloudIterator<ElementType>, &iterators, 3);
			ply_set_read_cb(ply, "vertex", "nz", callbackPointcloudIterator<ElementType>, &iterators, 3);
		}

		if ((pointcloud_flag & PointcloudFlag::TRIANGLES) > 0) {
			ply_set_read_cb(ply, "face", "vertex_indices", callbackFaceIterator<ElementType>, &iterators, 0);
		}

		/**
			Read the file 
		*/
		if (!ply_read(ply)) {
			return 0;
		}

		ply_close(ply);

		return true;
	}

	/**
		Write ply file with given structure

		@param[in] file Name of file
		@param pointcloud Structure which elements will be written in the file
		@return Returns true if writing was successful
	*/
	template <typename ElementType> bool writePly(char *file, 
		const pointcloud::Pointcloud<ElementType>& pointcloud)
	{
		/**
			Create the file
		*/
		p_ply ply = ply_create(file, PLY_ASCII, NULL, 0, NULL);
	
		/**
			Define the type of the points
		*/
		e_ply_type ply_type = std::is_same<ElementType,float>::value ? PLY_FLOAT32 : PLY_FLOAT64;
		
		uint8_t pointcloud_flag = pointcloud.getPointcloudFlag();
		ply_add_element(ply, "vertex", (long)pointcloud.getNumberOfVertices());

		/**
			Set vertex elements to write
		*/
		ply_add_property(ply, "x", ply_type, ply_type, ply_type);
		ply_add_property(ply, "y", ply_type, ply_type, ply_type);
		ply_add_property(ply, "z", ply_type, ply_type, ply_type);

		if ((pointcloud_flag & PointcloudFlag::RGB) > 0) {
			ply_add_property(ply, "red", PLY_UCHAR, PLY_UCHAR, PLY_UCHAR);
			ply_add_property(ply, "green", PLY_UCHAR, PLY_UCHAR, PLY_UCHAR);
			ply_add_property(ply, "blue", PLY_UCHAR, PLY_UCHAR, PLY_UCHAR);
		}

		if ((pointcloud_flag & PointcloudFlag::NORMALS) > 0) {
			ply_add_property(ply, "nx", ply_type, ply_type, ply_type);
			ply_add_property(ply, "ny", ply_type, ply_type, ply_type);
			ply_add_property(ply, "nz", ply_type, ply_type, ply_type);
		}

		/**
			Set face elements to write
		*/
		if ((pointcloud_flag & PointcloudFlag::TRIANGLES) > 0) {
			ply_add_element(ply, "face", (long) pointcloud.getNumberOfTriangles());
			ply_add_list_property(ply, "vertex_indices", PLY_UCHAR, PLY_INT);
		}

		/**
			Write the header
		*/
		ply_write_header(ply);
				
		/**
			Set the itertors of the pointcloud
		*/
		PointcloudIterators<ElementType> iterators(pointcloud);

		/**
			Write vertex elements

			Rply counts the elements which are written and sets automatically a line break
		*/
		while (!iterators.endVertices()) {
			ply_write(ply, iterators.getPoint());
			ply_write(ply, iterators.getPoint());
			ply_write(ply, iterators.getPoint());

			if ((pointcloud_flag & PointcloudFlag::RGB) > 0) {
				ply_write(ply, iterators.getColor());
				ply_write(ply, iterators.getColor());
				ply_write(ply, iterators.getColor());
			}
			if ((pointcloud_flag & PointcloudFlag::NORMALS) > 0) {
				ply_write(ply, iterators.getNormal());
				ply_write(ply, iterators.getNormal());
				ply_write(ply, iterators.getNormal());
			}
		}

		/**
			Write face elements

			Rply counts the elements which are written and sets automatically a line break
		*/
		while (!iterators.endTriangles()) {
			ply_write(ply, 3);
			ply_write(ply, iterators.getTriangle());
			ply_write(ply, iterators.getTriangle());
			ply_write(ply, iterators.getTriangle());
		}
		
		ply_close(ply);
	
		return true;
	}
}

#endif /* IO_IOPLY_H_ */