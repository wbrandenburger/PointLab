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

#ifndef GL_MESHGRID_H_
#define GL_MESHGRID_H_

#include "tools/parameters.h"

#include "tools/utils/boundingbox.h"
#include "tools/utils/matrix.h"

#include "tools/pointcloud/pointcloud.h"

namespace gl
{
	/**
		Computes a 2D-meshgrid of a specific range

		@param[in] points_neighborhood Matrix with the neighborhood
		@param[in] number_of_vertices Number of vertices
		@return Meshgrid
	*/
	template<typename ElementType> utils::Matrix<ElementType> meshGrid(
		utils::Matrix<ElementType> points_neighborhood,
		size_t number_of_vertices)
	{
		utils::BoundingBox<ElementType> bounding_box(points_neighborhood.getPtr(), points_neighborhood.getRows(), 3);

		return meshGrid<ElementType>(bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1), number_of_vertices);
	}

	/**
		Computes a 2D-meshgrid of a specific range

		@param[in] bounding_box Bounding box
		@param[in] number_of_vertices Number of vertices
		@return Meshgrid
	*/
	template<typename ElementType> utils::Matrix<ElementType> meshGrid(
		utils::BoundingBox<ElementType> bounding_box,
		size_t number_of_vertices)
	{
		return meshGrid<ElementType>(bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1), number_of_vertices);
	}

	/**
		Computes a 2D-meshgrid of a specific range

		@param[in] x_left Left border in x-direction
		@param[in] x_right Right border in x-direction
		@param[in] y_left Left border in y-direction
		@param[in] y_right Right borger in y-direction
		@param[in] number_of_vertices Number of vertices
		@return Meshgrid
	*/
	template<typename ElementType> utils::Matrix<ElementType> meshGrid(
		ElementType x_left,
		ElementType x_right, 
		ElementType y_left, 
		ElementType y_right,
		size_t number_of_vertices)
	{
		/** 
			Computation of the number of resulting elements and set the pointcloud
		*/
		ElementType quant_x = (x_right - x_left) / (ElementType)(number_of_vertices - 1);
		ElementType quant_y = (y_right - y_left) / (ElementType)(number_of_vertices - 1);

		size_t number_x = std::floor((x_right - x_left) / quant_x) + 1;
		size_t number_y = std::floor((y_right - y_left) / quant_y) + 1;

		number_of_vertices = number_x * number_y;

		ElementType* points = new ElementType[number_of_vertices * 3];

		/**
			Set the mehsgrid
		*/
		ElementType* points_ptr = points;
		ElementType x = x_left;
		for (size_t index_x = 0; index_x < number_x; index_x++) {
			ElementType y = y_left;
			for (size_t index_y = 0; index_y < number_y; index_y++) {
				/**
					Assign the x- and y-values
				*/
				*points_ptr = x; points_ptr++;
				*points_ptr = y; points_ptr++;
				/**
					Skip z-value
				*/
				*points_ptr = 0; points_ptr++;
				y = y + quant_y;
			}
			x = x + quant_x;
		}

		return utils::Matrix<ElementType>(points, number_of_vertices, 3);
	}

	/**
		Computes a 2D-meshgrid of a specific range and builds a mesh
		
		@param[in] points_neighborhood  Matrix with the neighborhood
		@param[in,out] points Meshgrid
		@param[in,out] lines Lines
		@param[in] number_of_vertices Number of vertices

	*/
	template<typename ElementType> void glMeshGrid(
		const utils::Matrix<ElementType>& points_neighborhood,
		utils::Matrix<ElementType>& points,
		utils::Matrix<unsigned int>& lines,
		size_t number_of_vertices)
	{
		utils::BoundingBox<ElementType> bounding_box(points_neighborhood.getPtr(), points_neighborhood.getRows(), 3);

		glMeshGrid<ElementType>(points, lines, bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1), number_of_vertices);
	}

	/**
		Computes a 2D-meshgrid of a specific range builds a mesh

		@param[in,out] points Meshgrid
		@param[in,out] lines Lines
		@param[in] bounding_box Bounding box
		@param[in] number_of_vertices Number of vertices
	*/
	template<typename ElementType> void glMeshGrid(
		utils::Matrix<ElementType>& points,
		utils::Matrix<unsigned int>& lines, 
		utils::BoundingBox<ElementType> bounding_box,
		size_t number_of_vertices)
	{
		glMeshGrid<ElementType>(points, lines, bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1), number_of_vertices);
	}

	/**
		Computes a 2D-meshgrid of a specific range  builds a mesh

		@param[in,out] points Meshgrid
		@param[in,out] lines Lines
		@param[in] x_left Left border in x-direction
		@param[in] x_right Right border in x-direction
		@param[in] y_left Left border in y-direction
		@param[in] y_right Right borger in y-direction
		@param[in] number_of_vertices Number of vertices
	*/
	template<typename ElementType> void glMeshGrid(
		utils::Matrix<ElementType>& points,
		utils::Matrix<unsigned int>& lines,
		ElementType x_left,
		ElementType x_right,
		ElementType y_left,
		ElementType y_right,
		size_t number_of_vertices)
	{

		/**
			Computation of the number of resulting elements and set the pointcloud
		*/
		ElementType quant_x = (x_right - x_left) / (ElementType)(number_of_vertices - 1);
		ElementType quant_y = (y_right - y_left) / (ElementType)(number_of_vertices - 1);

		size_t number_x = std::floor((x_right - x_left) / quant_x) + 1;
		size_t number_y = std::floor((y_right - y_left) / quant_y) + 1;

		/**
			compute the meshgrid
		*/
		points = meshGrid<ElementType>(x_left, x_right, y_left, y_right, number_of_vertices);

		size_t number_of_lines =  (number_x - 1) * (number_y - 1) * 2 + number_x + number_y - 2;
		
		unsigned int* lines_ptr = new unsigned int[number_of_lines * 2];
		lines.setMatrix(lines_ptr, number_of_lines, 2);

		/**
			Set the indices
		*/
		size_t index = 0;
		for (size_t x = 0; x < number_x; x++) {
			for (size_t y = 0; y < number_y; y++) {
				if (y != number_y - 1) {
					*lines_ptr = index; lines_ptr++;
					*lines_ptr = index + 1; lines_ptr++;
				}
				if (x != number_x - 1) {
					*lines_ptr = index; lines_ptr++;
					*lines_ptr = index + number_y; lines_ptr++;
				}
				index++;
			}
		}
	}
}

#endif /* GL_MESHGRID_H_ */