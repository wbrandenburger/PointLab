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

#include "tools/pointcloud/pointcloud.h"

namespace gl
{
	/**
		Computes a 2D-meshgrid of a specific range

		@param[in] data Matrix with the neighborhood
		@return Meshgrid
	*/
	template<typename ElementType> utils::Matrix<ElementType> meshGrid(
		utils::Matrix<ElementType> data)
	{
		BoundingBox<ElementType> bounding_box(data.getPtr(), dat.getRows(), 3);

		return meshGrid<ElementType>(bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1));
	}

	/**
		Computes a 2D-meshgrid of a specific range

		@param[in] bounding_box Bounding box
		@return Meshgrid
	*/
	template<typename ElementType> utils::Matrix<ElementType> meshGrid(
		utils::BoundingBox<ElementType> bounding_box)
	{
		return meshGrid<ElementType>(bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1));
	}

	/**
		Computes a 2D-meshgrid of a specific range

		@param[in] x_left Left border in x-direction
		@param[in] x_right Right border in x-direction
		@param[in] y_left Left border in y-direction
		@param[in] y_right Right borger in y-direction
		@return Meshgrid
	*/
	template<typename ElementType> utils::Matrix<ElementType> meshGrid(
		ElementType x_left,
		ElementType x_right, 
		ElementType y_left, 
		ElementType y_right)
	{
		/** 
			Computation of the number of resulting elements and set the pointcloud
		*/

		ElementType quant = std::floor(x_right - x_left) > std::floor(y_right - y_left) ? 
				std::floor(x_right - x_left) / (ElementType)( number_of_vertices - 1) :
				std::floor(y_right - y_left) / (ElementType)( number_of_vertices - 1);
		
		size_t number_x = std::floor((x_right - x_left) / quant) + 1;
		size_t number_y = std::floor((y_right - y_left) / quant) + 1;
		number_of_vertices = number_x * number_y;

		utils::Matrix<ElementType> data (new ElementType[number_of_vertices * 3], number_of_vertices, 3);

		/**
			Set the mehsgrid
		*/
		ElementType* data_ptr = data.getPtr();
		ElementType x = x_left;
		for (size_t index_x = 0; index_x < number_x; index_x++) {
			ElementType y = y_left;
			for (size_t index_y = 0; index_y < number_y; index_y++) {
				/**
					Assign the x- and y-values
				*/
				*data_ptr = x; data_ptr++;
				*data_ptr = y; data_ptr++;
				/**
					Skip z-value
				*/
				*data_ptr = 0; data_ptr++;
				y = y + quant;
			}
			x = x + quant;
		}

		return data;
	}

	/**
		Computes a 2D-meshgrid of a specific range and builds a mesh
		
		@param[in] data_in Matrix with the neighborhood
		@param[in,out] data_out Meshgrid
		@param[in,out] lines Lines

	*/
	template<typename ElementType> void glMeshGrid(
		const utils::Matrix<ElementType>& data_in, 
		utils::Matrix<ElementType>& data_out,
		utils::Matrix<unsigned int>& lines)
	{
		BoundingBox<ElementType> bounding_box(data_in.getPtr(), data_in.getRows(), 3);

		glMeshGrid<ElementType>(data_out, lines bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1));
	}

	/**
		Computes a 2D-meshgrid of a specific range builds a mesh

		@param[in,out] data_out Meshgrid
		@param[in,out] lines Lines
		@param[in] bounding_box Bounding box
	*/
	template<typename ElementType> void glMeshGrid(
		utils::Matrix<ElementType>& data_out,
		utils::Matrix<unsigned int>& lines, 
		utils::BoundingBox<ElementType> bounding_box)
	{
		glMeshGrid<ElementType>(data_out, lines bounding_box.getMinDim(0), bounding_box.getMaxDim(0),
			bounding_box.getMinDim(1), bounding_box.getMaxDim(1));
	}

	/**
		Computes a 2D-meshgrid of a specific range  builds a mesh

		@param[in,out] data_out Meshgrid
		@param[in,out] lines Lines
		@param[in] x_left Left border in x-direction
		@param[in] x_right Right border in x-direction
		@param[in] y_left Left border in y-direction
		@param[in] y_right Right borger in y-direction
	*/
	template<typename ElementType> void glMeshGrid(
		utils::Matrix<ElementType>& data_out,
		utils::Matrix<unsigned int>& lines,
		ElementType x_left, 
		ElementType x_right,
		ElementType y_left, 
		ElementType y_right,)
	{
		
		/** 
			Computation of the number of resulting elements and set the pointcloud
		*/

		ElementType quant = std::floor(x_right - x_left) > std::floor(y_right - y_left) ? 
				std::floor(x_right - x_left) / (ElementType)(number_of_vertices - 1) :
				std::floor(y_right - y_left) / (ElementType)(number_of_vertices - 1);
		
		size_t number_x = std::floor((x_right - x_left) / quant) + 1;
		size_t number_y = std::floor((y_right - y_left) / quant) + 1;

		/**
			compute the meshgrid
		*/
		data_out = meshGrid<ElementType>(x_left, x_right, y_left, y_right);

		number_of_lines =  (number_x - 1) * (number_y - 1) * 2 + number_x + number_y - 2;

		lines.setMatrix(new unsigned int[number_of_lines * 2], number_of_lines, 2);

		/**
			Set the indices
		*/
		unsigned int* lines_ptr = lines.getPtr();
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

#endif /* GL_MESHGRID_H_ */