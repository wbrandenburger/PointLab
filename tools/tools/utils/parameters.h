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
			if (!CURRENT_GL_WINDOWS) {
				CURRENT_GL_WINDOWS = 0;
			}
		}

		/**
			Destructor
		*/
		~GLInstances()
		{
		}

	private:
	public:
		/**
			Number of called windows
		*/
		static size_t CURRENT_GL_WINDOWS;

		/**
			Instance of plotting tool
		*/
		static utils::GLPlot<ElementType> GLPLOT;
		/**
			Instance of viewer tool
		*/
		static utils::GLViewer<ElementType> GLVIEWER;
	};

	/**
		Static variable GLInstances<ElementType>::CURRENT_GL_WINDOWS
	*/
	template<typename ElementType> size_t  GLInstances<ElementType>::CURRENT_GL_WINDOWS;

	/**
		Static variable GLInstances<ElementType>::GLPLOT
	*/
	template<typename ElementType> utils::GLPlot<ElementType> GLInstances<ElementType>::GLPLOT;

	/**
		Static variable GLInstances<ElementType>::GLPLOT
	*/
	template<typename ElementType> utils::GLViewer<ElementType> GLInstances<ElementType>::GLVIEWER;
}

#endif /* UTILS_PARAMETERS_H_ */