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

#include <vector>

#include <Windows.h>

#include "tools/utils/matrix.h"
#include "tools/utils/windowspec.h"

#include "tools/utils/glplot.h"
#include "tools/utils/glplot3d.h"
#include "tools/utils/glviewer.h"

namespace utils
{
	enum struct GLInstance
	{
		NONE = 0,
		GLVIEWER = 1,
		GLPLOT = 2,
		GLPLOT3D = 3
	};

	template<typename ElementType> class GLView
	{
	public:
		/**
			Constructor
		*/
		GLView()
		{
			int argc = 0;
			char** argv;
			
			glutInit(&argc, argv);

			glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
		}

		/**
			Destructor
		*/
		~GLView()
		{
		}

		/**
			Copy constructor

			@param[in] glview_ An instance of class GLView
		*/
		GLView(const GLView<ElementType>& glview_) = delete;
		
		/**
			Copy constructor

			@param[in] glview_ An instance of class GLView
		*/
		GLView(const GLView<ElementType>&& glview_) = delete;

		/**
			Operator =

			@param[in] glview_ An instance of class GLView
		*/
		GLView& operator=(const GLView<ElementType>& glview) = delete;
		
		/**
			Operator =

			@param[in] glview_ An instance of class GLView
		*/
		GLView& operator=(const GLView<ElementType>&& glview) = delete;

		/**
			Initializes a viewer and sets a instance of a container, where the data can be assigned
		*/
		void setViewer()
		{
			glviewer.setViewer();

			current_instance = GLInstance::GLVIEWER;
		}

		/**
			Initializes a graph plot and sets a instance of a container, where the data can be assigned

			@param[in] number_of_elements_ Number of elements
		*/
		void setPlot(size_t number_of_elements_)
		{
			glplot.setPlot(number_of_elements_);

			current_instance = GLInstance::GLPLOT;
		}

		/**
			Initializes a viewer for a plot in 3d and sets a instance of a container, where the data can be assigned
		*/
		void setPlot3D()
		{
			glplot3d.setPlot3D();

			current_instance = GLInstance::GLPLOT3D;
		}

		/**
			Sets the x-data array for plotting

			@param[in] x_ x-values
		*/
		void setX(ElementType* x_)
		{
			if (current_instance != GLInstance::GLPLOT) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			glplot.setX(x_);
		}

		/**
			Sets the x-data array for plotting

			@param[in] y_ y-values
		*/
		void setX(std::vector<ElementType>& x_)
		{
			if (current_instance != GLInstance::GLPLOT) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			glplot.setX(x_);
		}
		
		/**
			Sets the x-data array for plotting

			@param[in] y_ y-values
		*/
		void setX(utils::Matrix<ElementType>& x_)
		{
			if (current_instance != GLInstance::GLPLOT) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			glplot.setX(x_);
		}

		/**
			Sets the y-data array for plotting

			@param[in] y_ y-values
		*/
		void setY(ElementType* y_)
		{
			if (current_instance != GLInstance::GLPLOT) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			glplot.setY(y_);
		}

		/**
			Sets the x-data array for plotting

			@param[in] y_ y-values
		*/
		void setY(std::vector<ElementType>& y_)
		{
			if (current_instance != GLInstance::GLPLOT) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			glplot.setY(y_);
		}
		
		/**
			Sets the x-data array for plotting

			@param[in] y_ y-values
		*/
		void setY(utils::Matrix<ElementType>& y_)
		{
			if (current_instance != GLInstance::GLPLOT) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			glplot.setY(y_);
		}

		/**
			Set pointcloud

			@param[in] pointcloud_ Pointcloud
		*/
		void setPointcloud(const pointcloud::Pointcloud<ElementType>& pointcloud_)
		{
			if (current_instance != GLInstance::GLVIEWER) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			glviewer.setPointcloud(pointcloud_);
		}

		/**
			Set pointcloud

			@param[in] points_ Pointcloud
			@param[in] number_of_elements_ Number of elements
		*/
		void setPointcloud(ElementType* points_, size_t number_of_elements_) 
		{
			if (current_instance == GLInstance::GLPLOT) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			glviewer.setPointcloud(points_, number_of_elements_);
		}

		/**
			Creates the window and sets the callback functions	
		*/
		void plot()
		{
			switch (current_instance) {
			case GLInstance::GLVIEWER:	
				glviewer.plot(); 
				break;
			case GLInstance::GLPLOT:	
				glplot.plot(); 
				break;
			default:	
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;	
				std::exit(EXIT_FAILURE);
				break;
			}
			current_instance = GLInstance::NONE;
		}

		/**
			Creates a subwindow and sets the callback functions	
		*/
		void subPlot(size_t windows_x_, size_t windows_y_, size_t index_)
		{
			size_t screen_width = (size_t) GetSystemMetrics(SM_CXSCREEN);
			size_t screen_height = (size_t) GetSystemMetrics(SM_CYSCREEN);

			size_t index_window_x = index_ % windows_x_;
			size_t index_window_y = index_ / windows_x_;

			size_t width = screen_width / windows_x_;
			size_t height = screen_height / windows_y_;

			size_t position_x = width * index_window_x;
			size_t position_y = height * index_window_y;

			utils::WindowSpec window_spec(position_x, position_y, width, height);

			switch (current_instance) {
			case GLInstance::GLVIEWER:
				glviewer.plot(nullptr, window_spec);
				break;
			case GLInstance::GLPLOT:
				glplot.plot(nullptr, window_spec);
				break;
			default:
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
				break;
			}
			current_instance = GLInstance::NONE;
		}

		/**
			Start the loop
		*/
		void mainLoop()
		{
			glutMainLoop();
		}

	private:

		/**
			Instance of plotting tool
		*/
		static utils::GLPlot<ElementType> glplot;
		
		/**
			Instance of viewer tool
		*/
		static utils::GLViewer<ElementType> glviewer;

		/**
			Instance of plotting in 3d
		*/
		static utils::GLPlot3D<ElementType> glplot3d;

		/**
			Current instance
		*/
		GLInstance current_instance;
	};

	/**
		Static variable GLView<ElementType>::glplot
	*/
	template<typename ElementType> utils::GLPlot<ElementType> GLView<ElementType>::glplot;

	/**
		Static variable GLView<ElementType>::glviewer
	*/
	template<typename ElementType> utils::GLViewer<ElementType> GLView<ElementType>::glviewer;

	/**
		Static variable GLPlot3D<ElementType>::glplot3d
	*/
	template<typename ElementType> utils::GLPlot3D<ElementType> GLView<ElementType>::glplot3d;

}

#endif /* UTILS_PARAMETERS_H_ */