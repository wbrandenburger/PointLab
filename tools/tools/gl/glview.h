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

#ifndef GL_PARAMETERS_H_
#define GL_PARAMETERS_H_

#include <vector>

#include <Windows.h>

#include "tools/parameters.h"

#include "tools/utils/matrix.h"
#include "tools/utils/windowspec.h"

#include "tools/gl/glplot.h"
#include "tools/gl/glplot3d.h"
#include "tools/gl/glviewer.h"

namespace gl
{
	enum struct GLInstance
	{
		NONE = 0,
		GLVIEWER = 1,
		GLPLOT = 2,
		GLPLOT3D = 3
	};

	/** 
		Providing a class which allows to visualize 2d-functions, 3d-function 
		and pointclouds
		
		The arrays for the function-plots can be an instance of:
			- std::vector<ElementType>
			- utils::Matrix<ElementType>
			- ElementType*
		
		The viewer copy the data from the specific container.

		The pointcloud can be an instance of:
			- pointcloud::Pointcloud<ElementType>
			- ElementType*

		An instance of a plotting object can be called with
			- utils::GLView<ElementType> glview

		This instance hold several plots which can be separate visualized.
		To set a viewer call
			- glview.setPlot(<number_of_elements>) : sets container for the function-plotter
			- glview.setPlot3D() : sets container for 3d-function-plotter
			- glview.setViewer() : sets container for the pointcloud viewer
		These functions have to be called otherwise the pointclouds and functions
		wont be assigned.

		To set the data call
			- glview.setX(<container>) : assign the functions to the plotter
			- glview.setY(<container>) : assign the functions to the plotter
			- glview.setPointcloud(<container>) : assign the data to the visualizer

			To draw elements in GLPlot3d call
				- glview.setPointcloud(mode, pointcloud)
				- glview.setPointcloud(mode, points, number_of_vertices)
				- glview.setPointcloud(mode, points, indices, number_of_vertices, number_of_indices)

			There are three possible modes for the primitives to be rendered
				- GLParams::POINTS
				- GLParams::LINES
				- GLParams::TRIANGLES

		With the function plot()
			- glview.plot()
		the data will be visualized.

		To keep up a window with the results call mainLoop()
			- glview.mainLoop()
		When there a other programs called which use OpenGL you have to
		call mainLoop() once.

		Example for visualization of a pointcloud:
		
		utils::GLView<ElementType> glview;

			glview.setViewer();
				pointcloud::Pointcloud<ElementType> pointcloud;
				*
				* Assigning values to pointcloud
				*
				glview.setPointlcoud(pointcloud),
			glview.subPlot(2, 2, 0)

			glview.setPlot3D();
				float* dataset;
				size_t number_of_vertices;
				unsigned int* lines;
				size_t number_of_lines;
				gl::glMeshGrid<float>(&dataset, -1, 1, -1, 1, number_of_vertices, &lines, number_of_lines);
				glview.setPointcloud(GLParams::LINES, dataset, lines, number_of_vertices, number_of_lines);
			glview.subPlot(2, 2, 1);
			delete[] dataset, lines;

			glview.setPlot(versuch);
				int number_of_elements;
				float* array_x(new float[number_of_elements]);
				float* array_y(new float[number_of_elements]);

				for (int i = 0; i < number_of_elements; i++) {
					array_x[i] = (float) i;
					array_y1[i] = (float) i;
				}
				glview.setY(array_y);
				glview.setX(array_x);
			glview.subPlot(2, 2, 2);

		glview.mainLoop();
	*/

