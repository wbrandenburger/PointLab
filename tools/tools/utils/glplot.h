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

#ifndef UTILS_GLPLOT_H_
#define UTILS_GLPLOT_H_

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include "eigen3/Eigen/Dense"

#include "tools/utils/matrix.h"

namespace utils
{
	template<typename ElementType> class PlotFunction
	{
	public:

		/**
		Constructor
		*/
		PlotFunction() : index_left(0.0f), index_right(0.0f), 
			number_of_functions(NULL), number_of_elements(NULL),zero(NULL) {}

		/**
		Destructor
		*/
		~PlotFunction() {}

		/**
		Set y-values

		@param[in] y_ y-values
		*/
		void setY(const std::vector<ElementType>& y_)
		{
			y.push_back(y_);
			number_of_functions++;

			if (!number_of_elements) {
				number_of_elements = y[0].size();

				index_left = 0;
				index_right = number_of_elements;
			}
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(const std::vector<ElementType>& x_)
		{
			x = x_;

			if (!number_of_elements) {
				number_of_elements = x.size();

				index_left = 0;
				index_right = number_of_elements;
			}

			if (x[index_left] < 0 && x[index_right - 1] >= 0) {
				for (size_t i = index_left + 1; i < index_right; i++) {
					if (x[i - 1] < 0 && x[i] >= 0) {
						zero = i;
					}
				}
			}
		}

		/**
			Zoom in
		*/
		void zoomIn(int x_, int y_, int width_, int height_)
		{
			float zoom_factor = 0.05f;

			size_t temp_index_left = (size_t)index_left + (float)x_ / float(width_) * zoom_factor*((float)index_right - (float)index_left);
			size_t temp_index_right = (size_t)index_right - (float(width_) - (float)x_) / float(width_) * zoom_factor*((float)index_right - (float)index_left);

			index_left = temp_index_left;
			index_right = temp_index_right;

		}

		/**
			Zoom in
		*/
		void zoomOut()
		{
			float zoom_factor = 0.05f;

			index_left = (size_t)index_left - zoom_factor * (float)index_left;
			index_right = (size_t)index_right + zoom_factor * (number_of_elements - index_right);
		}

		/**
			Set zero
		*/
		void setZero() 
		{

		}

		/**
			Draw the functions
		*/
		void draw()
		{
			glPushMatrix(); /* GL_MODELVIEW is default */

			ElementType y2 = y[0][index_left];
			ElementType y1 = y[0][index_left];
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = index_left; j < index_right; j++) {
					y2 = y2 < y[i][j] ? y[i][j] : y2;
					y1 = y1 > y[i][j] ? y[i][j] : y1;
				}
			}

			glScalef(1.0 / (index_right - index_left), 1.0 / (y2 - y1), 1.0);
			glTranslatef(0.0, -y1, 0.0);
			
			for (size_t i = 0; i < number_of_functions; i++) {

				float r, g, b;
				utils::colorSchemeRGB(r, g, b, i, number_of_functions);

				glColor3f(r, g, b);

				glBegin(GL_LINE_STRIP);

				for (size_t j = index_left; j < index_right; j++) {
					glVertex2f(j-index_left, y[i][j]);
				}

				glEnd();
			}

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
			y-values
		*/
		std::vector<std::vector<ElementType>> y;

		/**
			x-values
		*/
		std::vector<ElementType> x;

		/**
			Zero;
		*/
		float zero;
	};


	template<typename ElementType> struct Plots;

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
			Set  a new plot
		*/
		void setPlot()
		{
			plots.plot.push_back(new PlotFunction<ElementType>);
			plots.current_plot = plots.number_of_plots;
			
			plots.number_of_plots++;
		}

		/**
			Set  current plot
		*/
		void setPlot(size_t current_plot_)
		{
			plots.current_plot = current_plot_;
		}

		/**
			Set y-values

			@param[in] y_ y-values	
		*/
		void setY(const std::vector<ElementType>& y_)
		{
			(*plots.plot[plots.current_plot]).setY(y_);
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(const std::vector<ElementType>& x_)
		{
			(*plots.plot[plots.current_plot]).setX(x_);
		}

		/* Redrawing func */
		static void redraw(void)
		{
			glutSetWindow(plots.current_plot + 1);

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity(); 

			(*plots.plot[plots.current_plot]).draw();

			glutSwapBuffers();
		};

		///* Idle proc. Redisplays, if called. */
		static void idle(void)
		{
			glutPostRedisplay();
		};

		static void reshape(int w, int h)
		{
			glViewport(0, 0, w, h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 1, 0, 1, -1, 1);
			glMatrixMode(GL_MODELVIEW);
		}

		void plot()
		{
			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

			std::ostringstream os;
			os << "Window " << plots.number_of_plots - 1;
			std::string ostring = os.str();

			glutCreateWindow(ostring.c_str());

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
				Init the GL state
			*/
			glLineWidth(2.0);
		}

		/**
			Callback for keyboard

			@param[in] key_ Key
			@param[in] x_ Mouse position in x-direction
			@param[in] y_ Mouse position in y-direction
		*/
		static void key(unsigned char key_, int x_, int y_)
		{
			if (key_ == 27) { exit(0); }
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
				plots.current_plot = glutGetWindow() - 1;
				(*plots.plot[plots.current_plot]).zoomIn(x_, y_, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			}
			else {
				plots.current_plot = glutGetWindow() - 1;
				(*plots.plot[plots.current_plot]).zoomOut();
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
		}


	public:

		/**
			Several Plots		
		*/
		static Plots<ElementType> plots;
	};



	/**
		Plots
	*/
	template<typename ElementType> struct Plots{
	
	public:

		std::vector<PlotFunction<ElementType>*> plot;

		size_t  number_of_plots;
		/**
			Current plot
		*/
		size_t current_plot;

		/**
			Constructor
		*/
		Plots() : number_of_plots(0), current_plot(NULL) {
			clear();
		}

		/**
			Destructor
		*/
		~Plots() 
		{
			clear();
		}

		/**
			Clear
		*/
		void clear () {
			if (number_of_plots) {
				for (size_t i = 0; i < number_of_plots; i++) {
					delete plot[i];
				}
			}
			number_of_plots = 0;
			current_plot = NULL;
		}

	};

	template<typename ElementType> Plots<ElementType> GLPlot<ElementType>::plots;

}

#endif /* UTILS_GLPLOT_H_*/	