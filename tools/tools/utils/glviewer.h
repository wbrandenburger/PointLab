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

#ifndef UTILS_GLVIEWER_H_
#define UTILS_GLVIEWER_H_

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include "tools/pointcloud/pointcloud.h"

namespace utils
{
	template<typename ElementType> struct BoundingBox 
	{
	public:

		/**
			Constructor

			@param[in] dataset_ Dataset
			@param[in] number_of_elements_ Number of elements
			@param[in] dim_ Specific dimension
		*/
		BoundingBox() : min(nullptr), max(nullptr), dim(NULL) {}

		/**
			Constructor

			@param[in] dataset_ Dataset
			@param[in] number_of_elements_ Number of elements
			@param[in] dim_ Specific dimension
		*/
		BoundingBox(ElementType* dataset_, size_t number_of_elements, size_t dim_) :
			min(nullptr), max(nullptr), dim(dim_)
		{
			min = new ElementType[dim];
			max = new ElementType[dim];

			std::memcpy(min, dataset_, sizeof(ElementType)*dim);
			std::memcpy(max, dataset_, sizeof(ElementType)*dim);

			for (size_t i = 0; i < number_of_elements * dim; i++ )
			{
				setCompareValue(dataset_[i], i % dim);
			}
		}

		/**
			Destructor
		*/
		~BoundingBox()
		{
			clear();
		}

		/**
			Copy Constructor

			@param[in] bounding_box_ Bounding Box
		*/
		BoundingBox(const BoundingBox<ElementType>& bounding_box_) : min(nullptr), max(nullptr), dim(NULL)
		{
			dim = bounding_box_.getDim();

			min = new ElementType[dim];
			max = new ElementType[dim];

			std::memcpy(min, bounding_box_.getMin(), sizeof(ElementType)*dim);
			std::memcpy(max, bounding_box_.getMax(), sizeof(ElementType)*dim);
		}

		/**
			Operator =

			@param[in] bounding_box_ Bounding Box
		*/
		BoundingBox& operator=(const BoundingBox<ElementType>& bounding_box_)
		{
			clear();

			dim = bounding_box_.getDim();

			std::cout << dim << std::endl;

			min = new ElementType[dim];
			max = new ElementType[dim];

			std::memcpy(min, bounding_box_.getMin(), sizeof(ElementType)*dim);
			std::memcpy(max, bounding_box_.getMax(), sizeof(ElementType)*dim);

			return (*this);
		}

		/**
			Clear
		*/
		void clear()
		{
			if (min) {
				delete[] min;
				min = nullptr;
			}

			if (max) {
				delete[] max;
				max = nullptr;
			}
			dim = NULL;
		}

		/**
			Get minimum value for specific dimension

			@param[in] dim_ Specific dimension
		*/
		ElementType getMinDim(size_t dim_) const
		{
			return min[dim_];
		}

		/**
			Get maximum value for specific dimension

			@param[in] dim_ Specific dimension
		*/
		ElementType getMaxDim(size_t dim_) const
		{
			return max[dim_];
		}

		/**
			Get dimension
		*/
		size_t getDim() const
		{
			return dim;
		}

		/**
			Get minimum values
		*/
		ElementType* getMin() const
		{
			return min;
		}

		/**
			Get maximum values
		*/
		ElementType* getMax() const
		{
			return max;
		}	

		/**
			Get difference for a specific dimension
		*/
		ElementType getDifference(size_t dim_) const
		{
			return max[dim_] - min[dim_];
		}

		/**
			Get difference for a specific dimension
		*/
		ElementType getMiddle(size_t dim_) const
		{
			return (max[dim_] + min[dim_]) / 2.0;
		}

		/**
			Set value for specific dimension

			@param[in] value_ Value
			@param[in] dim_ Specific dimension
		*/
		void setValue(ElementType value_, size_t dim_)
		{
			min[dim_] = value_;
			max[dim_] = value_;
		}

		/**
			Compare and set value for specific dimension

			@param[in] value_ Value
			@param[in] dim_ Specific dimension
		*/
		void setCompareValue(ElementType value_, size_t dim_)
		{
			if (value_ < min[dim_]) {
				min[dim_] = value_;
			}

			if (value_ > max[dim_]) {
				max[dim_] = value_;
			}
		}

	private:

		/**
			Minimum values
		*/
		ElementType* min;

		/**
			Maximum values
		*/
		ElementType* max;

		/**
			Dimension
		*/
		size_t dim;
	};

