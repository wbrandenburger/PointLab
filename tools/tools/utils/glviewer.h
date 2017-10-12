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
			min(nullptr), max(nullptr), dim(NULL)
		{
			min = new ElementType[dim_];
			max = new ElementType[dim_];

			std::memcpy(min, dataset_, sizeof(ElementType)*dim_);
			std::memcpy(max, dataset_, sizeof(ElementType)*dim_);

			for (size_t i = 0; i < number_of_elements * dim_; i++ )
			{
				setCompareValue(dataset_[i], i % dim_);
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
			std::cout << "HIER " << std::endl;

			clear();

			dim = bounding_box_.getDim();

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
		out_ << "HUHU" << std::endl;

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

			std::cout << "HALLO" << std::endl;

			std::cout << bounding_box << std::endl;
		}

		/**
			Draw the points
		*/
		void draw() 
		{
			glPushMatrix();

				glEnableClientState(GL_VERTEX_ARRAY);
			
				glPointSize(1.0);
				
				//glScalef(1.0f / bounding_box.getDifference(0), 1.0f / bounding_box.getDifference(1), 1.0f / bounding_box.getDifference(2));
				//glTranslatef(bounding_box.getMiddle(0), bounding_box.getMiddle(1), bounding_box.getMiddle(2));


				//glPointSize(1.0);
				//glBegin(GL_POINTS);
				//for (int i = 0; number_of_elem; i++)
				//{
				//		glColor3f(texture[i][j][0] / 255, texture[i][j][1] / 255, texture[i][j][2] / 255);
				//		x = imgdata[i][j][0];
				//		y = imgdata[i][j][1];
				//		z = imgdata[i][j][2];
				//		glVertex3f(x, y, z);
				//	}
				//}
				//glEnd();

				glVertexPointer(3, GL_FLOAT, 0, points);
				glColorPointer(3, GL_UNSIGNED_BYTE, 0, color);
				
				glDrawArrays(GL_POINTS, 0, number_of_elements);

			glPopMatrix();
		}

	public:
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
			int argc = 0;  char** argv;
			glutInit(&argc, argv);
			
			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

			clear();
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

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity(); 

			viewer_instances.getCurrentViewerInstance().draw();
			
			glutSwapBuffers();
			
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
		Static variable  GLPlot<ElementType>::plots_vector
	*/
	template<typename ElementType> StaticViewerInstance<ElementType> GLViewer<ElementType>::viewer_instances;

}

#endif /* UTILS_GLVIEWER_H_*/	