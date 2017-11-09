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

#ifndef GL_PLOT3D_H_
#define GL_PLOT3D_H_

#include <map>
#include <vector>

#include <stdint.h>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include "tools/parameters.h"

#include "tools/math/angle.h"

#include "tools/utils/boundingbox.h"
#include "tools/pointcloud/quaterion.h"
#include "tools/utils/mouseposition.h"
#include "tools/utils/windowspec.h"

#include "tools/gl/glcontainer.h"
#include "tools/gl/glmousemovement.h"

#include "tools/pointcloud/pointcloudnodes.h"
#include "tools/pointcloud/pointcloud.h"

namespace gl
{
	template<typename ElementType> class Plot3DInstance
	{
	public: 
		/**
			Constructor
		*/
		Plot3DInstance() : number_of_clouds(0) 
		{
			gl_pointsize = 1;
		}

		/**
			Destructor
		*/
		~Plot3DInstance() 
		{
		}

		/**
			Clear Memory
		*/
		void clearMemory()
		{
		}

		/**
			Set pointcloud

			@param[in] gl_container Container with elements which has to be drawn
		*/
		void setPointcloud(gl::GLContainer<ElementType>& gl_container_)
		{
			gl_container.push_back(gl_container_);
			
			number_of_clouds++;
		}

		/**
			Set initial parameters
		*/
		void setParameters()
		{
			/**
				Compute the bounding box of the pointcloud
			*/
			utils::BoundingBox<ElementType> bounding_box;
			for (size_t i = 0; i < number_of_clouds; i++) {
				bounding_box.setBoundingBox(gl_container[i].getPoints(), gl_container[i].getNumberOfVertices(), 3);
			}

			/**
				Center and normalize the pointlcoud
			*/
			ElementType gl_zoom = bounding_box.getDifference(0) > bounding_box.getDifference(1) ? bounding_box.getDifference(0) : bounding_box.getDifference(1);
			for (size_t i = 0; i < number_of_clouds; i++) {
				ElementType* points = gl_container[i].getPoints();
				for (size_t j = 0; j <  gl_container[i].getNumberOfVertices() * 3; j++) {
					points[j] = (points[j] - bounding_box.getMiddle(j % 3)) * (1 / gl_zoom);
				}
			}
		}

		/**
			Increase point size
		*/
		void increasePointSize()
		{
			if (gl_pointsize < 10) {
				gl_pointsize++;
			}
		}

		/**
			Decrease point size
		*/
		void decreasePointSize()
		{
			if (gl_pointsize > 1) {
				gl_pointsize--;
			}
		}

		/**
			Zoom

			@param[in] direction_ Direction in which the wheel is turned: 1 for up and -1 for down
		*/
		void zoom(int direction) {
			gl_mouse_movement_.setZoom(direction);
		}

		/**
			Translate

			@param[in] x Mouse position in x-direction
			@param[in] y Mouse position in y-direction
		*/
		void translate(int x, int y)
		{
			gl_mouse_movement_.setTranslation(x, y);
		}

		/**
			Rotate

			@param[in] x_old Old mouse position in x-direction
			@param[in] y_old Old mouse position in y-direction
			@param[in] x_new New mouse position in x-direction
			@param[in] y_new New mouse position in y-direction
		*/
		void rotate(int x_old, int y_old, int x_new, int y_new)
		{
			gl_mouse_movement_.SetRotation(x_old, y_old, x_new, y_new);
		}

		/**
			Draw the pointcloud with float accurancy
		*/
		void draw() 
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			for (size_t i = 0; i < number_of_clouds; i++) {
				/**
					Enables use of glVertexPointer and glColorPointer when drawing with glDrawArrays/
				*/
				glEnableClientState(GL_VERTEX_ARRAY);
				if (gl_container[i].isColor()) {
					glEnableClientState(GL_COLOR_ARRAY);
				}
				if (gl_container[i].isNormal()) {
					glEnableClientState(GL_NORMAL_ARRAY);
				}
				if (gl_container[i].getMode() != GL_POINTS) {
					glEnableClientState(GL_INDEX_ARRAY);
				}

				float r, g, b;
				utils::colorSchemeRGB(r, g, b, i, number_of_clouds);
				glColor4f(r, g, b, 0.0f);


				glLoadIdentity();

				/**
					Increase or decrease the entire pointcloud by the factor gl_zoom
				*/
				glScalef(1.0f / gl_mouse_movement_.getZoom(), 
					1.0f / gl_mouse_movement_.getZoom(), 
					1.0f / gl_mouse_movement_.getZoom());

				/**
					Translate the pointcloud by the chosen centerpoint
				*/
				glTranslatef(gl_mouse_movement_.getCenterX() * gl_mouse_movement_.getZoom(),
					gl_mouse_movement_.getCenterY() * gl_mouse_movement_.getZoom(),
					gl_mouse_movement_.getCenterZ() * gl_mouse_movement_.getZoom());

				/**
					Rotate the entire pointcloud around the x- and y-axis
				*/
				ElementType gl_rot_x, gl_rot_y, gl_rot_z;
				gl_mouse_movement_.getRotation().getEulerAngles(gl_rot_x, gl_rot_y, gl_rot_z);
				glRotatef(-1.0f *math::toDeg<ElementType>(gl_rot_x), 1.0f, 0.0, 0.0);
				glRotatef(1.0f *math::toDeg<ElementType>(gl_rot_y), 0.0, 1.0f, 0.0);
				glRotatef(1.0f *math::toDeg<ElementType>(gl_rot_z), 0.0, 0.0, 1.0f);

				/**
					Determine the size of the points
				*/
				glPointSize((GLfloat)gl_pointsize);

				/**
					Link the points, colors and normals for drawing
				*/
				glVertexPointer(3, GL_FLOAT, 0, gl_container[i].getPoints());
				if (gl_container[i].isColor()) {
					glColorPointer(3, GL_UNSIGNED_BYTE, 0, gl_container[i].getColor());
				}
				if (gl_container[i].isNormal()) {
					glNormalPointer(GL_FLOAT, 0, gl_container[i].getNormals());
				}
				
				if (gl_container[i].getMode() == GL_POINTS) {
					glDrawArrays(gl_container[i].getMode(), 0, gl_container[i].getNumberOfVertices());
				}
				else{
					glDrawElements(gl_container[i].getMode(), gl_container[i].getNumberOfIndices(),
						GL_UNSIGNED_INT, gl_container[i].getIndices());
				}

				/**
					Disables use of glVertexPointer and glColorPointer when drawing with glDrawArrays/
				*/
				glDisableClientState(GL_VERTEX_ARRAY);
				if (gl_container[i].isColor()) {
					glDisableClientState(GL_COLOR_ARRAY);
				}
				if (gl_container[i].isNormal()) {
					glDisableClientState(GL_NORMAL_ARRAY);
				}
				if (gl_container[i].getMode() != GL_POINTS) {
					glDisableClientState(GL_INDEX_ARRAY);
				}
			}
				
			/**
				Draw axis
			*/

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
				glLineWidth(2.0f);
				glBegin(GL_LINES);
					glColor4f(1.0f, 0.0, 0.0, 0.0);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(0.25f*gl_mouse_movement_.getZoom(), 0.0, 0.0);
				glEnd();
				glBegin(GL_LINES);
					glColor4f(0.0, 1.0f, 0.0, 0.0);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(0.0, 0.25f*gl_mouse_movement_.getZoom(), 0.0);
				glEnd();
				glBegin(GL_LINES);
					glColor4f(0.0, 0.0, 1.0f, 0.0);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(0.0, 0.0, 0.25f*gl_mouse_movement_.getZoom());
				glEnd();
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);

