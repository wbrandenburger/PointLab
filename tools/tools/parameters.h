/**********************	*************************************************
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

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <iostream>

	/**
		Exit the program if there is detected an error

		@param[in] file_ String which specifies the file where the error is detected
		@param[in] line_ Integer which specifies the line in the file where the error is detected
	*/
	void exitFailure(const char* file, const int line)
	{
		std::cout << "Exit in " << file << " in line " << line << std::endl;
		std::exit(EXIT_FAILURE);
	}

	/**
		Swap

		@param[in,out] a Object which should be swapped with b
		@param[in,out] b Object which should be swapped with a
	*/
	template<typename ElementType> inline void swap(ElementType& a, ElementType& b) {
		ElementType temp = b;
		b = a;
		a = temp;
	}

	/**
		Flags which specify the members of a pointcloud
	*/
	enum struct PointcloudFlag
	{
		/**
			POINTS is used if the pointlcoud contains points
		*/
		POINTS = 0,
		/**
			RGB is used if the pointcloud contains colors
		*/
		RGB = 1,
		/**
			NORMALS is used if the pointcloud contains normals
		*/
		NORMALS = 2,
		/**
			TRIANGLES is used if the pointcloud contains triangles
		*/
		TRIANGLES = 3,
		/**
			RGBNORMALS is used if the pointcloud contains colors and normals
		*/
		RGBNORMALS = 6,
		/**
			MESH is used if the pointcloud contains color, normals and triangles
		*/
		MESH = 7
	};

	/**
		Operator |= for Vertex

		@param[in] identifier_
		@param[in] vertex_ An instance of struct Vertex
	*/
	uint8_t& operator|=(uint8_t& flag, const PointcloudFlag& vertex)
	{
		switch (vertex) {
		case PointcloudFlag::RGB: flag |= 1 << 1; break;
		case PointcloudFlag::NORMALS: flag |= 1 << 2; break;
		case PointcloudFlag::TRIANGLES: flag |= 1 << 3; break;
		case PointcloudFlag::RGBNORMALS: flag |= 1 << 6; break;
		case PointcloudFlag::MESH: flag |= 1 << 7; break;
		}

		return flag;
	}

	/**
		Operator |= for Vertex

		@param[in] identifier_
		@param[in] vertex_ An instance of struct Vertex
	*/
	uint8_t operator &(const uint8_t& flag, const PointcloudFlag& vertex)
	{
		switch (vertex) {
		case PointcloudFlag::RGB: return flag & 1 << 1; break;
		case PointcloudFlag::NORMALS: return flag & 1 << 2; break;
		case PointcloudFlag::TRIANGLES: return flag & 1 << 3; break;
		case PointcloudFlag::RGBNORMALS: return flag & 1 << 6; break;
		case PointcloudFlag::MESH: return flag & 1 << 7; break;
		}

		return 0;
	}

	/**
		Specify the type of pointcloud container
	*/
	enum struct PointcloudType
	{
		NONE =0,
		/**
			PointcloudAoS, which stores the information as an array of structures
		*/
		AOS = 1,
		/**
			PointcloudSoA, which stores the information as several arrays of structures
		*/
		SOA = 2
	};

	/**
		Flags which specify what kind of primitives to render
	*/
	enum struct GLParams
	{
		/**
			Points are rendered
		*/
		POINTS = 0,
		/**
			Triangles are rendered
		*/
		LINES = 1,
		/**
			Triangles are rendered
		*/
		TRIANGLES = 2
	};

	/**
		Computation of normals
	*/
	enum struct NormalComputation
	{
		/**
			Computation of a plane in the neighborhood of a point
		*/
		PLANESVD = 0,

		/**
			Computation of a plane using principal component analysis
		*/
		PLANEPCA = 1,

		/**
			Minimize the inner product of tangential vector and normal vector
		*/
		VECTORSVD = 2,

		/**
			Computation of a quadratic surface in the neighborhood of a point
		*/
		QUADSVD = 3
	};

	/**
		Weight functions
	*/
	enum struct WeightFunction
	{
		/**
			Equal weights
		*/
		BOX = 0,

		/**
			Linear weight function
		*/
		LINEAR = 1,

		/**
			Gaussian weight function
		*/
		GAUSSIAN = 2,
	};

	/**
		Approximation of the root
	*/
	enum struct RootsApproximation
	{
		/**
			Newton-Raphson method
		*/
		NEWTON = 0,
		
		/**
			Quadratic inverse Interpolation
		*/
		QUAD = 1,
	};

	/**
		Computation of a surface in a neighborhood
	*/
	enum struct SurfaceComputation
	{
		/**
			Computation of a plane
		*/
		PLANE = 0,

		/**
			Computation of a plane using moving least squares
		*/
		PLANEMLS = 1,

		/**
			Computation of a polynom of abitrary degree to approximate the surface
		*/
		SURFPOLY = 2,

		/**
			Computation of a polynom of abitrary degree to approximate the surface using moving least squares
		*/
		SURFPOLYMLS = 3,

	};

#endif /* PARAMETERS_H_ */