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
			translation_factor_ = (ElementType) 0.001;

			ElementType center = (ElementType) 0.5;
			gl_center_.setPoint(center, center, center);

			gl_zoom_ = (ElementType) 1.0;
		}

		/**
			Constructor

			@param[in] zoom_factor Zoom factor
			@param[in] translation_factor Translation factor
		*/
		GLMouseMovement(ElementType zoom_factor, ElementType translation_factor)
		{
			zoom_factor_ = zoom_factor;
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

		/**
			Copy constructor

			@param[in] mouse_movement An instance of class GLMouseMovement
		*/
		GLMouseMovement(const GLMouseMovement<ElementType>& gl_mouse_movement) = delete;

		/**
			Copy constructor

			@param[in] mouse_movement An instance of class GLMouseMovement
		*/
		GLMouseMovement(const GLMouseMovement<ElementType>&& gl_mouse_movement) = delete;

		/**
			Operator =

			@param[in] mouse_movement An instance of class GLMouseMovement
			@return Returns reference to the current instance
		*/
		GLMouseMovement& operator=(const GLMouseMovement<ElementType>& gl_mouse_movement) = delete;

		/**
			Operator =

			@param[in] mouse_movement An instance of class GLMouseMovement
			@return Returns reference to the current instance
		*/
		GLMouseMovement& operator=(const GLMouseMovement<ElementType>&& gl_mouse_movement) = delete;

		/**
			Set GLMouseMovement

			@param[in] zoom_factor Zoom factor
			@param[in] translation_factor Translation factor
		*/
		void setGLMouseMovement(ElementType zoom_factor, ElementType translation_factor)
		{
			zoom_factor_ = zoom_factor;
			translation_factor_ = translation_factor;
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
			Get rotation parameter

			@return rotation parameter
		*/
		pointcloud::Quaterion<ElementType> getQuaterion() const
		{
			return gl_quaterion_;
		}

		/**
			Get zoom parameter

			@return zoom parameter
		*/
		ElementType getZoom() const
		{
			return gl_zoom_;
		}

	private:

		/**
			Zooom factor
		*/
		ElementType zoom_factor_;

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
		pointcloud::Quaterion<ElementType> gl_quaterion_;

		/**
			Zoom
		*/
		ElementType gl_zoom_;
	};
}

#endif /* GL_MOUSEMOVEMENT_H_ */