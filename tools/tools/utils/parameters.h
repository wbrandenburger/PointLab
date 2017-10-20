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

#ifndef UTILS_PARAMETERS_H_
#define UTILS_PARAMETERS_H_

#include "tools/utils/glplot.h"
#include "tools/utils/glviewer.h"

namespace utils
{
	template<typename ElementType> class GLPlot;
	template<typename ElementType> class GLViewer;
	template<typename ElementType> class GLInstances
	{
	public:
		/**
			Constructor
		*/
		GLInstances()
		{
			if (!current_gl_windows) {
				current_gl_windows = 0;
			}
		}

		/**
			Destructor
		*/
		~GLInstances()
		{
		}


		/**
		
			
		*/
		void getWindowSpecification(size_t index_x_, size_t index_y_, size_t index_, 
			size_t& x_, size_t& y_, size_t& width_, size_t& height_)
		{	
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);


		}

	private:
	public:
		/**
			Number of called windows
		*/
		static size_t current_gl_windows;

		/**
			Instance of plotting tool
		*/
		static utils::GLPlot<ElementType> glplot;
		/**
			Instance of viewer tool
		*/
		static utils::GLViewer<ElementType> glviewer;
	};

	/**
		Static variable GLInstances<ElementType>::CURRENT_GL_WINDOWS
	*/
	template<typename ElementType> size_t  GLInstances<ElementType>::current_gl_windows;

	/**
		Static variable GLInstances<ElementType>::GLPLOT
	*/
	template<typename ElementType> utils::GLPlot<ElementType> GLInstances<ElementType>::glplot;

	/**
		Static variable GLInstances<ElementType>::GLPLOT
	*/
	template<typename ElementType> utils::GLViewer<ElementType> GLInstances<ElementType>::glviewer;

}

#endif /* UTILS_PARAMETERS_H_ */