	/**
		Operator << Prints the values of the bounding box

		@param[in,out] out_ Outstream in which the bounding box will be printed
		@param[in] bounding_box Bounding box which shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const BoundingBox<ElementType>& bounding_box_)
	{
		for (size_t i = 0; i <  bounding_box_.getDim(); i++) {
			out_ << bounding_box_.getMinDim(i) << " " 
				<< bounding_box_.getMaxDim(i) << " " 
				<< bounding_box_.getDifference(i) << " " 
				<< bounding_box_.getMiddle(i) << std::endl;
		}
		return out_;
	}

	template<typename ElementType> class ViewerInstance
	{
	public: 
		/**
			Constructor
		*/
		ViewerInstance() : points(nullptr), color(nullptr), number_of_elements(NULL) {}

		/**
			Destructor
		*/
		~ViewerInstance() {}

		/**
			Clear
		*/
		void clear()
		{
			if (points) {
				delete[] points;
				points = nullptr;
			}

			if (color) {
				delete[] color;
				color = nullptr;
			}

			number_of_elements;
		}

		/**
			Set pointcloud

			@param[in] pointcloud_ Pointcloud
		*/
		void setPointcloud(const pointcloud::Pointcloud<ElementType>& pointcloud_)
		{
			points = pointcloud_.getPointsPtr();
			color = pointcloud_.getColorsPtr();
		
			number_of_elements = pointcloud_.getRows();

			bounding_box = utils::BoundingBox<ElementType>(points, number_of_elements, 3);
			
			std::cout << bounding_box << std::endl;

			gl_pointsize = 1;

			gl_center_x = bounding_box.getMiddle(0);
			gl_center_y = bounding_box.getMiddle(1);
			gl_center_z = bounding_box.getMiddle(2);

			gl_zoom = bounding_box.getDifference(0) > bounding_box.getDifference(1) ? bounding_box.getDifference(0) : bounding_box.getDifference(1);
			
			gl_translate_x = 0.5;
			gl_translate_y = 0.5;
			gl_translate_z = 0.5;

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
			Zoom in
		*/
		void zoomIn() {
			gl_zoom = gl_zoom * 1.1;
		}

		/**
			Zoom in
		*/
		void zoomOut() {
			gl_zoom = gl_zoom / 1.1;
		}

		/**
			Draw the pointcloud
		*/
		void draw()
		{
			if (std::is_same<float, ElementType>::value) {
				drawf();
			}
			else {
				drawd();
			}
		}

		/**
			Draw the pointcloud with float accurancy
		*/
		void drawf() 
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				
				glLoadIdentity();
				glTranslatef(-gl_center_x / gl_zoom, -gl_center_y / gl_zoom, -gl_center_z / gl_zoom);
				glScalef(1.0f / gl_zoom, 1.0f / gl_zoom, 1.0f / gl_zoom);
				glTranslatef(gl_translate_x*gl_zoom, gl_translate_y*gl_zoom, gl_translate_z*gl_zoom);

				glPointSize(gl_pointsize);

				glVertexPointer(3, GL_FLOAT, 0, points);
				glColorPointer(3, GL_UNSIGNED_BYTE, 0, color);
				glDrawArrays(GL_POINTS, 0, number_of_elements);


				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

			glPopMatrix();

			glutSwapBuffers();
		}

		/**
			Draw the pointcloud with double accurancy
		*/
		void drawd() 
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				
				glPointSize(1.0);
				glLoadIdentity();

				//glScaled(1.0 / gl_zoom, 1.0 / gl_zoom, 1.0 / gl_zoom);

				//glTranslated(gl_translate_x, gl_translate_y, gl_translate_z);
				
				glVertexPointer(3, GL_DOUBLE, 0, points);
				glColorPointer(3, GL_UNSIGNED_BYTE, 0, color);
				
				glDrawArrays(GL_POINTS, 0, number_of_elements);


				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

			glPopMatrix();

