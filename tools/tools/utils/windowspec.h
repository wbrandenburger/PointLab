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

#ifndef UTILS_WINDOWSPEC_H_
#define UTILS_WINDOWSPEC_H_

#include <Windows.h>

#include "tools/utils/matrix.h"

namespace utils
{
	class WindowSpec
	{
	public:
		/**
			Constructor
		*/
		WindowSpec() 
		{
			position_x = 0;
			position_y = 0;

			width = (size_t) GetSystemMetrics(SM_CXSCREEN) / 3;
			height = (size_t) GetSystemMetrics(SM_CYSCREEN) / 3;
		}

		/**
			Constructor

			@param[in] position_x_ Window position in x-direction
			@param[in] position_y_ Window position in y-direction
			@param[in] width_ Width of the window
			@param[in] height_ Height of the window
		*/
		WindowSpec(size_t position_x_, size_t position_y_, size_t width_, size_t height_)
		{
			position_x = position_x_;
			position_y = position_y_;
			width = width_;
			height = height_;
		}

		/**
			Set WindowSpec parameter

			@param[in] position_x_ Window position in x-direction
			@param[in] position_y_ Window position in y-direction
			@param[in] width_ Width of the window
			@param[in] height_ Height of the window
		*/
		void setWindowSpec(size_t position_x_, size_t position_y_, size_t width_, size_t height_)
		{
			position_x = position_x_;
			position_y = position_y_;
			width = width_;
			height = height_;
		}

		/**
			Get window position in x-direction

			@return Window position in x-direction
		*/
		size_t getPositionX()
		{
			return position_x;
		}

		/**
			Get window position in y-direction

			@return Window position in y-direction
		*/
		size_t getPositionY()
		{
			return position_y;
		}

		/**
			Get width of the window

			@return Width of the window
		*/
		size_t getWidth()
		{
			return width;
		}

		/**
			Get height of the window

			@return Height of the window
		*/
		size_t getHeight()
		{
			return height;
		}

	private:

		/**
			Window position in x-direction
		*/
		size_t position_x;

		/**
			Window position in y-direction
		*/
		size_t position_y;

		/**
			Width of the window
		*/
		size_t width;

		/**
			Height of the window
		*/
		size_t height;
	};	
}

#endif /* UTILS_WINDOWSPEC_H_ */