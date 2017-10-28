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

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <iostream>

	/**
		Exit the program if there is detected an error

		@param[in] file_ String which specifies the file where the error is detected
		@param[in] line_ Integer which specifies the line in the file where the error is detected
	*/
	void exitFailure(const char* file_, const int line_)
	{
		std::cout << "Exit in " << file_ << " in line " << line_ << std::endl;
		std::exit(EXIT_FAILURE);
	}

	/**
		Flags which specify the members of a pointcloud
	*/
	enum struct Vertex
	{
		RGB = 1,
		NORMALS = 2,
		TRIANGLES = 3,
		RGBNORMALS = 6,
		MESH = 7
	};

	/**
		Operator |= for Vertex

		@param[in] vertex_ An instance of struct Vertex
	*/
	uint8_t& operator|=(uint8_t& identifier_, const Vertex& vertex)
	{
		switch (vertex) {
		case Vertex::RGB: identifier_ |= 1 << 1; break;
		case Vertex::NORMALS: identifier_ |= 1 << 2; break;
		case Vertex::TRIANGLES: identifier_ |= 1 << 3; break;
		case Vertex::RGBNORMALS: identifier_ |= 1 << 6; break;
		case Vertex::MESH: identifier_ |= 1 << 7; break;
		}

		return identifier_;
	}

	/**
		Flags which specify what kind of primitives to render
	*/
	enum struct GLParams
	{
		POINTS = 0,
		TRIANGLES = 1
	};

#endif /* PARAMETERS_H_ */