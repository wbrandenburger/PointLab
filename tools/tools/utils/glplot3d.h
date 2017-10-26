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

#ifndef UTILS_PLOT3D_H_
#define UTILS_PLOT3D_H_

#include <map>
#include <vector>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include "tools/math/angle.h"

#include "tools/utils/boundingbox.h"
#include "tools/pointcloud/quaterion.h"
#include "tools/utils/mouseposition.h"
#include "tools/utils/windowspec.h"

#include "tools/pointcloud/pointcloud.h"

namespace utils
{
	/** 
		Providing a class which allows to visualize pointclouds
		This pointcloud can be a instance of:
			- pointcloud::Pointcloud<ElementType>
			- ElementType* 

		A instance of a plotting object can be called with
			- utils::GLPlot3D<ElementType> plot3d

		This instance hold several plots which can be separate visualized.
		To set a plot3d call
			- plot3d.setPlot3D()
		The setPlot3D() has to be called otherwise the pointclouds
		cant be assigned.

		To set the pointcloud call
			- plot3d.setPointcloud(<container>)

		With the function plot()
			- plot3d.plot()
		the data will be visualized.

		To keep up a window with the results call mainLoop()
			- plot3d.mainLoop()
		When there a other programs called which use OpenGL you have to
		call mainLoop() once.

		Example:
		utils::GLPlot3D<ElementType> plot3d;
		plot3d.setPlot3D();
			pointcloud::Pointcloud<ElementType> pointcloud;
			*
			* Assigning values to pointcloud
			*
			plot3d.setPointlcoud(pointcloud),
		plot3d.plot();
		plot3d.mainLoop();
	*/

