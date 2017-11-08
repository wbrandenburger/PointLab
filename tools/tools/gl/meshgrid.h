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

#ifndef UTILS_MESHGRID_H_
#define UTILS_MESHGRID_H_

#include "tools/parameters.h"

#include "tools/utils/boundingbox.h"

#include "tools/pointcloud/pointcloud.h"

namespace utils
{
	/**
		Computes a 2D-meshgrid of a specific range
		
		@param[in] dataset_in Pointer to a dataset with the reference to a neighborhood
		@param[in,out] dataset_new Pointer in which the mehsgrid will be saved
		@param[in] number_of_vertices_in 
		@param[in,out] number_of_vertices_out Number of points in one direction
	*/
	template<typename ElementType> void meshGrid(ElementType* dataset_in , ElementType** dataset_out,
		size_t number_of_vertices_in, size_t& number_of_vertices_out)
	{
		BoundingBox<ElementType> bounding_box(dataset_in, number_of_vertices_in, 3);

		meshGrid<ElementType>(dataset_out, bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1), number_of_vertices_out);
	}

	/**
		Computes a 2D-meshgrid of a specific range

		@param[in,out] dataset Pointer in which the mehsgrid will be saved
		@param[in] bounding_box Bounding box
		@param[in,out] number_of_vertices Number of points in one direction
	*/
	template<typename ElementType> void meshGrid(ElementType** dataset,
		utils::BoundingBox<ElementType> bounding_box, size_t& number_of_vertices)
	{
		meshGrid<ElementType>(pointcloud, bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1), number_of_vertices);
	}

	/**
		Computes a 2D-meshgrid of a specific range

		@param[in,out] dataset Pointer in which the mehsgrid will be saved
		@param[in] x_left Left border in x-direction
		@param[in] x_right Right border in x-direction
		@param[in] y_left Left border in y-direction
		@param[in] y_right Right borger in y-direction
		@param[in,out] number_of_vertices Number of points in one direction
	*/
	template<typename ElementType> void meshGrid(ElementType** dataset,
		ElementType x_left,ElementType x_right, ElementType y_left, ElementType y_right, 
		size_t& number_of_vertices)
	{
		/** 
			Computation of the number of resulting elements and set the pointcloud
		*/

		ElementType quant = std::floor(x_right - x_left) > std::floor(y_right - y_left) ? 
				std::floor(x_right - x_left) / ((ElementType) number_of_vertices - (ElementType) 1.0) : 
				std::floor(y_right - y_left) / ((ElementType) number_of_vertices - (ElementType) 1.0);
		
		size_t number_x = std::floor((x_right - x_left) / quant) + 1;
		size_t number_y = std::floor((y_right - y_left) / quant) + 1;
		number_of_vertices = number_x * number_y;

		*dataset = new ElementType[number_of_vertices * 3];

		/**
			Set the mehsgrid
		*/
		ElementType* dataset_ptr = *dataset;
		for (ElementType x = x_left; x <= x_right; x += quant) {
			for (ElementType y = y_left; y <= y_right; y += quant) {
				/**
					Assign the x- and y-values
				*/
				*dataset_ptr = x; dataset_ptr++;
				*dataset_ptr = y; dataset_ptr++;
				/**
					Skip z-value
				*/
				*dataset_ptr = 0; dataset_ptr++;
			}
		}
	}

	/**
		Computes a 2D-meshgrid of a specific range
		
		@param[in] dataset_in Pointer to a dataset with the reference to a neighborhood
		@param[in,out] dataset_new Pointer in which the mehsgrid will be saved
		@param[in] number_of_vertices_in 
		@param[in,out] number_of_vertices_out Number of points in one direction
		@param[in,out] lines Pointer to the lines
		@param[in,out] number_of_lines Number of lines
	*/
	template<typename ElementType> void glMeshGrid(ElementType* dataset_in , ElementType** dataset_out,
		size_t number_of_vertices_in, size_t& number_of_vertices_out, unsigned int** lines, size_t& number_of_lines)
	{
		BoundingBox<ElementType> bounding_box(dataset_in, number_of_vertices_in, 3);

		glMeshGrid<ElementType>(dataset_out, bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1), number_of_vertices_out, lines, number_of_lines);
	}

	/**
		Computes a 2D-meshgrid of a specific range

		@param[in,out] dataset Pointer in which the mehsgrid will be saved
		@param[in] bounding_box Bounding box
		@param[in,out] number_of_vertices Number of points in one direction
		@param[in,out] lines Pointer to the lines
		@param[in,out] number_of_lines Number of lines
	*/
	template<typename ElementType> void glMeshGrid(ElementType** dataset, utils::BoundingBox<ElementType> bounding_box,
		size_t& number_of_vertices, unsigned int** lines, size_t& number_of_lines)
	{
		glMeshGrid<ElementType>(pointcloud, bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1), number_of_vertices, lines, number_of_lines);
	}

	/**
		Computes a 2D-meshgrid of a specific range

		@param[in,out] dataset Pointer in which the mehsgrid will be saved
		@param[in] x_left Left border in x-direction
		@param[in] x_right Right border in x-direction
		@param[in] y_left Left border in y-direction
		@param[in] y_right Right borger in y-direction
		@param[in,out] number_of_vertices Number of points in one direction
		@param[in,out] lines Pointer to the lines
		@param[in,out] number_of_lines Number of lines
	*/
	template<typename ElementType> void glMeshGrid(ElementType** dataset,
		ElementType x_left, ElementType x_right, ElementType y_left, ElementType y_right,
		size_t& number_of_vertices, unsigned int** lines, size_t& number_of_lines)
	{
		
		/** 
			Computation of the number of resulting elements and set the pointcloud
		*/

		ElementType quant = std::floor(x_right - x_left) > std::floor(y_right - y_left) ? 
				std::floor(x_right - x_left) / ((ElementType) number_of_vertices - (ElementType) 1.0) : 
				std::floor(y_right - y_left) / ((ElementType) number_of_vertices - (ElementType) 1.0);
		
		size_t number_x = std::floor((x_right - x_left) / quant) + 1;
		size_t number_y = std::floor((y_right - y_left) / quant) + 1;
		
		/**
			compute the meshgrid
		*/
		meshGrid<ElementType>(dataset, x_left, x_right, y_left, y_right, number_of_vertices);

		number_of_lines =  (number_x - 1) * (number_y - 1) * 2 + number_x + number_y - 2;
		*lines = new unsigned int[number_of_lines * 2];

		/**
			Set the indices
		*/
		unsigned int* lines_ptr = *lines;
		size_t index = 0;
		for (size_t x = 0; x < number_x; x++) {
			for (size_t y = 0; y < number_y; y++) {
				if (y != number_x - 1) {
					*lines_ptr = index; lines_ptr++;
					*lines_ptr = index + 1; lines_ptr++;
				}
				if (x != number_y - 1) {
					*lines_ptr = index; lines_ptr++;
					*lines_ptr = index + number_x; lines_ptr++;
				}
				index++;
			}
		}

	}
}

#endif /* UTILS_MESHGRID_H_ */