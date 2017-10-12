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

#include <vector>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include "eigen3/Eigen/Dense"

#include "tools/utils/matrix.h"

namespace utils
{
	class PlotFunction
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
			Zoom in

			@param[in] x_ Mouse position in x-direction
			@param[in] y_ Mouse position in y-direction
			@param[in] width_ Current width of the window
			@param[in] height_ Current height of the window
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
			Zoom Out
		*/
		void zoomOut()
		{
			float zoom_factor = 0.05f;

			index_left = (size_t)index_left - zoom_factor * (float)index_left;
			index_right = (size_t)index_right + zoom_factor * (number_of_elements - index_right);
		}

	protected: 

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
	};

	template<typename ElementType> class PlotFunctionVector : public PlotFunction
	{
	public:

		/**
			Constructor
		*/
		PlotFunctionVector() {}

		/**
			Destructor
		*/
		~PlotFunctionVector() {}

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
			Get mouse position in x-direction

			@param[in] x_ Mouse position in x-direction
			@param[in] width_ Current width of the window
		*/
		ElementType getX(int x_, int width_)
		{
			return  x[index_left + (index_right - index_left) * (ElementType)x_ / (ElementType)width_];
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
			ElementType y2 = y[0][index_left];
			ElementType y1 = y[0][index_left];
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = index_left; j < index_right; j++) {
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
			ElementType y2 = y[0][index_left];
			ElementType y1 = y[0][index_left];
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = index_left; j < index_right; j++) {
					y2 = y2 < y[i][j] ? y[i][j] : y2;
					y1 = y1 > y[i][j] ? y[i][j] : y1;
				}
			}

			/**
				Draw the functions
			*/
			glScalef(1.0 / (index_right - index_left), 1.0 / (y2 - y1), 1.0);
			glTranslatef(0.0, -y1, 0.0);
			
			/**
				Init the GL state
			*/
			glLineWidth(1.0f);

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
			y-values
		*/
		std::vector<std::vector<ElementType>> y;

		/**
			x-values
		*/
		std::vector<ElementType> x;
	};

	template<typename ElementType> class PlotFunctionMatrix : public PlotFunction
	{
	public:

		/**
			Constructor
		*/
		PlotFunctionMatrix() : index_left(0.0f), index_right(0.0f), 
			number_of_functions(NULL), number_of_elements(NULL),zero(NULL) {}

		/**
			Destructor
		*/
		~PlotFunctionMatrix() {}

		/**
			Set y-values

			@param[in] y_ y-values
		*/
		void setY(const utils::Matrix<ElementType>& y_)
		{
			y.push_back(y_);
			number_of_functions++;

			if (!number_of_elements) {
				number_of_elements = y[0].getRows();

				index_left = 0;
				index_right = number_of_elements;
			}
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(const utils::Matrix<ElementType>& x_)
		{
			x = x_;

			if (!number_of_elements) {
				number_of_elements = x.getRows();

				index_left = 0;
				index_right = number_of_elements;
			}

			if (x[index_left][0] < 0 && x[index_right - 1][0] >= 0) {
				for (size_t i = index_left + 1; i < index_right; i++) {
					if (x[i - 1][0]  < 0 && x[i][0] >= 0) {
						zero = i;
					}
				}
			}
		}

		/**
			Get mouse position in x-direction

			@param[in] x_ Mouse position in x-direction
			@param[in] width_ Current width of the window
		*/
		ElementType getX(int x_, int width_)
		{
			return  x[index_left + (index_right - index_left) * (ElementType)x_ / (ElementType)width_][0];
		}

		/**
			Get mouse position in y-direction

			@param[in] y_ Mouse position in x-direction
			@param[in] height_ Current heigth of the window
		*/
		float getY(int y_, int heigth_)
		{
			/**
				Minimum and maximum y-value
			*/
			ElementType y2 = y[0][index_left][0];
			ElementType y1 = y[0][index_left][0];
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = index_left; j < index_right; j++) {
					y2 = y2 < y[i][j][0] ? y[i][j][0] : y2;
					y1 = y1 > y[i][j][0] ? y[i][j][0] : y1;
				}
			}
			return  y1 + (y2 - y1)* ((ElementType)heigth_ - (ElementType)y_) / (ElementType) heigth_;
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
			ElementType y2 = y[0][index_left][0];
			ElementType y1 = y[0][index_left][0];
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = index_left; j < index_right; j++) {
					y2 = y2 < y[i][j][0] ? y[i][j][0] : y2;
					y1 = y1 > y[i][j][0] ? y[i][j][0] : y1;
				}
			}

			/**
				Draw the functions
			*/
			glScalef(1.0 / (index_right - index_left), 1.0 / (y2 - y1), 1.0);
			glTranslatef(0.0, -y1, 0.0);
			
			/**
				Init the GL state
			*/
			glLineWidth(1.0f);

			for (size_t i = 0; i < number_of_functions; i++) {

				float r, g, b;
				utils::colorSchemeRGB(r, g, b, i, number_of_functions);

				glColor3f(r, g, b);

				glBegin(GL_LINE_STRIP);

				for (size_t j = index_left; j < index_right; j++) {
					glVertex2f(j-index_left, y[i][j][0]);
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
			y-values
		*/
		std::vector<utils::Matrix<ElementType>> y;

		/**
			x-values
		*/
		utils::Matrix<ElementType> x;
	};

	/**
		Forward declaration of class StaticPlots
	*/
	class StaticPlots;

	/**
		Forward declaration of class StaticPlots
	*/	
	template<typename ElementType> class PlotsVector;

	/**
		Forward declaration of class StaticPlots
	*/
	template<typename ElementType> class PlotsMatrix;

	template<typename ElementType> class GLPlotVector
	{

	public:

		/**
			Constructor
		*/	
		GLPlotVector()
		{
			int argc; char** argv;
			glutInit(&argc, argv);
			
			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

			clear();
		}

		/**
			Destructor
		*/
		~GLPlotVector() 
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
			plots_vector().push_back(PlotFunctionVector<ElementType>());
			plots_vector.setCurrentPlot(plots_vector.getNumberOfPlots());
			
			plots_vector.setNumberOfPlots(plots_vector.getNumberOfPlots() + 1 );
		}

		/**
			Set y-values

			@param[in] y_ y-values	
		*/
		void setY(const std::vector<ElementType>& y_)
		{
			plots_vector()[plots_vector.getCurrentPlot()].setY(y_);
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(const std::vector<ElementType>& x_)
		{
			plots_vector()[plots_vector.getCurrentPlot()].setX(x_);
		}

		/** 
			Redrawing function 
		*/
		static void redraw(void)
		{
			glutSetWindow(plots_vector.getCurrentPlot() + 1);

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity(); 

			plots_vector()[plots_vector.getCurrentPlot()].draw();
			
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
			glOrtho(0, 1, 0, 1, -1, 1);
			glMatrixMode(GL_MODELVIEW);
		}
				
		/**
			Creates the window and sets the callback functions
			
			@param[in] window_name_ Name of the window
		*/
		void plot(char* window_name_ = nullptr)
		{
			if (!window_name_) {
				window_name_ = new char[10];
				sprintf(window_name_, "Window %d", plots_vector.getNumberOfPlots() - 1);
			}
			plots_vector.setCurrentPlot(glutCreateWindow(window_name_) - 1);

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
				plots_vector.setCurrentPlot(glutGetWindow() - 1);
				plots_vector()[plots_vector.getCurrentPlot()].zoomIn(x_, y_, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			}
			else {
				plots_vector.setCurrentPlot(glutGetWindow() - 1);
				plots_vector()[plots_vector.getCurrentPlot()].zoomOut();
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
				plots_vector.setCurrentPlot(glutGetWindow() - 1);
				std::cout << plots_vector()[plots_vector.getCurrentPlot()].getX(x_, glutGet(GLUT_WINDOW_WIDTH)) << " "
					<< plots_vector()[plots_vector.getCurrentPlot()].getY(y_, glutGet(GLUT_WINDOW_HEIGHT)) << std::endl;
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
		static PlotsVector<ElementType> plots_vector;
	};

	template<typename ElementType> class GLPlotMatrix
	{

	public:

		/**
			Constructor
		*/	
		GLPlotMatrix()
		{
			int argc; char** argv;
			glutInit(&argc, argv);

			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
			
			clear();
		}

		/**
			Destructor
		*/
		~GLPlotMatrix() 
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
			plots_matrix().push_back(PlotFunctionMatrix<ElementType>());
			plots_matrix.setCurrentPlot(plots_matrix.getNumberOfPlots());
			
			plots_matrix.setNumberOfPlots(plots_matrix.getNumberOfPlots() + 1 );
		}

		/**
			Set y-values

			@param[in] y_ y-values	
		*/
		void setY(const utils::Matrix<ElementType>& y_)
		{
			plots_matrix()[plots_matrix.getCurrentPlot()].setY(y_);
		}

		/**
			Set x-values

			@param[in] x_ x-values
		*/
		void setX(const utils::Matrix<ElementType>& x_)
		{
			plots_matrix()[plots_matrix.getCurrentPlot()].setX(x_);
		}

		/** 
			Redrawing function 
		*/
		static void redraw(void)
		{
			glutSetWindow(plots_matrix.getCurrentPlot() + 1);

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity(); 
			
			plots_matrix()[plots_matrix.getCurrentPlot()].draw();

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
			glViewport(0, 0, width_, height_);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 1, 0, 1, -1, 1);
			glMatrixMode(GL_MODELVIEW);
		}
				
		/**
			Creates the window and sets the callback functions
			
			@param[in] window_name_ Name of the window
		*/
		void plot(char* window_name_ = nullptr)
		{
			if (!window_name_) {
				window_name_ = new char[10];
				sprintf(window_name_, "Window %d", plots_matrix.getNumberOfPlots() - 1);
			}
			plots_matrix.setCurrentPlot(glutCreateWindow(window_name_) - 1);

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
				plots_matrix.setCurrentPlot(glutGetWindow() - 1);
				plots_matrix()[plots_matrix.getCurrentPlot()].zoomIn(x_, y_, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			}
			else {
				plots_matrix.setCurrentPlot(glutGetWindow() - 1);
				plots_matrix()[plots_matrix.getCurrentPlot()].zoomOut();
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
				plots_matrix.setCurrentPlot(glutGetWindow() - 1);
				std::cout << plots_matrix()[plots_matrix.getCurrentPlot()].getX(x_, glutGet(GLUT_WINDOW_WIDTH)) << " "
					<< plots_matrix()[plots_matrix.getCurrentPlot()].getY(y_, glutGet(GLUT_WINDOW_HEIGHT)) << std::endl;
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
		static PlotsMatrix<ElementType> plots_matrix;
	};

	/**
		Plots
	*/
	class StaticPlots
	{
	public:
		
		/**
			Constructor
		*/
		StaticPlots(void) : number_of_plots(0), current_plot(NULL) {}

		/**
			Destructor
		*/
		~StaticPlots() {}

		/**
			Clear
		*/
		virtual void clear() = 0;

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
			Get current plots
		*/
		size_t getCurrentPlot()
		{
			return current_plot;
		}

		/**
			Set current plot
		*/
		void setCurrentPlot(size_t current_plot_)
		{
			current_plot = current_plot_;
		}

	protected:
		/**
			Number of plots
		*/
		size_t  number_of_plots;
		
		/**
			Current plot
		*/
		size_t current_plot;
	};

	template<typename ElementType> class PlotsVector : public StaticPlots
	{
	public:

		/**
			Constructor
		*/
		PlotsVector()
		{
			clear();
		}

		/**
			Destructor
		*/
		~PlotsVector() 
		{
			clear();
		}

		/**
			Clear
		*/
		void clear () {
			plot.clear();
			number_of_plots = 0;
			current_plot = NULL;
		}

		/**
			Operator()

			@return plot Structure with plots
		*/
		std::vector<PlotFunctionVector<ElementType>>& operator()()
		{
			return plot;
		}

		/**
			Structure with all plots
		*/
		std::vector<PlotFunctionVector<ElementType>> plot;
	};

	template<typename ElementType> class PlotsMatrix : public StaticPlots
	{
	public:

		/**
			Constructor
		*/
		PlotsMatrix()
		{
			clear();
		}

		/**
			Destructor
		*/
		~PlotsMatrix() 
		{
			clear();
		}

		/**
			Clear
		*/
		void clear () {
			plot.clear();
			number_of_plots = 0;
			current_plot = NULL;
		}

		/**
			Operator()

			@return plot Structure with plots
		*/
		std::vector<PlotFunctionMatrix<ElementType>>& operator()()
		{
			return plot;
		}

		/**
			Structure with all plots
		*/
		std::vector<PlotFunctionMatrix<ElementType>> plot;
	};

	/**
		Static variable  GLPlot<ElementType>::plots_vector
	*/
	template<typename ElementType> PlotsVector<ElementType> GLPlotVector<ElementType>::plots_vector;

	/**
		Static variable  GLPlot<ElementType>::plots_matrix
	*/
	template<typename ElementType> PlotsMatrix<ElementType> GLPlotMatrix<ElementType>::plots_matrix;

}

#endif /* UTILS_GLPLOT_H_*/	