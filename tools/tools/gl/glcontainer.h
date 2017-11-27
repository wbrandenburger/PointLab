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

#ifndef GL_GLCONTAINER_H_
#define GL_GLCONTAINER_H_

#include "tools/pointcloud/pointcloud.h"

namespace gl
{
	template<typename ElementType> class GLContainer 
	{
	public:
		/**
			Constructor
		*/
		GLContainer() : mode_(NULL),
			points_(nullptr), color_(nullptr), normals_(nullptr), indices_(nullptr),
			number_of_vertices_(NULL), number_of_indices_(NULL)
		{
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		GLContainer(const pointcloud::Pointcloud<ElementType>& pointcloud ) : GLContainer()
		{
			mode_ = GL_POINTS;

			points_ = pointcloud.getPointsPtr();

			if (pointcloud.isColor()) {
				color_ = pointcloud.getColorsPtr();
			}
			if (pointcloud.isNormal()) {
				normals_ = pointcloud.getNormalsPtr();
			}

			number_of_vertices_ = pointcloud.getNumberOfVertices();

			if (pointcloud.isTriangle()) {
				number_of_indices_ = pointcloud.getNumberOfTriangles() * 3;
				indices_ = pointcloud.getTrianglesPtr<unsigned int>();
			}
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		GLContainer(GLParams mode, const pointcloud::Pointcloud<ElementType>& pointcloud ) : GLContainer()
		{
			setMode(mode);

			points_ = pointcloud.getPointsPtr();

			if (pointcloud.isColor()) {
				color_ = pointcloud.getColorsPtr();
			}
			if (pointcloud.isNormal()) {
				normals_ = pointcloud.getNormalsPtr();
			}

			number_of_vertices_ = pointcloud.getNumberOfVertices();

			if (pointcloud.isTriangle()) {
				number_of_indices_ = pointcloud.getNumberOfTriangles() * 3;
				indices_ = pointcloud.getTrianglesPtr<unsigned int>();
			}
		}

		/**
			Constructor

			@param[in] points_ Points
		*/
		GLContainer(const utils::Matrix<ElementType>& points) : GLContainer() 
		{
			mode = GL_POINTS;

			number_of_vertices_ = points.getRows();

			points_ = points.getAllocatedPtr(); 
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
		*/
		GLContainer(GLParams mode, const utils::Matrix<ElementType>& points) : GLContainer()
		{
			setMode(mode);

			number_of_vertices_ = points.getRows();

			points_ = points.getAllocatedPtr(); 
		}

		/**
			Constructor

			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of elements
		*/
		GLContainer(ElementType* points, size_t number_of_vertices) : GLContainer() 
		{
			mode = GL_POINTS;

			number_of_vertices_ = number_of_vertices;

			points_ = new ElementType[number_of_vertices_ * 3]; 
			std::memcpy(points_, points, sizeof(ElementType) * number_of_vertices_ * 3);
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of elements
		*/
		GLContainer(GLParams mode, ElementType* points, size_t number_of_vertices) : GLContainer() 
		{
			setMode(mode);

			number_of_vertices_ = number_of_vertices;

			points_ = new ElementType[number_of_vertices_ * 3];
			std::memcpy(points_, points, sizeof(ElementType) * number_of_vertices_ * 3);
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] indices_ indices
			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_indices_ Number of indices
		*/
		GLContainer(GLParams mode, ElementType* points, unsigned int* indices,
			size_t number_of_vertices, size_t number_of_indices) : GLContainer()
		{
			setMode(mode);

			number_of_vertices_ = number_of_vertices;

			switch (mode) {
			case GLParams::LINES: number_of_indices_ = number_of_indices * 2; break;
			case GLParams::TRIANGLES: number_of_indices_ = number_of_indices * 3; break;
			}

			points_ = new ElementType[number_of_vertices_ * 3];
			std::memcpy(points_, points, sizeof(ElementType) * number_of_vertices_ * 3);

			indices_ = new unsigned int[number_of_indices_];
			std::memcpy(indices_, indices, sizeof(unsigned int) * number_of_indices_);
		}

		/**
			Destructor
		*/
		~GLContainer()
		{
			clearMemory();
		}

		/**
			Clear Memory
		*/
		void clearMemory()
		{
			if (points_) {
				delete[] points_;
				points_ = nullptr;
			}
			if (color_) {
				delete[] color_;
				color_ = nullptr;
			}
			if (normals_) {
				delete[] normals_;
				normals_ = nullptr;
			}
			if (indices_) {
				delete[] indices_;
				indices_ = nullptr;
			}
		}

		/**
			Copy constructor
	
			@param[in] gl_container_ An instance of class GLContainer
		*/
		GLContainer(const GLContainer <ElementType>& gl_container) : GLContainer()
		{
			copy(gl_container);
		}
		
		/**
			Copy constructor
	
			@param[in] gl_container_ An instance of class GLContainer
		*/

		GLContainer(const GLContainer <ElementType>&& gl_container) : GLContainer()
		{
			copy(gl_container);
		}
		/**
			Operator =
	
			@param[in] gl_container_ An instance of class GLContainer
		*/
		GLContainer& operator=(const GLContainer<ElementType>& gl_container)
		{
			clearMemory();

			copy(gl_container);

			return *this;
		}
	
		/**
			Operator =
	
			@param[in] gl_container_ An instance of class GLContainer
		*/
		GLContainer& operator=(const GLContainer<ElementType>&& gl_container)
		{
			clearMemory();

			copy(gl_container);

			return *this;
		}

		/**
			Copy data

			@param[in] gl_container_ An instance of class GLContainer
		*/
		void copy(const GLContainer<ElementType>& gl_container)
		{
			mode_ = gl_container.getMode();
			number_of_vertices_ = gl_container.getNumberOfVertices();
			number_of_indices_ = gl_container.getNumberOfIndices();

			points_ = new ElementType[number_of_vertices_ * 3];
			std::memcpy(points_, gl_container.getPoints(), sizeof(ElementType) * number_of_vertices_ * 3);

			if (gl_container.isColor()) {
				color_ = new uint8_t[number_of_vertices_ * 3];
				std::memcpy(color_, gl_container.getColor(), sizeof(uint8_t) * number_of_vertices_ * 3);
			}
			if (gl_container.isNormal()) {
				normals_ = new ElementType[number_of_vertices_ * 3];
				std::memcpy(normals_, gl_container.getNormals(), sizeof(ElementType) * number_of_vertices_ * 3);
			}
			if (gl_container.isIndice()) {
				indices_ = new unsigned int[number_of_indices_];
				std::memcpy(indices_, gl_container.getIndices(), sizeof(unsigned int)*number_of_indices_);
			}
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		void setGLContainer(pointcloud::Pointcloud<ElementType>& pointcloud ) 
		{
			mode_ = GL_POINTS;

			points_ = pointcloud.getPointsPtr();

			if (pointcloud.isColor()) {
				color_ = pointcloud_.getColorsPtr();
			}
			if (pointcloud_.isNormal()) {
				normals_ = pointcloud.getNormalsPtr();
			}

			number_of_vertices_ = pointcloud.getNumberOfVertices();

			if (pointcloud.isTriangle()) {
				number_of_indices_ = pointcloud.getNumberOfTriangles() * 3;
				indices_ = pointcloud.getTrianglesPtr<unsigned int>();
			}
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		void setGLContainer(GLParams mode, pointcloud::Pointcloud<ElementType>& pointcloud) 
		{
			setMode(mode);

			points_ = pointcloud.getPointsPtr();

			if (pointcloud_.isColor()) {
				color_ = pointcloud.getColorsPtr();
			}
			if (pointcloud_.isNormal()) {
				normals_ = pointcloud.getNormalsPtr();
			}

			number_of_vertices_ = pointcloud.getNumberOfVertices();

			if (pointcloud.isTriangle()) {
				number_of_indices_ = pointcloud.getNumberOfTriangles() * 3;
				indices_ = pointcloud.getTrianglesPtr<unsigned int>();
			}
		}

		/**
			Set plot container

			@param[in] points_ Points
		*/
		void setGLContainer(const utils::Matrix<ElementType>& points) 
		{
			mode = GL_POINTS;

			number_of_vertices_ = points.getRows();

			points_ = points.getAllocatedPtr(); 
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
		*/
		void setGLContainer(GLParams mode, const utils::Matrix<ElementType>& points)
		{
			setMode(mode);

			number_of_vertices_ = points.getRows();

			points_ = points.getAllocatedPtr(); 
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of vertices
		*/
		void setGLContainer(ElementType* points, size_t number_of_vertices)
		{
			mode = GL_POINTS;

			number_of_vertices_ = number_of_vertices;

			points_ = new ElementType[number_of_vertices_ * 3];
			std::memcpy(points_, points, sizeof(ElementType) * number_of_vertices_ * 3);
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of vertices
		*/
		void setGLContainer(GLParams mode, ElementType* points, size_t number_of_vertices)
		{
			setMode(mode);

			number_of_vertices_ = number_of_vertices;

			points_ = new ElementType[number_of_vertices_ * 3];
			std::memcpy(points_, points, sizeof(ElementType) * number_of_vertices_ * 3);
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] indices_ Indices
			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_indices_ Number of indices
		*/
		void setGLContainer(GLParams mode, ElementType* points, unsigned int* indices,
			size_t number_of_vertices, size_t number_of_indices)
		{
			setMode(mode);

			number_of_vertices_ = number_of_vertices;

			switch (mode) {
			case GLParams::LINES: number_of_indices_ = number_of_indices * 2; break;
			case GLParams::TRIANGLES: number_of_indices_ = number_of_indices * 3; break;
			}

			points_ = new ElementType[number_of_vertices_ * 3];
			std::memcpy(points_, points, sizeof(ElementType) * number_of_vertices_ * 3);
			
			indices_ = new unsigned int[number_of_indices_];
			std::memcpy(indices_, indices, sizeof(unsigned int) * number_of_indices_);
		}

		/**
			Get the OpenGl specifier for the mode what kind of primitives to render

			@param[in] mode_ Specifies what kind of primitives to render
		*/
		void setMode(GLParams mode)
		{
			switch (mode) {
			case GLParams::POINTS: mode_ = GL_POINTS; break;
			case GLParams::LINES: mode_ = GL_LINES; break;
			case GLParams::TRIANGLES: mode_ = GL_TRIANGLES; break;
			}
		}

		/**
			Get the mode which specifies what kind of primitives to render
		*/
		GLenum getMode() const
		{
			return mode_;
		}

		/**
			Get points

			@return Points
		*/
		ElementType* getPoints() const
		{
			return points_;
		}

		/**
			Get colors

			@return Colors
		*/
		uint8_t* getColor() const
		{
			return color_;
		}

		/**
			Get normals

			@return Normals
		*/
		ElementType* getNormals() const
		{
			return normals_;
		}

		/**
			Get Indices
			
			@return indices
		*/
		unsigned int* getIndices() const
		{
			return indices_;
		}

		/**
			Get number of vertices

			@return Number of vertices
		*/
		size_t getNumberOfVertices() const
		{
			return number_of_vertices_;
		}

		/**
			Get number of triangles

			@return Number of triangles
		*/
		size_t getNumberOfIndices() const
		{
			return number_of_indices_;
		}

		/**
			Returns true if points are set

			@return True if points are set
		*/
		bool isPoint() const
		{
			return (points_ != nullptr);
		}

		/**
			Returns true if colors are set

			@return True if colors are set
		*/
		bool isColor() const
		{
			return (color_ != nullptr);
		}

		/**
			Returns true if normals are set

			@return True if normals are set
		*/
		bool isNormal() const
		{
			return (normals_ != nullptr);
		}

		/**
			Returns true if triangles are set

			@return True if triangles are set
		*/
		bool isIndice() const
		{
			return (indices_ != nullptr);
		}

	private:

		/**
			Specifies what kind of primitives to render
		*/
		GLenum mode_;

		/**
			Points
		*/
		ElementType* points_;

		/**
			Color
		*/
		uint8_t* color_;

		/**
			Normals
		*/
		ElementType* normals_;

		/**
			Indices
		*/
		unsigned int* indices_;

		/**
			Number of vertices
		*/
		size_t number_of_vertices_;

		/**
			Number of indices
		*/
		size_t number_of_indices_;
	};
}

#endif /* GL_GLPLOTCONTAINER_H_ */