	template<typename ElementType> class PlotContainer 
	{
	public:
		/**
			Constructor
		*/
		PlotContainer() :  mode(GL_POINTS), 
			points(nullptr), color(nullptr), normals(nullptr), indices(nullptr),
			number_of_elements(NULL), number_indices(NULL)
		{
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		PlotContainer(GLenum mode_, pointcloud::Pointcloud<ElementType>& pointcloud_ ) : mode(mode_),
			points(nullptr), color(nullptr), normals(nullptr), indices(nullptr),
			number_of_elements(NULL), number_of_indices(NULL)
		{
			if (pointcloud_.isPoints()) {
				points = pointcloud_.getPointsPtr();
			}
			if (pointcloud_.isColor()) {
				color = pointcloud_.getColorsPtr();
			}
			if (pointcloud_.isNormal()) {
				normals = pointcloud_.getNormalsPtr();
			}

			number_of_elements = pointcloud_.getRows();

			//if (pointcloud_.isIndex()) {
			//	number_of_indices = getNumberOfIndices();
			//	indices = pointcloud.getIndicesPtr();
			//}
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_elements_ Number of elements
		*/
		PlotContainer(GLenum mode_, ElementType* points_, size_t number_of_elements_) : mode(mode_),
			points(nullptr), color(nullptr), normals(nullptr), indices(nullptr),
			number_of_elements(number_of_elements_), number_of_indices(NULL)
		{
			number_of_elements = number_of_elements_;

			points = new ElementType[number_of_elements];
			std::memcpy(points, points_, sizeof(ElementType)*number_of_elements);
		}

		/**
			Destructor
		*/
		~PlotContainer()
		{
			clear();
		}

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
			if (normals) {
				delete[] normals;
				nomrals = nullptr;
			}
			if (indices) {
				delete[] indices;
				indices = nullptr;
			}
		}

		/**
			Copy constructor
	
			@param[in] plot_container_ An instance of class Class
		*/
		PlotContainer(PlotContainer <ElementType>& plot_container_) = delete;
		
		/**
			Copy constructor
	
			@param[in] plot_container_ An instance of class Class
		*/

		PlotContainer(PlotContainer <ElementType>&& plot_container_) = delete;		
		/**
			Operator =
	
			@param[in] plot_container_ An instance of class Class
		*/
		PlotContainer& operator=(const PlotContainer<ElementType>& plot_container_) = delete;
	
		/**
			Operator =
	
			@param[in] plot_container_ An instance of class Class
		*/
		PlotContainer& operator=(const PlotContainer<ElementType>&& plot_container_) = delete;

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		void setPlotContainer(GLenum mode_, pointcloud::Pointcloud<ElementType>& pointcloud_ ) 
		{
			mode = mode_;

			if (pointcloud_.isPoints()) {
				points = pointcloud_.getPointsPtr();
			}
			if (pointcloud_.isColor()) {
				color = pointcloud_.getColorsPtr();
			}
			if (pointcloud_.isNormal()) {
				normals = pointcloud_.getNormalsPtr();
			}

			number_of_elements = pointcloud_.getRows();

			//if (pointcloud_.isIndex()) {
			//	number_of_indices = getNumberOfIndices();
			//	indices = pointcloud.getIndicesPtr();
			//}
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_elements_ Number of elements
		*/
		void setPlotContainer(GLenum mode_, ElementType* points_, size_t number_of_elements_)
		{
			mode = mode_;

			number_of_elements = number_of_elements_;

			points = new ElementType[number_of_elements];
			std::memcpy(points, points_, sizeof(ElementType)*number_of_elements);
		}
	
		/**
			Get the mode which specifies what kind of primitives to render
		*/
		GLenum getMode() const
		{
			return mode;
		}

		/**
			Get points

			@return Points
		*/
		ElementType* getPoints() const
		{
			return points;
		}

		/**
			Get colors

			@return Colors
		*/
		unsigned char* getColor() const
		{
			return color;
		}

		/**
			Get normals

			@return Normals
		*/
		ElementType* getNormals() const
		{
			return normals;
		}

		/**
			Get indices
			
			@return Indices
		*/
		unsigned int* getIndices() const
		{
			return indices;
		}

		/**
			Get number of elements
		*/
		size_t getNumberOfElements() const
		{
			return number_of_elements;
		}

		/**
			Get number of indices
		*/
		size_t getNumberOfIndices() const
		{
			return number_of_indices;
		}

		/**
			Returns true if points are set

			@return True if points are set
		*/
		bool isPoint() const
		{
			return (points != nullptr);
		}

		/**
			Returns true if colors are set

			@return True if colors are set
		*/
		bool isColor() const
		{
			return (colors != nullptr);
		}

		/**
			Returns true if normalss are set

			@return True if normals are set
		*/
		bool isNormal() const
		{
			return (normals != nullptr);
		}

		/**
			Returns true if normalss are set

			@return True if normals are set
		*/
		bool isIndex() const
		{
			return (indices != nullptr);
		}

	private:

		/**
			Specifies what kind of primitives to render
		*/
		GLenum mode;

		/**
			Points
		*/
		ElementType* points;

		/**
			Color
		*/
		unsigned char* color;

		/**
			Normals
		*/
		ElementType* normals;

		/**
			Indices
		*/
		unsigned int* indices;

		/**
			Number of elements
		*/
		size_t number_of_elements;

		/**
			Number of indices
		*/
		size_t number_of_indices;
	};

	template<typename ElementType> class Plot3DInstance
	{
	public: 
		/**
			Constructor
		*/
		Plot3DInstance() : number_of_clouds(0) {}

		/**
			Destructor
		*/
		~Plot3DInstance() {}

		/**
			Clear
		*/
		void clear()
		{
		}

		/**
			Set pointcloud

			@param[in] pointcloud_ Pointcloud
		*/
		void setPointcloud(const pointcloud::Pointcloud<ElementType>& pointcloud_)
		{
			plot_container.push_back(PlotContainer<ElementType>(GL_POINTS,pointcloud_));

			number_of_clouds++;
		}

		/**
			Set pointcloud

			@param[in] points_ Points
			@param[in] number_of_elements_ Number of elements
		*/
		void setPointcloud(ElementType* points_, size_t number_of_elements_)
		{
			plot_container.push_back(PlotContainer<ElementType>(GL_POINTS, points_, number_of_elements_));

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
			for (size_t i = 0; i < number_of_clouds; i++){
				bounding_box.setBoundingBox(plot_container[i].getPoints(), plot_container[i].getNumberOfElements(), 3);
			}

			gl_pointsize = 1;

			gl_center_x = 0.5;
			gl_center_y = 0.5;
			gl_center_z = 0.5;

			/**
				Center and normalize the pointlcoud
			*/
			gl_zoom = bounding_box.getDifference(0) > bounding_box.getDifference(1) ? bounding_box.getDifference(0) : bounding_box.getDifference(1);
			
			for (size_t i = 0; i < number_of_clouds; i++) {
				ElementType* points = plot_container[i].getPoints();
				for (size_t i = 0; i <  plot_container[i].number_of_elements * 3; i++) {
					points[i] = (points[i] - bounding_box.getMiddle(i % 3)) * (1 / gl_zoom);
				}
			}
			gl_zoom = 1.0;
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
			gl_zoom = gl_zoom * (ElementType) 1.1;
		}

		/**
			Zoom in
		*/
		void zoomOut() {
			gl_zoom = gl_zoom / (ElementType) 1.1;
		}

		/**
			Translate
		*/
		void translate(int x_, int y_)
		{
			gl_center_x = gl_center_x + (float)x_/500.0f;
			gl_center_y = gl_center_y - (float)y_/500.0f;
		}

		/**
			Rotate
		*/
		void rotate(int x_old_, int y_old_, int x_new_, int y_new_)
		{
			/**
				Compute the radius of the imaginary sphere
			*/
			ElementType max_x = gl_center_x > (ElementType)0.5 ? gl_center_x : (ElementType)1.0 - gl_center_x;
			ElementType max_y = gl_center_y > (ElementType)0.5 ? gl_center_y : (ElementType)1.0 - gl_center_y;

			ElementType radius_sqr = max_x*max_x + max_y*max_y;

			/**
				Normalize the coordinates
			*/
			int width = glutGet(GLUT_WINDOW_WIDTH);
			int height = glutGet(GLUT_WINDOW_HEIGHT);

			if (x_new_ > 0 && x_new_ < width && y_new_ > 0 && y_new_ < height && 
				!(x_new_ - x_old_ == 0 && y_new_ - y_old_ == 0) ) {

				ElementType x_old = (ElementType)x_old_ / (ElementType)width - gl_center_x;
				ElementType y_old = (ElementType)y_old_ / (ElementType)height - gl_center_y;
				ElementType x_new = (ElementType)x_new_ / (ElementType)width - gl_center_x;
				ElementType y_new = (ElementType)y_new_ / (ElementType)height - gl_center_y;

				ElementType z_old = std::sqrt(radius_sqr - x_old*x_old - y_old*y_old);
				ElementType z_new = std::sqrt(radius_sqr - x_new*x_new - y_new*y_new);
				/**
					Compute the rotation axis
				*/
				ElementType x = y_new*z_old - z_new*y_old;
				ElementType y = z_new*x_old - x_new*z_old;
				ElementType z = x_new*y_old - y_new*x_old;
				
				/**
					Normalie the axis
				*/
				ElementType n = std::sqrt(x*x + y*y + z*z);
				x /= n; y /= n; z /= n;

				/** 
					Computation of rotation angle
				*/
				ElementType w = (ElementType)5.0 * std::acos((x_new*x_old + y_new*y_old + z_new*z_old) /
				(std::sqrt(x_new*x_new + y_new*y_new + z_new*z_new) * std::sqrt(x_old*x_old + y_old*y_old + z_old*z_old)));

				gl_quaterion *= pointcloud::Quaterion<ElementType>(w, x, y, z);
			}
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

			/**
				Enables use of glVertexPointer and glColorPointer when drawing with glDrawArrays/
			*/
			glEnableClientState(GL_VERTEX_ARRAY);
			if (color) {
				glEnableClientState(GL_COLOR_ARRAY);
			}
			if (normals) {
				glEnableClientState(GL_NORMAL_ARRAY);
			}
			//glEnableClientState(GL_INDEX_ARRAY);

				glLoadIdentity();

				/**
					Increase or decrease the entire pointcloud by the factor gl_zoom
				*/

				glScalef(1.0f / gl_zoom, 1.0f / gl_zoom, 1.0f / gl_zoom);
				/**
					Translate the pointcloud by the chosen centerpoint
				*/

				glTranslatef(gl_center_x * gl_zoom, gl_center_y * gl_zoom, gl_center_z * gl_zoom);
				
				/**
					Roatate the entire pointcloud around the x- and y-axis
				*/
				float gl_rot_x, gl_rot_y, gl_rot_z;
				gl_quaterion.getEulerAngles(gl_rot_x, gl_rot_y, gl_rot_z);
				glRotatef( -1.0f *math::toDeg<ElementType>(gl_rot_x), 1.0f, 0.0, 0.0);
				glRotatef(  1.0f *math::toDeg<ElementType>(gl_rot_y), 0.0, 1.0f, 0.0);
				glRotatef(  1.0f *math::toDeg<ElementType>(gl_rot_z), 0.0, 0.0, 1.0f);

				/**
					Determine the size of the points
				*/
				glPointSize((GLfloat) gl_pointsize);

				/**
					Link the points, colors and normals for drawing
				*/
				glVertexPointer(3, GL_FLOAT, 0, points);
				if (color) {
					glColorPointer(3, GL_UNSIGNED_BYTE, 0, color);
				}
				if (normals) {
					glNormalPointer(GL_FLOAT, 0, normals);
				}
				glDrawArrays(GL_POINTS, 0, (GLsizei) number_of_elements);
				//////glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(indices.data()));

			/**
				Disables use of glVertexPointer and glColorPointer when drawing with glDrawArrays/
			*/
			glDisableClientState(GL_VERTEX_ARRAY);
			if (color) {
				glDisableClientState(GL_COLOR_ARRAY);
			}
			if (normals) {
				glDisableClientState(GL_NORMAL_ARRAY);
			}
				/**
					Draw axis
				*/				
				glLineWidth(2.0f);
				glBegin(GL_LINES);
					glColor4f(1.0, 0.0, 0.0, 0.0);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(0.25f*gl_zoom, 0.0, 0.0);
				glEnd();
				glBegin(GL_LINES);
					glColor4f(0.0, 1.0, 0.0, 0.0);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(0.0, 0.25f*gl_zoom, 0.0);
				glEnd();
				glBegin(GL_LINES);
					glColor4f(0.0, 0.0, 1.0, 0.0);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(0.0, 0.0, 0.25f*gl_zoom);
				glEnd();

			glPopMatrix();

			glutSwapBuffers();
		}

		///////**
		//////	Draw the pointcloud with double accurancy
		//////*/
		//////void drawd() 
		//////{
		//////	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		//////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//////	glMatrixMode(GL_MODELVIEW);
		//////	glPushMatrix();
		//////	
		//////	/**
		//////		Enables use of glVertexPointer and glColorPointer when drawing with glDrawArrays/
		//////	*/
		//////	glEnableClientState(GL_VERTEX_ARRAY);
		//////	if (color) {
		//////		glEnableClientState(GL_COLOR_ARRAY);
		//////	}
		//////	if (normals) {
		//////		glEnableClientState(GL_NORMAL_ARRAY);
		//////	}
		//////	//glEnableClientState(GL_INDEX_ARRAY);	
		//////		
		//////		glLoadIdentity();
		//////		
		//////		/**
		//////			Increase or decrease the entire pointcloud by the factor gl_zoom
		//////		*/

		//////		glScaled(1.0 / gl_zoom, 1.0 / gl_zoom, 1.0 / gl_zoom);
		//////		/**
		//////			Translate the pointlcoud by the chosen centerpoint
		//////		*/

		//////		glTranslated(gl_center_x * gl_zoom, gl_center_y * gl_zoom, gl_center_z * gl_zoom);
		//////		
		//////		/**
		//////			Roatate the entire pointcloud around the x- and y-axis
		//////		*/
		//////		ElementType gl_rot_x, gl_rot_y, gl_rot_z;
		//////		gl_quaterion.getEulerAngles(gl_rot_x, gl_rot_y, gl_rot_z);
		//////		glRotated(-1.0f *math::toDeg<ElementType>(gl_rot_x), 1.0f, 0.0, 0.0);
		//////		glRotated( 1.0f *math::toDeg<ElementType>(gl_rot_y), 0.0, 1.0f, 0.0);
		//////		glRotated( 1.0f *math::toDeg<ElementType>(gl_rot_z), 0.0, 0.0, 1.0f);

		//////		/**
		//////			Determine the size of the points
		//////		*/
		//////		glPointSize((GLdouble)gl_pointsize);

		//////						/**
		//////			Link the points, colors and normals for drawing
		//////		*/
		//////		glVertexPointer(3, GL_DOUBLE, 0, points);
		//////		if (color) {
		//////			glColorPointer(3, GL_UNSIGNED_BYTE, 0, color);
		//////		}
		//////		if (normals) {
		//////			glNormalPointer(GL_DOUBLE, 0, normals);
		//////		}
		//////		glDrawArrays(GL_POINTS, 0, (GLsizei) number_of_elements);
		//////		//////std::vector<GLuint> indices;
		//////		//////// populate vertices
		//////		//////glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(indices.data()));

		//////		/**
		//////			Draw axis
		//////		*/				
		//////		glLineWidth(2.0f);
		//////		glBegin(GL_LINES);
		//////			glColor4f(1.0, 0.0, 0.0, 0.0);
		//////			glVertex3d(0.0, 0.0, 0.0);
		//////			glVertex3d(0.25*gl_zoom, 0.0, 0.0);
		//////		glEnd();
		//////		glBegin(GL_LINES);
		//////			glColor4f(0.0, 1.0, 0.0, 0.0);
		//////			glVertex3d(0.0, 0.0, 0.0);
		//////			glVertex3d(0.0, 0.25*gl_zoom, 0.0);
		//////		glEnd();
		//////		glBegin(GL_LINES);
		//////			glColor4f(0.0, 0.0, 1.0, 0.0);
		//////			glVertex3d(0.0, 0.0, 0.0);
		//////			glVertex3d(0.0, 0.0, 0.25*gl_zoom);
		//////		glEnd();


		//////	/**
		//////		Disables use of glVertexPointer and glColorPointer when drawing with glDrawArrays/
		//////	*/
		//////	glDisableClientState(GL_VERTEX_ARRAY);
		//////	if (color) {
		//////		glDisableClientState(GL_COLOR_ARRAY);
		//////	}
		//////	if (normals) {
		//////		glDisableClientState(GL_NORMAL_ARRAY);
		//////	}

		//////	glPopMatrix();

		//////	glutSwapBuffers();

		//////}

	private:

		/**
			Container with points, colors, normals and indices
		*/
		std::vector<PlotContainer<ElementType>> plot_container;

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
			Quaterion
		*/
		pointcloud::Quaterion<ElementType> gl_quaterion;
		
		/**
			Point size
		*/
		size_t gl_pointsize;

		/**
			Zoom factor
		*/
		ElementType gl_zoom;

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
			clear();

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
			clear();
		}

		/**
			Clear
		*/
		void clear() 
		{
			plot3d_instances.clear();
			
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

			@param[in] pointcloud_ Pointcloud
		*/
		void setPointcloud(const pointcloud::Pointcloud<ElementType>& pointcloud_)
		{
			plot3d_instances.getCurrentPlot3DInstance().setPointcloud(pointcloud_);
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
			//if (height_ > width_) {
			//	glOrtho(0.0, 1.0,0.0, (double)height_ / (double)width_, -1.0, 1.0);
			//}
			//else{
			//	glOrtho(0.0, (double)width_ / (double)height_,0.0, 1.0, -1.0, 1.0);
			//}
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
				sprintf(window_name_, "Pointcloud %d", (int) plot3d_instances.getNumberOfPlot3D() - 1);
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
		static void key(unsigned char key_, int x_, int y_)
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
			if (direction_ == 1) {
				plot3d_instances.getCurrentPlot3DInstance().zoomOut();
			}
			else {
				plot3d_instances.getCurrentPlot3DInstance().zoomIn();
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
			clear();
		}

		/**
			Destructor
		*/
		~StaticPlot3DInstance()
		{
			clear();
		}

		/**
			Clear
		*/
		void clear()
		{
			for (size_t i = 0; i < number_of_plot3d; i++) {
				plot3d_instances[i].clear();
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

#endif /* UTILS_PLOT3D_H_*/	