			glPopMatrix();

			glutSwapBuffers();
		}

	private:

		/**
			Container with points, colors, normals and indices
		*/
		std::vector<gl::GLContainer<ElementType>> gl_container;

		/**
			Computes translation, rotation and zoom
		*/
		gl::GLMouseMovement<ElementType> gl_mouse_movement_;

		/**
			Point size
		*/
		size_t gl_pointsize;

		/**
			Number of pointclouds
		*/
		size_t number_of_clouds;
	};

	/**
		Forward declaration of class StaticPlot3DInstance
	*/
	template<typename ElementType> class StaticPlot3DInstance;

	template<typename ElementType> class GLPlot3D
	{

	public:

		/**
			Constructor
		*/	
		GLPlot3D()
		{
			clearMemory();

			if (!std::is_same<float,ElementType>::value && !std::is_same<double, ElementType>::value){
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			mouse_button = NULL;
		}

		/**
			Destructor
		*/
		~GLPlot3D() 
		{
			clearMemory();
		}

		/**
			Clear Memory
		*/
		void clearMemory() 
		{
			plot3d_instances.clearMemory();
			
			mouse_button = NULL;
		}

		/**
			Set  a new plot
		*/
		void setPlot3D()
		{
			plot3d_instances.setPlot3DInstance();
		}

		/**
			Set pointlcoud
			
			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		void setPointcloud(GLParams mode_, const pointcloud::Pointcloud<ElementType>& pointcloud_)
		{
			gl::GLContainer<ElementType> gl_container(mode_, pointcloud_);
			plot3d_instances.getCurrentPlot3DInstance().setPointcloud(gl_container);
		}

		/**
			Set pointcloud

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of elements
		*/
		void setPointcloud(GLParams mode_, ElementType* points_, size_t number_of_vertices_)
		{
			gl::GLContainer<ElementType> gl_container(mode_, points_, number_of_vertices_);
			plot3d_instances.getCurrentPlot3DInstance().setPointcloud(gl_container);
		}
				
		/**
			Set pointcloud

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] lines_ Lines
			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_lines_ Number of indices
		*/
		void setPointcloud(GLParams mode_, ElementType* points_, unsigned int* lines_, 
			size_t number_of_vertices_, size_t number_of_lines_)
		{
			gl::GLContainer<ElementType> gl_container(mode_, points_, lines_, number_of_vertices_, number_of_lines_);
			plot3d_instances.getCurrentPlot3DInstance().setPointcloud(gl_container);
		}

		/** 
			Redrawing function 
		*/
		static void redraw(void)
		{
			glutSetWindow((int)plot3d_instances.getCurrentWindow() + 1);

			plot3d_instances.getCurrentPlot3DInstance().draw();
		};

		/**  
			Redisplays, if called 
		*/
		static void idle(void)
		{
			glutPostRedisplay();
		}

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
				sprintf(window_name_, "Plot3D %d", (int) plot3d_instances.getNumberOfPlot3D() - 1);
			}


			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

			glutInitWindowSize((int)window_spec.getWidth(), (int)window_spec.getHeight());
			glutInitWindowPosition((int)window_spec.getPositionX(), (int)window_spec.getPositionY());

			size_t window_index = glutCreateWindow(window_name_) - 1;
			plot3d_instances.setWindowIndex(window_index);
			plot3d_instances.setCurrentInstance(window_index);

			plot3d_instances.getCurrentPlot3DInstance().setParameters();
			/**
				Register GLUT callbacks.
			*/
			glutDisplayFunc(redraw);
			glutKeyboardFunc(key);
			glutReshapeFunc(reshape);
			glutIdleFunc(idle);
			glutMouseWheelFunc(mouseWheel);
			glutMouseFunc(mouseFunc);
			glutMotionFunc(mouseMotion);

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
		static void key(uint8_t key_, int x_, int y_)
		{
			switch (key_) {		
			case '+': plot3d_instances.getCurrentPlot3DInstance().increasePointSize(); break;
			case '-': plot3d_instances.getCurrentPlot3DInstance().decreasePointSize(); break;
			case 27	: glutLeaveMainLoop(); break;
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
			plot3d_instances.setCurrentInstance(glutGetWindow() - 1);
			plot3d_instances.getCurrentPlot3DInstance().zoom(direction_);
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
			plot3d_instances.setCurrentInstance(glutGetWindow() - 1);
			if (!state_){
				mouse_button = button_;

				mouse_position.setPosition(x_, y_);
			}
			else {
				mouse_button = NULL;
			}

		}

		/**
			Callback for mouse movement

			@param[in] x_ Mouse position in x-direction
			@param[in] y_ Mouse position in y-direction
		*/
		static void mouseMotion(int x_, int y_)
		{
			plot3d_instances.setCurrentInstance(glutGetWindow() - 1);
			if (mouse_button == GLUT_LEFT_BUTTON){
				plot3d_instances.getCurrentPlot3DInstance().rotate(mouse_position.getX(), 
					mouse_position.getY(), x_, y_);
				
				mouse_position.setPosition(x_, y_);
			}
			else if (mouse_button == GLUT_MIDDLE_BUTTON) {
				mouse_position.setPosition(x_, y_);
			}
			else if (mouse_button == GLUT_RIGHT_BUTTON) {
				int x_difference;
				int y_difference;

				mouse_position.setPosition(x_, y_, x_difference, y_difference);
				plot3d_instances.getCurrentPlot3DInstance().translate(x_difference, y_difference);
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
		static StaticPlot3DInstance<ElementType> plot3d_instances;

		/**
			Mouse button
		*/
		static int mouse_button;

		/**
			Mouse position
		*/
		static utils::MousePosition mouse_position;
	};

	template<typename ElementType> class StaticPlot3DInstance
	{
	public:
		/** 
			Constructor
		*/
		StaticPlot3DInstance() : number_of_plot3d(0), current_instance(NULL), current_window(NULL)
		{
		}

		/**
			Destructor
		*/
		~StaticPlot3DInstance()
		{
			clearMemory();
		}

		/**
			Clear Memory
		*/
		void clearMemory()
		{
			for (size_t i = 0; i < number_of_plot3d; i++) {
				plot3d_instances[i].clearMemory();
			}
			plot3d_instances.clear();
			number_of_plot3d = 0;
			current_instance = NULL;
			current_window = NULL;
		}

		/** 
			Set plot3d instance
		*/
		void setPlot3DInstance()
		{
			plot3d_instances.push_back(Plot3DInstance<ElementType>());
			
			current_instance = number_of_plot3d;
			number_of_plot3d++;
		}

		/**
			Get number of plot3d
		*/
		size_t getNumberOfPlot3D()
		{
			return number_of_plot3d;
		}

		/**
			Set number of plot3d
		*/
		void setNumberOfPlot3D(size_t number_of_plot3d_)
		{
			number_of_plot3d = number_of_plot3d_;
		}

		/**
			Get current instance
		*/
		size_t getCurrentInstance()
		{
			return current_instance;
		}

		/**
			Set current instance
		*/
		void setCurrentInstance(size_t current_window_)
		{
			current_instance = plot3d_indices[current_window_];
			current_window = current_window_;
		}

		/**
			Get current window
		*/
		size_t getCurrentWindow(){
			return current_window;
		}

		/**
			Assign a window index with an plot index
		*/
		void setWindowIndex(size_t window_index_) 
		{
			plot3d_indices[window_index_] = number_of_plot3d-1;
		}

		/** 
			Get current plot3d instance
		*/
		Plot3DInstance<ElementType>& getCurrentPlot3DInstance()
		{
			return plot3d_instances[current_instance];
		}

		/**
			Operator()

			@return plot Structure with plots
		*/
		std::vector<Plot3DInstance<ElementType>>& operator()()
		{
			return plot3d_instances;
		}

	private:

		/**
			Number of plot3d
		*/
		size_t  number_of_plot3d;
		
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
		std::map<size_t, size_t> plot3d_indices;

		/**
			Structure with all plot3d
		*/
		std::vector<Plot3DInstance<ElementType>> plot3d_instances;
	};

	/**
		Static variable GLPlot3D<ElementType>::plot3d_instances
	*/
	template<typename ElementType> StaticPlot3DInstance<ElementType> GLPlot3D<ElementType>::plot3d_instances;

	/**
		Static variable GLPlot3D<ElementType>::mouse_button
	*/
	template<typename ElementType> int GLPlot3D<ElementType>::mouse_button;

	/**
		Static variable GLPlot3D<ElementType>::mouse_button
	*/
	template<typename ElementType> utils::MousePosition GLPlot3D<ElementType>::mouse_position;

}

#endif /* GL_PLOT3D_H_*/	