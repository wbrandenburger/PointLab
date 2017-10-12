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

#ifndef UTILS_GLVIEWER_H_
#define UTILS_GLVIEWER_H_

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include "tools/pointcloud/pointcloud.h"

namespace utils
{
	typedef unsigned char uchar;

	template<typename ElementType> class ViewerInstance
	{
		/**
			Constructor
		*/
		ViewerInstance() {}

		/**
			Destructor
		*/
		~ViewerInstance() {}

		/**
			Set pointcloud

			@param[in] pointcloud_ Pointcloud
		*/
		void setPointcloud(const pointcloud::Pointcloud<ElementType>& pointcloud_)
		{
			points = pointcloud_.getPointsPtr();
			color = pointcloud_.getColorsPtr();
		}

		/**
			Draw the points
		*/
		void draw() 
		{
			glPushMatrix();

			glPopMatrix();
		}

		/**
			Points
		*/
		ElementType* points;
		
		/**
			Color
		*/
		uchar* color;

		/**
			Number of elements
		*/
		size_t number_of_elements;
	};

}

#endif /* UTILS_GLVIEWER_H_*/	