			glutSwapBuffers();

		}
	
	private:
		/**
			Points
		*/
		ElementType* points;
		
		/**
			Color
		*/
		unsigned char* color;

		/**
			Number of elements
		*/
		size_t number_of_elements;

		/**
			Bounding box
		*/
		utils::BoundingBox<ElementType> bounding_box;

		/**
			Center in x-direction
		*/
		ElementType gl_center_x;

		/**
			Center in y-direction
		*/
		ElementType gl_center_y;

		/**
			Center in x-direction
		*/
		ElementType gl_center_z;
		
		/**
			Translation in x-direction
		*/
		ElementType gl_translate_x;

		/**
			Translation in y-direction
		*/
		ElementType gl_translate_y;

		/**
			Translation in x-direction
		*/
		ElementType gl_translate_z;
		
		/**
			Point size
		*/
		size_t gl_pointsize;

		/**
			Zoom factor
		*/
		ElementType gl_zoom;

	};

	/**
		Forward declaration of class StaticViewerInstance
	*/
	template<typename ElementType> class StaticViewerInstance;

	template<typename ElementType> class GLViewer
	{

	public:

		/**
			Constructor
		*/	
		GLViewer()
		{
			clear();

			int argc = 0;  char** argv;
			glutInit(&argc, argv);
			
			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

			if (!std::is_same<float,ElementType>::value && !std::is_same<double, ElementType>::value){
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			mouse_button = NULL;
		}

		/**
			Destructor
		*/
		~GLViewer() 
		{
			clear();
		}

		/**
			Clear
		*/
		void clear() 
		{
			viewer_instances.clear();
			
			mouse_button = NULL;
		}

		/**
			Set  a new plot
		*/
		void setViewer()
		{
			viewer_instances.setViewerInstance();
		}

		/**
			Set pointlcoud

			@param[in] pointcloud_ Pointcloud
		*/
		void setPointcloud(const pointcloud::Pointcloud<ElementType>& pointcloud_)
		{
			viewer_instances.getCurrentViewerInstance().setPointcloud(pointcloud_);
		}

		/** 
			Redrawing function 
		*/
		static void redraw(void)
		{


			glutSetWindow(viewer_instances.getCurrentInstance() + 1);

			viewer_instances.getCurrentViewerInstance().draw();
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
				sprintf(window_name_, "Window %d", viewer_instances.getNumberOfViewer() - 1);
			}

			viewer_instances.setCurrentInstance(glutCreateWindow(window_name_) - 1);

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
		static void key(unsigned char key_, int x_, int y_)
		{
			switch (key_) {		
			case '+': viewer_instances.getCurrentViewerInstance().increasePointSize(); break;
			case '-': viewer_instances.getCurrentViewerInstance().decreasePointSize(); break;
			case 27: exit(0); break;
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
				viewer_instances.getCurrentViewerInstance().zoomOut();
			}
			else {
				viewer_instances.getCurrentViewerInstance().zoomIn();
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
			if (!state_){
				mouse_button = button_;
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
			if (mouse_button == GLUT_LEFT_BUTTON){
				std::cout << "Left" << std::endl;
			}
			else if (mouse_button == GLUT_MIDDLE_BUTTON) {
				std::cout << "Middle" << std::endl;
			}
			else if (mouse_button == GLUT_RIGHT_BUTTON) {
				std::cout << "Right" << std::endl;
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
		static StaticViewerInstance<ElementType> viewer_instances;

		/**
			Mouse button
		*/
		static int mouse_button;
	};

	template<typename ElementType> class StaticViewerInstance
	{
	public:
		/** 
			Constructor
		*/
		StaticViewerInstance() : number_of_viewer(0), current_instance(NULL)
		{
			clear();
		}

		/**
			Destructor
		*/
		~StaticViewerInstance()
		{
			clear();
		}

		/**
			Clear
		*/
		void clear()
		{
			for (size_t i = 0; i < number_of_viewer; i++) {
				viewer_instances[i].clear();
			}
			viewer_instances.clear();
			number_of_viewer = 0;
			current_instance = NULL;
		}

		/** 
			Set viewer instance
		*/
		void setViewerInstance()
		{
			viewer_instances.push_back(ViewerInstance<ElementType>());
			
			current_instance = number_of_viewer;
			number_of_viewer++;
		}

		/**
			Get number of viewer
		*/
		size_t getNumberOfViewer()
		{
			return number_of_viewer;
		}

		/**
			Set number of viewer
		*/
		void setNumberOfViewer(size_t number_of_viewer_)
		{
			number_of_viewer = number_of_viewer_;
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
		void setCurrentInstance(size_t current_instance_)
		{
			current_instance = current_instance_;
		}

		/** 
			Get current viewer instance
		*/
		ViewerInstance<ElementType>& getCurrentViewerInstance()
		{
			return viewer_instances[current_instance];
		}

		/**
			Operator()

			@return plot Structure with plots
		*/
		std::vector<ViewerInstance<ElementType>>& operator()()
		{
			return viewer_instances;
		}

	private:

		/**
			Number of viewer
		*/
		size_t  number_of_viewer;
		
		/**
			Current plot
		*/
		size_t current_instance;

		/**
			Structure with all viewer
		*/
		std::vector<ViewerInstance<ElementType>> viewer_instances;
	};

	/**
		Static variable GLViewer<ElementType>::viewer_instances
	*/
	template<typename ElementType> StaticViewerInstance<ElementType> GLViewer<ElementType>::viewer_instances;

	/**
		Static variable GLViewer<ElementType>::mouse_button
	*/
	template<typename ElementType> int GLViewer<ElementType>::mouse_button;
}

#endif /* UTILS_GLVIEWER_H_*/	