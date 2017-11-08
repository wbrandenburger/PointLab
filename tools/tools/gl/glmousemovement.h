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

#ifndef GL_MOUSEMOVEMENT_H_
#define GL_MOUSEMOVEMENT_H_

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include "tools/pointcloud/pointcloudnodes.h"

#include "tools/pointcloud/quaterion.h"

namespace gl
{
	template<typename ElementType> class GLMouseMovement
	{
	public:

		/**
			Constructor
		*/
		GLMouseMovement()
		{
			zoom_factor_ = (ElementType) 1.1;
			rotation_factor_ = (ElementType)5.0;
			translation_factor_ = (ElementType) 0.001;
			
			ElementType center = (ElementType) 0.5;
			gl_center_.setPoint(center, center, center);

			gl_zoom_ = (ElementType) 1.0;
		}

		/**
			Constructor

			@param[in] zoom_factor Zoom factor
			@param[in] rotation_factor Rotation factor
			@param[in] translation_factor Translation factor
		*/
		GLMouseMovement(ElementType zoom_factor, 
			ElementType rotation_factor, ElementType translation_factor)
		{
			zoom_factor_ = zoom_factor;
			rotation_factor_ = rotation_factor;
			translation_factor_ = translation_factor;

			ElementType center = (ElementType) 0.5;
			gl_center_.setPoint(center, center, center);

			gl_zoom = (ElementType) 1.0;
		}

		/**
			Destructor
		*/
		~GLMouseMovement()
		{
			clearMemory();
		}

		/**
			Clear
		*/
		void clearMemory()
		{
		}

		///**
		//	Copy constructor

		//	@param[in] mouse_movement An instance of class GLMouseMovement
		//*/
		//GLMouseMovement(const GLMouseMovement<ElementType>& gl_mouse_movement) = delete;

		///**
		//	Copy constructor

		//	@param[in] mouse_movement An instance of class GLMouseMovement
		//*/
		//GLMouseMovement(const GLMouseMovement<ElementType>&& gl_mouse_movement) = delete;

		///**
		//	Operator =

		//	@param[in] mouse_movement An instance of class GLMouseMovement
		//	@return Returns reference to the current instance
		//*/
		//GLMouseMovement& operator=(const GLMouseMovement<ElementType>& gl_mouse_movement) = delete;

		///**
		//	Operator =

		//	@param[in] mouse_movement An instance of class GLMouseMovement
		//	@return Returns reference to the current instance
		//*/
		//GLMouseMovement& operator=(const GLMouseMovement<ElementType>&& gl_mouse_movement) = delete;

		/**
			Set GLMouseMovement

			@param[in] zoom_factor Zoom factor
			@param[in] rotation_factor Rotation factor
			@param[in] translation_factor Translation factor
		*/
		void setGLMouseMovement(ElementType zoom_factor, 
			ElementType rotation_factor, ElementType translation_factor)
		{
			zoom_factor_ = zoom_factor;
			rotation_factor_ = rotation_factor;
			translation_factor_ = translation_factor;
		}

		/**
			Set zoom factor

			@param[in] zoom_factor Zoom factor
		*/
		void setZoomFactor(ElementType zoom_factor)
		{
			zoom_factor_ = zoom_factor;
		}

		/**
			Set rotation factor

			@param[in] rotation_factor Rotation factor
		*/
		void setRotationFactor(ElementType rotation_factor)
		{
			rotation_factor_ = rotation_factor;
		}

		/**
			Set translation factor

			@param[in] translation_factor Translation factor
		*/
		void setTranslationFactor(ElementType translation_factor)
		{
			translation_factor_ = translation_factor;
		}

		/**
			Set zoom parameter

			@param[in] gl_zoom Zoom parameter
		*/
		void setZoom(ElementType gl_zoom)
		{
			gl_zoom_ = gl_zoom;
		}

		/**
			Set center parameter

			@param[in] gl_center Center parameter
		*/
		void setCenter(pointcloud::XYZNode<ElementType> gl_center)
		{
			gl_center_ = gl_center;
		}

		/**
			Set rotation parameter

			@param[in] gl_quaterion Rotation parameter
		*/
		void setRotation(pointcloud::Quaterion<ElementType> gl_rotation) 
		{
			gl_rotation_ = gl_rotation;
		}

		/**
			Get center parameter

			@return center parameter
		*/
		pointcloud::XYZNode<ElementType> getCenter() const
		{
			return gl_center_;
		}

		/**
			Get center x-value

			@return center x-value
		*/
		ElementType getCenterX() const
		{
			return gl_center_.getPoint(0);
		}

		/**
			Get center y-value

			@return center y-value
		*/
		ElementType getCenterY() const
		{
			return gl_center_.getPoint(1);
		}

		/**
			Get center z-value

			@return center z-value
		*/
		ElementType getCenterZ() const
		{
			return gl_center_.getPoint(2);
		}

		/**
			Get rotation parameter

			@return rotation parameter
		*/
		pointcloud::Quaterion<ElementType> getRotation() const
		{
			return gl_rotation_;
		}

		/**
			Get zoom parameter

			@return zoom parameter
		*/
		ElementType getZoom() const
		{
			return gl_zoom_;
		}

		/**
			Set translation
			
			@param[in] x Mouse position in x-direction
			@param[in] y Mouse position in y-direction
		*/
		void setTranslation(int x, int y)
		{
			gl_center_.setPoint(gl_center_.getPoint(0) + (ElementType)x * translation_factor_, 0);
			gl_center_.setPoint(gl_center_.getPoint(1) - (ElementType)y * translation_factor_, 1);
		}

		/**
			Set zoom

			@param[in] direction_ Direction in which the wheel is turned: 1 for up and -1 for down
		*/
		void setZoom(int direction)
		{
			switch (direction){
			case 1: gl_zoom_ = gl_zoom_ * zoom_factor_; break;
			case -1:gl_zoom_ = gl_zoom_ / zoom_factor_; break;
			}
		}

		/**
			Set rotation

			@param[in] x_old Old mouse position in x-direction
			@param[in] y_old Old mouse position in y-direction
			@param[in] x_new New mouse position in x-direction
			@param[in] y_new New mouse position in y-direction
		*/
		void SetRotation(int x_old, int y_old, int x_new, int y_new)
		{
			/**
				Compute the radius of the imaginary sphere
			*/
			ElementType max_x = gl_center_.getPoint(0) > (ElementType)0.5 ? gl_center_.getPoint(0) : (ElementType)1.0 - gl_center_.getPoint(0);
			ElementType max_y = gl_center_.getPoint(1) > (ElementType)0.5 ? gl_center_.getPoint(1) : (ElementType)1.0 - gl_center_.getPoint(1);

			ElementType radius_sqr = max_x*max_x + max_y*max_y;	

			int width = glutGet(GLUT_WINDOW_WIDTH);
			int height = glutGet(GLUT_WINDOW_HEIGHT);

			if (x_new > 0 && x_new < width && y_new > 0 && y_new < height &&
				!(x_new - x_old == 0 && y_new - y_old == 0)) {

				/**
					Normalize the coordinates
				*/
				ElementType x_old_norm = (ElementType)x_old / (ElementType)width - gl_center_.getPoint(0);
				ElementType y_old_norm = (ElementType)y_old / (ElementType)height - gl_center_.getPoint(1);
				ElementType x_new_norm = (ElementType)x_new / (ElementType)width - gl_center_.getPoint(0);
				ElementType y_new_norm = (ElementType)y_new / (ElementType)height - gl_center_.getPoint(1);

				ElementType z_old_norm = std::sqrt(radius_sqr - x_old_norm*x_old_norm - y_old_norm*y_old_norm);
				ElementType z_new_norm = std::sqrt(radius_sqr - x_new_norm*x_new_norm - y_new_norm*y_new_norm);

				/**
					Compute the rotation axis
				*/
				ElementType x = y_new_norm*z_old_norm - z_new_norm*y_old_norm;
				ElementType y = z_new_norm*x_old_norm - x_new_norm*z_old_norm;
				ElementType z = x_new_norm*y_old_norm - y_new_norm*x_old_norm;

				/**
					Normalize the axis
				*/
				ElementType n = std::sqrt(x*x + y*y + z*z);
				x /= n; y /= n; z /= n;

				/**
					Computation of rotation angle
				*/
				ElementType w = rotation_factor_ * std::acos((x_new_norm*x_old_norm + y_new_norm*y_old_norm + z_new_norm*z_old_norm) /
					(std::sqrt(x_new_norm*x_new_norm + y_new_norm*y_new_norm + z_new_norm*z_new_norm) *
						std::sqrt(x_old_norm*x_old_norm + y_old_norm*y_old_norm + z_old_norm*z_old_norm)));

				gl_rotation_ *= pointcloud::Quaterion<ElementType>(w, x, y, z);
			}
		}

	private:

		/**
			Zooom factor
		*/
		ElementType zoom_factor_;

		/**
			Rotation factor
		*/
		ElementType rotation_factor_;

		/**
			Normalization of translation
		*/
		ElementType translation_factor_;

		/**
			Center
		*/
		pointcloud::XYZNode<ElementType> gl_center_;

		/**
			Quaterion
		*/
		pointcloud::Quaterion<ElementType> gl_rotation_;

		/**
			Zoom
		*/
		ElementType gl_zoom_;
	};
}

#endif /* GL_MOUSEMOVEMENT_H_ */