	template<typename ElementType> class GLView
	{
	public:
		/**
			Constructor
		*/
		GLView()
		{
			int argc = 0;
			char** argv = nullptr;
			
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
		GLView(const GLView<ElementType>& glview) = delete;
		
		/**
			Copy constructor

			@param[in] glview_ An instance of class GLView
		*/
		GLView(const GLView<ElementType>&& glview) = delete;

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
			glviewer_.setViewer();

			current_instance_ = GLInstance::GLVIEWER;
		}

		/**
			Initializes a graph plot and sets a instance of a container, where the data can be assigned

			@param[in] number_of_elements_ Number of elements
		*/
		void setPlot(size_t number_of_elements_)
		{
			glplot_.setPlot(number_of_elements_);

			current_instance_ = GLInstance::GLPLOT;
		}

		/**
			Initializes a viewer for a plot in 3d and sets a instance of a container, where the data can be assigned
		*/
		void setPlot3D()
		{
			glplot3d_.setPlot3D();

			current_instance_ = GLInstance::GLPLOT3D;
		}

		/**
			Sets the x-data array for plotting

			@param[in] x_ x-values
		*/
		void setX(ElementType* x)
		{
			if (current_instance_ != GLInstance::GLPLOT) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot_.setX(x);
		}

		/**
			Sets the x-data array for plotting

			@param[in] y_ y-values
		*/
		void setX(std::vector<ElementType>& x)
		{
			if (current_instance_ != GLInstance::GLPLOT) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot_.setX(x);
		}
		
		/**
			Sets the x-data array for plotting

			@param[in] y_ y-values
		*/
		void setX(utils::Matrix<ElementType>& x)
		{
			if (current_instance_ != GLInstance::GLPLOT) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot_.setX(x);
		}

		/**
			Sets the y-data array for plotting

			@param[in] y_ y-values
		*/
		void setY(ElementType* y)
		{
			if (current_instance_ != GLInstance::GLPLOT) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot_.setY(y);
		}

		/**
			Sets the x-data array for plotting

			@param[in] y_ y-values
		*/
		void setY(std::vector<ElementType>& y)
		{
			if (current_instance_ != GLInstance::GLPLOT) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot_.setY(y);
		}
		
		/**
			Sets the x-data array for plotting

			@param[in] y_ y-values
		*/
		void setY(utils::Matrix<ElementType>& y)
		{
			if (current_instance_ != GLInstance::GLPLOT) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot_.setY(y);
		}

		/**
			Set pointcloud

			@param[in] pointcloud_ Pointcloud
		*/
		void setPointcloud(const pointcloud::Pointcloud<ElementType>& pointcloud)
		{
			if (current_instance_ != GLInstance::GLVIEWER) {
				exitFailure(__FILE__, __LINE__);
			}

			glviewer_.setPointcloud(pointcloud);
		}

		/**
			Set pointcloud

			@param[in] pointcloud_ Points
		*/
		void setPointcloud(const utils::Matrix<ElementType>& points)
		{
			if (current_instance_ != GLInstance::GLVIEWER) {
				exitFailure(__FILE__, __LINE__);
			}

			glviewer_.setPointcloud(points.getPtr(), points.getRows());
		}

		/**
			Set pointcloud

			@param[in] points_ Points
			@param[in] number_of_elements_ Number of elements
		*/
		void setPointcloud(ElementType* points, size_t number_of_elements) 
		{
			if (current_instance_ != GLInstance::GLVIEWER) {
				exitFailure(__FILE__, __LINE__);
			}

			glviewer_.setPointcloud(points, number_of_elements);
		}

		/**
			Set pointcloud

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		void setPointcloud(GLParams mode, const pointcloud::Pointcloud<ElementType>& pointcloud)
		{
			if (current_instance_ != GLInstance::GLPLOT3D) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot3d_.setPointcloud(mode, pointcloud);
		}
		
		/**
			Set pointcloud
			
			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points Points
		*/
		void setPointcloud(GLParams mode, const utils::Matrix<ElementType>& points)
		{
			if (current_instance_ != GLInstance::GLPLOT3D) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot3d_.setPointcloud(mode, points.getPtr(), points.getRows());
		}
		/**
			Set pointcloud

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of vertices
		*/
		void setPointcloud(GLParams mode, ElementType* points, size_t number_of_vertices)
		{
			if (current_instance_ != GLInstance::GLPLOT3D) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot3d_.setPointcloud(mode, points, number_of_vertices);
		}

		/**
			Set pointcloud

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] lines_ Lines
		*/
		void setPointcloud(GLParams mode, utils::Matrix<ElementType> points, 
			utils::Matrix<unsigned int> lines)
		{
			if (current_instance_ != GLInstance::GLPLOT3D) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot3d_.setPointcloud(mode, points.getPtr(), lines.getPtr(), points.getRows(), lines.getRows());
		}

		/**
			Set pointcloud

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] lines_ Lines
			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_lines_ Number of indices
		*/
		void setPointcloud(GLParams mode, ElementType* points, unsigned int* lines,
			size_t number_of_vertices, size_t number_of_lines)
		{
			if (current_instance_ != GLInstance::GLPLOT3D) {
				exitFailure(__FILE__, __LINE__);
			}

			glplot3d_.setPointcloud(mode, points, lines, number_of_vertices, number_of_lines);
		}

		/**
			Creates the window and sets the callback functions	
		*/
		void plot()
		{
			switch (current_instance_) {
			case GLInstance::GLVIEWER:	
				glviewer_.plot(); 
				break;
			case GLInstance::GLPLOT:	
				glplot_.plot(); 
				break;
			case GLInstance::GLPLOT3D:
				glplot3d_.plot();
				break;
			default:	
				exitFailure(__FILE__, __LINE__);
				break;
			}
			current_instance_ = GLInstance::NONE;
		}

		/**
			Creates a subwindow and sets the callback functions	
		*/
		void subPlot(size_t windows_x, size_t windows_y, size_t index)
		{
			size_t screen_width = (size_t) GetSystemMetrics(SM_CXSCREEN);
			size_t screen_height = (size_t) GetSystemMetrics(SM_CYSCREEN);

			size_t index_window_x = index % windows_x;
			size_t index_window_y = index / windows_x;

			size_t width = screen_width / windows_x;
			size_t height = screen_height / windows_y;

			size_t position_x = width * index_window_x;
			size_t position_y = height * index_window_y;

			utils::WindowSpec window_spec(position_x, position_y, width, height);

			switch (current_instance_) {
			case GLInstance::GLVIEWER:
				glviewer_.plot(nullptr, window_spec);
				break;
			case GLInstance::GLPLOT:
				glplot_.plot(nullptr, window_spec);
				break;
			case GLInstance::GLPLOT3D:
				glplot3d_.plot(nullptr, window_spec);
				break;
			default:
				exitFailure(__FILE__, __LINE__);
				break;
			}
			current_instance_ = GLInstance::NONE;
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
		static gl::GLPlot<ElementType> glplot_;
		
		/**
			Instance of viewer tool
		*/
		static gl::GLViewer<ElementType> glviewer_;

		/**
			Instance of plotting in 3d
		*/
		static gl::GLPlot3D<ElementType> glplot3d_;

		/**
			Current instance
		*/
		GLInstance current_instance_;
	};

	/**
		Static variable GLView<ElementType>::glplot
	*/
	template<typename ElementType> gl::GLPlot<ElementType> GLView<ElementType>::glplot_;

	/**
		Static variable GLView<ElementType>::glviewer
	*/
	template<typename ElementType> gl::GLViewer<ElementType> GLView<ElementType>::glviewer_;

	/**
		Static variable GLPlot3D<ElementType>::glplot3d
	*/
	template<typename ElementType> gl::GLPlot3D<ElementType> GLView<ElementType>::glplot3d_;

}

#endif /* GL_PARAMETERS_H_ */