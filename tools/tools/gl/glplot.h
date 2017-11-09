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
		PlotFunction(size_t number_of_elements_) : index_left(0.0f), index_right(0.0f),
			number_of_functions(NULL), number_of_elements(number_of_elements_), zero(NULL), x(nullptr), clear_x(false) {}
		
		/**
			Destructor
		*/
		~PlotFunction() 
		{
			clear();
		}

		/**
			Clear
		*/
		void clear()
		{
			index_right = 0.0f;
			index_left = 0.0f;
			number_of_functions = NULL;
			number_of_elements = NULL;

			if (clear_x) {
				delete[] x;
				x = nullptr;
			}
			clear_x = false;
			
			zero = NULL;
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(ElementType* y_)
		{
			y.push_back(y_);
			number_of_functions++;

			setParametersY();
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(std::vector<ElementType>& y_)
		{
			y.push_back((ElementType*)&y_[0]);
			number_of_functions++;

			setParametersY();
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(utils::Matrix<ElementType>& y_)
		{
			y.push_back((ElementType*)y_.getPtr());
			number_of_functions++;

			setParametersY();
		}

		/**
			Set parameters
		*/
		void setParametersY()
		{
			index_left = 0;
			index_right = (float)number_of_elements;
		}

		/**
			Set x-values
		*/
		void setX()
		{
			x = new ElementType[number_of_elements];
			for (size_t i = 0; i < number_of_elements; i++) {
				x[i] = i;
			}

			clear_x = true;

			setParametersX();
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(ElementType* x_)
		{
			x = x_;

			setParametersX();
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(std::vector<ElementType>& x_)
		{
			x = (ElementType*) &x_[0];

			setParametersX();
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(utils::Matrix<ElementType>& x_)
		{
			x = (ElementType*) x_.getPtr();

			setParametersX();
		}

		/**
			Set parameters
		*/
		void setParametersX()
		{
			index_left = 0;
			index_right = (float)number_of_elements;

			if (x[(size_t)index_left] < 0 && x[(size_t)index_right - 1] >= 0) {
				for (size_t i = (size_t)index_left + 1; i < (size_t)index_right; i++) {
					if (x[i - 1] < 0 && x[i] >= 0) {
						zero = (float)i;
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
		void zoomIn(int x_, int y_, int width_, int height_)
		{
			float zoom_factor = 0.05f;

			index_left = index_left + (float)x_ / float(width_) * zoom_factor*(index_right - index_left);
			index_right = index_right - ((float)width_ - (float)x_) / float(width_) * zoom_factor*(index_right - index_left);
		}

		/**
			Zoom Out
		*/
		void zoomOut()
		{
			float zoom_factor = 0.05f;

			index_left = (size_t)index_left - zoom_factor * (float)index_left;
			index_right = (size_t)index_right + zoom_factor * (number_of_elements - index_right);
		}
		/**
			Get mouse position in x-direction

			@param[in] x_ Mouse position in x-direction
			@param[in] width_ Current width of the window
		*/
		ElementType getX(int x_, int width_)
		{
			return  x[(size_t)index_left + (size_t) ((index_right - index_left) * (ElementType)x_ / (ElementType)width_)];
		}

		/**
			Get mouse position in y-direction

			@param[in] y_ Mouse position in y-direction
			@param[in] height_ Current height of the window
		*/
		ElementType getY(int y_, int heigth_)
		{
			/**
				Minimum and maximum y-value
			*/
			ElementType y2 = y[0][(size_t)index_left];
			ElementType y1 = y[0][(size_t)index_left];
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = (size_t)index_left; j < (size_t)index_right; j++) {
					y2 = y2 < y[i][j] ? y[i][j] : y2;
					y1 = y1 > y[i][j] ? y[i][j] : y1;
				}
			}
			return  y1 + (y2 - y1)*((ElementType)heigth_ - (ElementType)y_) / (ElementType) heigth_;
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
			ElementType y2 = y[0][(size_t)index_left];
			ElementType y1 = y[0][(size_t)index_left];
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = (size_t)index_left; j < (size_t)index_right; j++) {
					y2 = y2 < y[i][j] ? y[i][j] : y2;
					y1 = y1 > y[i][j] ? y[i][j] : y1;
				}
			}

			/**
				Draw the functions
			*/
			glScalef(1.0f / (index_right - index_left), 1.0f / (y2 - y1), 1.0f);
			glTranslatef(0.0f, -y1, 0.0f);
			
			/**
				Init the GL state
			*/
			glLineWidth(1.0f);

			for (size_t i = 0; i < number_of_functions; i++) {

				float r, g, b;
				utils::colorSchemeRGB(r, g, b, i, number_of_functions);

				glColor3f(r, g, b);

				glBegin(GL_LINE_STRIP);

				for (size_t j = (size_t) index_left; j < (size_t) index_right; j++) {
					glVertex2f((float)j-index_left, y[i][j]);
				}

				glEnd();
			}

			/**
				Draw the axis
			*/

			glColor3f(1.0f, 1.0f, 1.0f);

			glBegin(GL_LINE_STRIP);
			glVertex2f(zero - index_left, y2);
			glVertex2f(zero - index_left, y1);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(0, 0);
			glVertex2f(index_right-index_left, 0);
			glEnd();
			glPopMatrix();
		}

	private: 

		/**
			Current left index
		*/
		float index_left;

		/**
			Current right index
		*/
		float index_right;
		
		/**
			Number of functions
		*/
		size_t number_of_functions;

		/**
			Number of elements
		*/
		size_t number_of_elements;

		/**
			Zero;
		*/
		float zero;	

		/**
			y-values
		*/
		std::vector<ElementType*> y;

		/**
			x-values
		*/
		ElementType* x;

		/**
			Flag which defines whether x has to be cleared
		*/
		bool clear_x;
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


			clear();
		}

		/**
			Destructor
		*/
		~GLPlot()
		{
			clear();
		}

		/**
			Clear
		*/
		void clear() {}

		/**
			Set new plot
		*/
		void setPlot(size_t number_of_elements_)
		{
			plot_instances.setPlot(number_of_elements_);
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(ElementType* y_)
		{
			plot_instances.getCurrentInstanceFunction().setY(y_);
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(std::vector<ElementType>& y_)
		{
			plot_instances.getCurrentInstanceFunction().setY(y_);
		}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(utils::Matrix<ElementType>& y_)
		{
			plot_instances.getCurrentInstanceFunction().setY(y_);
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/

		void setX(ElementType* x_)
		{
			plot_instances.getCurrentInstanceFunction().setX(x_);
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(std::vector<ElementType>& x_)
		{
			plot_instances.getCurrentInstanceFunction().setX(x_);
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(utils::Matrix<ElementType>& x_)
		{
			plot_instances.getCurrentInstanceFunction().setX(x_);
		}

		/**
			Redrawing function
		*/
		static void redraw(void)
		{
			glutSetWindow((int)plot_instances.getCurrentWindow() + 1);

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			plot_instances.getCurrentInstanceFunction().draw();

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
		static void reshape(int width_, int height_)
		{
			glutSetWindow(glutGetWindow());

			glViewport(0, 0, width_, height_);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
			glMatrixMode(GL_MODELVIEW);
		}

		/**
			Creates the window and sets the callback functions

			@param[in] window_name_ Name of the window
		*/
		void plot(char* window_name_ = nullptr, utils::WindowSpec window_spec = utils::WindowSpec())
		{
			if (!window_name_) {
				window_name_ = new char[10];
				sprintf(window_name_, "Plot %d", (int)plot_instances.getNumberOfPlots() - 1);
			}

			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

			glutInitWindowSize((int)window_spec.getWidth(), (int)window_spec.getHeight());
			glutInitWindowPosition((int)window_spec.getPositionX(), (int)window_spec.getPositionY());

			size_t window_index = glutCreateWindow(window_name_) - 1;
			plot_instances.setWindowIndex(window_index);
			plot_instances.setCurrentInstance(window_index);

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
		static void key(unsigned char key_, int x_, int y_)
		{
			switch (key_) {
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
		static void mouseWheel(int button_, int direction_, int x_, int y_)
		{
			if (direction_ == 1) {
				plot_instances.setCurrentInstance(glutGetWindow() - 1);
				plot_instances.getCurrentInstanceFunction().zoomIn(x_, y_, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			}
			else {
				plot_instances.setCurrentInstance(glutGetWindow() - 1);
				plot_instances.getCurrentInstanceFunction().zoomOut();
			}
		}

		/**
			Callback for mouse

			@param[in] button_ Button which is pushed: 0 for left, 1 for middle and 2 for right
			@param[in] state_ Whether the button is released: 0 for pushing and 1 for releasing
			@param[in] x_ Mouse position in x-direction
			@param[in] y_ Mouse position in y-direction
		*/
		static void mouseFunc(int button_, int state_, int x_, int y_)
		{
			if (button_ == GLUT_LEFT_BUTTON && state_ == GLUT_UP) {
				plot_instances.setCurrentInstance(glutGetWindow() - 1);
				std::cout << plot_instances.getCurrentInstanceFunction().getX(x_, glutGet(GLUT_WINDOW_WIDTH)) << " "
					<< plot_instances.getCurrentInstanceFunction().getY(y_, glutGet(GLUT_WINDOW_HEIGHT)) << std::endl;
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
		static StaticPlotInstance<ElementType> plot_instances;
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
		StaticPlotInstance(void) : number_of_plots(0), current_instance(NULL), current_window(NULL)
		{
		}

		/**
			Destructor
		*/
		~StaticPlotInstance() 
		{
			clear();
		}


		/**
			Clear
		*/
		void clear() {
			plot_instances.clear();
			plot_indices.clear();
			number_of_plots = 0;
			current_instance = NULL;
			current_window = NULL;
		}
		/**
			Set plot

			@param[in] number_of_elements_ Number of elements
		*/
		void setPlot(size_t number_of_elements_)
		{
			plot_instances.push_back(PlotFunction<ElementType>(number_of_elements_));

			current_instance = number_of_plots;
			number_of_plots++;
		}

		/**
			Get number of plots
		*/
		size_t getNumberOfPlots()
		{
			return number_of_plots;
		}

		/**
			Set number of plots
		*/
		void setNumberOfPlots(size_t number_of_plots_)
		{
			number_of_plots = number_of_plots_;
		}

		/**
			Get current plot
		*/
		size_t getCurrentInstance()
		{
			return current_instance;
		}

		/**
			Set current plot
		*/
		void setCurrentInstance(size_t current_window_)
		{
			current_instance = plot_indices[current_window_];
			current_window = current_window_;
		}

		/**
			Get current window
		*/
		size_t getCurrentWindow() {
			return current_window;
		}

		/**
			Assign a window index with an plot index
		*/
		void setWindowIndex(size_t window_index_)
		{
			plot_indices[window_index_] = number_of_plots - 1;
		}

		/**
			Get current plotfunction
		*/
		PlotFunction<ElementType>& getCurrentInstanceFunction()
		{
			return plot_instances[current_instance];
		}

		/**
			Operator()

			@	return plot Structure with plots
		*/
		std::vector<PlotFunction<ElementType>>& operator()()
		{
			return plot_instances;
		}

	private:
		/**
			Number of plots
		*/
		size_t  number_of_plots;

		/**
			Current plot
		*/
		size_t current_instance;

		/**
			Current window
		*/
		size_t current_window;

		/**
			Container which assign a instance to the windows
		*/
		std::map<size_t, size_t> plot_indices;

		/**
			Structure with all plots
		*/
		std::vector<PlotFunction<ElementType>> plot_instances;
	};

	/**
		Static variable GLPlot<ElementType>::plot_instances
	*/
	template<typename ElementType> StaticPlotInstance<ElementType> GLPlot<ElementType>::plot_instances;
}

#endif /* GL_GLPLOT_H_*/	