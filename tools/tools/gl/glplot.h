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

#ifndef GL_GLPLOT_H_
#define GL_GLPLOT_H_

#include <map>
#include <vector>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include "tools/utils/matrix.h"
#include "tools/utils/windowspec.h"

namespace gl
{
	template<typename ElementType> class PlotFunction
	{
	public:
		/**
			Constructor

			@param[in] number_of_elements_ Number of elements
		*/
		PlotFunction(size_t number_of_elements_) : index_left_(0.0f), index_right_(0.0f),
			number_of_functions_(NULL), number_of_elements_(number_of_elements_), zero_(NULL), x_(nullptr), clear_x_(false)
		{
		}
		
		/**
			Destructor
		*/
		~PlotFunction() 
		{
			clearMemory();
		}

		/**
			Clear
		*/
		void clearMemory()
		{
			index_right_ = 0.0f;
			index_left_ = 0.0f;
			number_of_functions_ = NULL;
			number_of_elements_ = NULL;

			if (clear_x_) {
				delete[] x_;
				x_ = nullptr;
			}
			clear_x_ = false;
			
			zero_ = NULL;
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(ElementType* y)
		{
			y_.push_back(y);
			number_of_functions_++;

			setParametersY();
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(std::vector<ElementType>& y)
		{
			y_.push_back((ElementType*)&y[0]);
			number_of_functions_++;

			setParametersY();
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(utils::Matrix<ElementType>& y)
		{
			y_.push_back((ElementType*)y.getPtr());
			number_of_functions_++;

			setParametersY();
		}

		/**
			Set parameters
		*/
		void setParametersY()
		{
			index_left_ = 0;
			index_right_ = (float)number_of_elements_;
		}

		/**
			Set x-values
		*/
		void setX()
		{
			x_ = new ElementType[number_of_elements_];
			for (size_t i = 0; i < number_of_elements_; i++) {
				x_[i] = i;
			}

			clear_x_ = true;

			setParametersX();
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(ElementType* x)
		{
			x_ = x;

			setParametersX();
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(std::vector<ElementType>& x)
		{
			x_ = (ElementType*) &x[0];

			setParametersX();
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(utils::Matrix<ElementType>& x)
		{
			x_ = (ElementType*) x.getPtr();

			setParametersX();
		}

		/**
			Set parameters
		*/
		void setParametersX()
		{
			index_left_ = 0;
			index_right_ = (float)number_of_elements_;

			if (x_[(size_t)index_left_] < 0 && x_[(size_t)index_right_ - 1] >= 0) {
				for (size_t i = (size_t)index_left_ + 1; i < (size_t)index_right_; i++) {
					if (x_[i - 1] < 0 && x_[i] >= 0) {
						zero_ = (float)i;
					}
				}
			}
		}	

		/**
			Zoom in

			@param[in] x_ Mouse position in x-direction
			@param[in] y_ Mouse position in y-direction
			@param[in] width_ Current width of the window
			@param[in] height_ Current height of the window
		*/
		void zoomIn(int x, int y, int width, int height)
		{
			float zoom_factor = 0.05f;

			index_left_ = index_left_ + (float)x / float(width) * zoom_factor*(index_right_ - index_left_);
			index_right_ = index_right_ - ((float)width - (float)x) / float(width) * zoom_factor*(index_right_ - index_left_);
		}

		/**
			Zoom Out
		*/
		void zoomOut()
		{
			float zoom_factor = 0.05f;

			index_left_ = (size_t)index_left_ - zoom_factor * (float)index_left_;
			index_right_ = (size_t)index_right_ + zoom_factor * (number_of_elements_ - index_right_);
		}
		/**
			Get mouse position in x-direction

			@param[in] x_ Mouse position in x-direction
			@param[in] width_ Current width of the window
		*/
		ElementType getX(int x, int width)
		{
			return  x_[(size_t)index_left_ + (size_t) ((index_right_ - index_left_) * (ElementType)x / (ElementType)width)];
		}

		/**
			Get mouse position in y-direction

			@param[in] y_ Mouse position in y-direction
			@param[in] height_ Current height of the window
		*/
		ElementType getY(int y, int height)
		{
			/**
				Minimum and maximum y-value
			*/
			ElementType y2 = y_[0][(size_t)index_left_];
			ElementType y1 = y_[0][(size_t)index_left_];
			for (size_t i = 0; i < number_of_functions_; i++) {
				for (size_t j = (size_t)index_left_; j < (size_t)index_right_; j++) {
					y2 = y2 < y_[i][j] ? y_[i][j] : y2;
					y1 = y1 > y_[i][j] ? y_[i][j] : y1;
				}
			}
			return  y1 + (y2 - y1)*((ElementType)height - (ElementType)y) / (ElementType) height;
		}

		/**
			Draw the functions
		*/
		void draw()
		{
			glPushMatrix(); /* GL_MODELVIEW is default */

			/**
				Minimum and maximum y-value
			*/
			ElementType y2 = y_[0][(size_t)index_left_];
			ElementType y1 = y_[0][(size_t)index_left_];
			for (size_t i = 0; i < number_of_functions_; i++) {
				for (size_t j = (size_t)index_left_; j < (size_t)index_right_; j++) {
					y2 = y2 < y_[i][j] ? y_[i][j] : y2;
					y1 = y1 > y_[i][j] ? y_[i][j] : y1;
				}
			}

			/**
				Draw the functions
			*/
			glScalef(1.0f / (index_right_ - index_left_), 1.0f / (y2 - y1), 1.0f);
			glTranslatef(0.0f, -y1, 0.0f);
			
			/**
				Init the GL state
			*/
			glLineWidth(1.0f);

			for (size_t i = 0; i < number_of_functions_; i++) {

				float r, g, b;
				utils::colorSchemeRGB(r, g, b, i, number_of_functions_);

				glColor3f(r, g, b);
				
				glBegin(GL_LINE_STRIP);

				for (size_t j = (size_t) index_left_; j < (size_t) index_right_; j++) {
					glVertex2f((float)j-index_left_, y_[i][j]);
				}

				glEnd();
			}

			/**
				Draw the axis
			*/
			glColor3f(1.0f, 1.0f, 1.0f);

			glBegin(GL_LINE_STRIP);
			glVertex2f(zero_ - index_left_, y2);
			glVertex2f(zero_ - index_left_, y1);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(0, 0);
			glVertex2f(index_right_-index_left_, 0);
			glEnd();
			glPopMatrix();
		}

	private: 

		/**
			Current left index
		*/
		float index_left_;

		/**
			Current right index
		*/
		float index_right_;
		
		/**
			Number of functions
		*/
		size_t number_of_functions_;

		/**
			Number of elements
		*/
		size_t number_of_elements_;

		/**
			Zero;
		*/
		float zero_;	

		/**
			y-values
		*/
		std::vector<ElementType*> y_;

		/**
			x-values
		*/
		ElementType* x_;

		/**
			Flag which defines whether x has to be cleared
		*/
		bool clear_x_;
	};

	/**
		Forward declaration of class StaticPlotInstance
	*/
	template<typename ElementType> class StaticPlotInstance;

	template<typename ElementType> class GLPlot
	{

	public:

		/**
			Constructor
		*/
		GLPlot()
		{
		}

		/**
			Destructor
		*/
		~GLPlot()
		{
			clearMemory();
		}

		/**
			Clear
		*/
		void clearMemory() {}

		/**
			Set new plot
		*/
		void setPlot(size_t number_of_elements)
		{
			plot_instances_.setPlot(number_of_elements);
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(ElementType* y)
		{
			plot_instances_.getCurrentInstanceFunction().setY(y);
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(std::vector<ElementType>& y)
		{
			plot_instances_.getCurrentInstanceFunction().setY(y);
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(utils::Matrix<ElementType>& y)
		{
			plot_instances_.getCurrentInstanceFunction().setY(y);
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/

		void setX(ElementType* x)
		{
			plot_instances_.getCurrentInstanceFunction().setX(x);
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(std::vector<ElementType>& x)
		{
			plot_instances_.getCurrentInstanceFunction().setX(x);
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(utils::Matrix<ElementType>& x)
		{
			plot_instances_.getCurrentInstanceFunction().setX(x);
		}

		/**
			Redrawing function
		*/
		static void redraw(void)
		{
			glutSetWindow((int)plot_instances_.getCurrentWindow() + 1);

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			plot_instances_.getCurrentInstanceFunction().draw();

			glutSwapBuffers();

		};

		/**
			Redisplays, if called
		*/
		static void idle(void)
		{
			glutPostRedisplay();
		};

		/**
			Changes the size of the window

			@param[in] width_ The new Width of the window
			@param[in] height_ The new Height of the window
		*/
		static void reshape(int width, int height)
		{
			glutSetWindow(glutGetWindow());

			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
			glMatrixMode(GL_MODELVIEW);
		}

		/**
			Creates the window and sets the callback functions

			@param[in] window_name_ Name of the window
		*/
		void plot(char* window_name = nullptr, utils::WindowSpec window_spec = utils::WindowSpec())
		{
			if (!window_name) {
				window_name = new char[10];
				sprintf(window_name, "Plot %d", (int)plot_instances_.getNumberOfPlots() - 1);
			}

			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

			glutInitWindowSize((int)window_spec.getWidth(), (int)window_spec.getHeight());
			glutInitWindowPosition((int)window_spec.getPositionX(), (int)window_spec.getPositionY());

			size_t window_index = glutCreateWindow(window_name) - 1;
			plot_instances_.setWindowIndex(window_index);
			plot_instances_.setCurrentInstance(window_index);

			/**
				Register GLUT callbacks.
			*/
			glutDisplayFunc(redraw);
			glutKeyboardFunc(key);
			glutReshapeFunc(reshape);
			glutIdleFunc(idle);
			glutMouseWheelFunc(mouseWheel);
			glutMouseFunc(mouseFunc);

			/**
				Draw the initial image
			*/
			reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			redraw();
		}

		/**
			Callback for keyboard

			@param[in] key_ Key
			@param[in] x_ Mouse position in x-direction
			@param[in] y_ Mouse position in y-direction
		*/
		static void key(unsigned char key, int x, int y)
		{
			switch (key) {
			case 27: glutLeaveMainLoop(); break;
			}
		};

		/**
			Callback for mouse wheel

			@param[in] button_
			@param[in] direction_ Direction in which the wheel is turned: 1 for up and -1 for down
			@param[in] x_ Mouse position in x-direction
			@param[in] y_ Mouse position in y-direction
		*/
		static void mouseWheel(int button, int direction, int x, int y)
		{
			if (direction == 1) {
				plot_instances_.setCurrentInstance(glutGetWindow() - 1);
				plot_instances_.getCurrentInstanceFunction().zoomIn(x, y, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			}
			else {
				plot_instances_.setCurrentInstance(glutGetWindow() - 1);
				plot_instances_.getCurrentInstanceFunction().zoomOut();
			}
		}

		/**
			Callback for mouse

			@param[in] button_ Button which is pushed: 0 for left, 1 for middle and 2 for right
			@param[in] state_ Whether the button is released: 0 for pushing and 1 for releasing
			@param[in] x_ Mouse position in x-direction
			@param[in] y_ Mouse position in y-direction
		*/
		static void mouseFunc(int button, int state, int x, int y)
		{
			if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
				plot_instances_.setCurrentInstance(glutGetWindow() - 1);
				std::cout << plot_instances_.getCurrentInstanceFunction().getX(x, glutGet(GLUT_WINDOW_WIDTH)) << " "
					<< plot_instances_.getCurrentInstanceFunction().getY(y, glutGet(GLUT_WINDOW_HEIGHT)) << std::endl;
			}
		}

		/**
			Start the loop
		*/
		void mainLoop()
		{
			glutMainLoop();
		}

	public:

		/**
			Structure where the different plots are organized
		*/
		static StaticPlotInstance<ElementType> plot_instances_;
	};

	/**
		Plot Instance
	*/
	template<typename ElementType> class StaticPlotInstance
	{
	public:

		/**
			Constructor
		*/
		StaticPlotInstance(void) : number_of_plots_(0), current_instance_(NULL), current_window_(NULL)
		{
		}

		/**
			Destructor
		*/
		~StaticPlotInstance() 
		{
			clearMemory();
		}


		/**
			Clear
		*/
		void clearMemory() {
			plot_instances_.clear();
			plot_indices_.clear();
			number_of_plots_ = 0;
			current_instance_ = NULL;
			current_window_ = NULL;
		}
		/**
			Set plot

			@param[in] number_of_elements_ Number of elements
		*/
		void setPlot(size_t number_of_elements)
		{
			plot_instances_.push_back(PlotFunction<ElementType>(number_of_elements));

			current_instance_ = number_of_plots_;
			number_of_plots_++;
		}

		/**
			Get number of plots
		*/
		size_t getNumberOfPlots()
		{
			return number_of_plots_;
		}

		/**
			Set number of plots
		*/
		void setNumberOfPlots(size_t number_of_plots)
		{
			number_of_plots_ = number_of_plots;
		}

		/**
			Get current plot
		*/
		size_t getCurrentInstance()
		{
			return current_instance_;
		}

		/**
			Set current plot
		*/
		void setCurrentInstance(size_t current_window)
		{
			current_instance_ = plot_indices_[current_window];
			current_window_ = current_window;
		}

		/**
			Get current window
		*/
		size_t getCurrentWindow() {
			return current_window_;
		}

		/**
			Assign a window index with an plot index
		*/
		void setWindowIndex(size_t window_index)
		{
			plot_indices_[window_index] = number_of_plots_ - 1;
		}

		/**
			Get current plotfunction
		*/
		PlotFunction<ElementType>& getCurrentInstanceFunction()
		{
			return plot_instances_[current_instance_];
		}

		/**
			Operator()

			@	return plot Structure with plots
		*/
		std::vector<PlotFunction<ElementType>>& operator()()
		{
			return plot_instances_;
		}

	private:
		/**
			Number of plots
		*/
		size_t  number_of_plots_;

		/**
			Current plot
		*/
		size_t current_instance_;

		/**
			Current window
		*/
		size_t current_window_;

		/**
			Container which assign a instance to the windows
		*/
		std::map<size_t, size_t> plot_indices_;

		/**
			Structure with all plots
		*/
		std::vector<PlotFunction<ElementType>> plot_instances_;
	};

	/**
		Static variable GLPlot<ElementType>::plot_instances
	*/
	template<typename ElementType> StaticPlotInstance<ElementType> GLPlot<ElementType>::plot_instances_;
}

#endif /* GL_GLPLOT_H_*/	