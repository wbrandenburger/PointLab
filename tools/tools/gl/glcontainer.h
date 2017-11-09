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
		GLContainer() : mode(NULL),
			points(nullptr), color(nullptr), normals(nullptr), indices(nullptr),
			number_of_vertices(NULL), number_of_indices(NULL)
		{
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		GLContainer(const pointcloud::Pointcloud<ElementType>& pointcloud_ ) : GLContainer()
		{
			mode = GL_POINTS;

			points = pointcloud_.getPointsPtr();

			if (pointcloud_.isColor()) {
				color = pointcloud_.getColorsPtr();
			}
			if (pointcloud_.isNormal()) {
				normals = pointcloud_.getNormalsPtr();
			}

			number_of_vertices = pointcloud_.getNumberOfVertices();

			if (pointcloud_.isTriangle()) {
				number_of_indices = pointcloud_.getNumberOfTriangles() * 3;
				indices = pointcloud_.getTrianglesPtr<unsigned int>();
			}
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		GLContainer(GLParams mode_, const pointcloud::Pointcloud<ElementType>& pointcloud_ ) : GLContainer()
		{
			setMode(mode_);

			points = pointcloud_.getPointsPtr();

			if (pointcloud_.isColor()) {
				color = pointcloud_.getColorsPtr();
			}
			if (pointcloud_.isNormal()) {
				normals = pointcloud_.getNormalsPtr();
			}

			number_of_vertices = pointcloud_.getNumberOfVertices();

			if (pointcloud_.isTriangle()) {
				number_of_indices = pointcloud_.getNumberOfTriangles() * 3;
				indices = pointcloud_.getTrianglesPtr<unsigned int>();
			}
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of elements
		*/
		GLContainer(ElementType* points_, size_t number_of_vertices_) : GLContainer() 
		{
			mode = GL_POINTS;

			number_of_vertices = number_of_vertices_;

			points = points_;
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of elements
		*/
		GLContainer(GLParams mode_, ElementType* points_, size_t number_of_vertices_) : GLContainer() 
		{
			setMode(mode_);

			number_of_vertices = number_of_vertices_;

			points = points_;
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] indices_ indices
			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_indices_ Number of indices
		*/
		GLContainer( ElementType* points_, unsigned int* indices_,
			size_t number_of_vertices_, size_t number_of_indices_) : GLContainer()
		{
			mode = GL_POINTS;

			number_of_vertices = number_of_vertices_;

			switch (mode_) {
			case GLParams::LINES: number_of_indices = number_of_indices_* 2; break;
			case GLParams::TRIANGLES: number_of_indices = number_of_indices_ * 3; break;
			}

			points = points_;
			indices = indices_;
		}

		/**
			Constructor

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] indices_ indices
			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_indices_ Number of indices
		*/
		GLContainer(GLParams mode_, ElementType* points_, unsigned int* indices_,
			size_t number_of_vertices_, size_t number_of_indices_) : GLContainer()
		{
			setMode(mode_);

			number_of_vertices = number_of_vertices_;

			switch (mode_) {
			case GLParams::LINES: number_of_indices = number_of_indices_* 2; break;
			case GLParams::TRIANGLES: number_of_indices = number_of_indices_ * 3; break;
			}

			points = points_;
			indices = indices_;
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
				normals = nullptr;
			}
			if (indices) {
				delete[] indices;
				indices = nullptr;
			}
		}

		/**
			Copy constructor
	
			@param[in] gl_container_ An instance of class GLContainer
		*/
		GLContainer(const GLContainer <ElementType>& gl_container_) : GLContainer()
		{
			copy(gl_container_);
		}
		
		/**
			Copy constructor
	
			@param[in] gl_container_ An instance of class GLContainer
		*/

		GLContainer(const GLContainer <ElementType>&& gl_container_) : GLContainer()
		{
			copy(gl_container_);
		}
		/**
			Operator =
	
			@param[in] gl_container_ An instance of class GLContainer
		*/
		GLContainer& operator=(const GLContainer<ElementType>& gl_container_)
		{
			clearMemory();

			copy(gl_container_);

			return *this;
		}
	
		/**
			Operator =
	
			@param[in] gl_container_ An instance of class GLContainer
		*/
		GLContainer& operator=(const GLContainer<ElementType>&& gl_container_)
		{
			clearMemory();

			copy(gl_container_);

			return *this;
		}

		/**
			Copy data

			@param[in] gl_container_ An instance of class GLContainer
		*/
		void copy(const GLContainer<ElementType>& gl_container_)
		{
			mode = gl_container_.getMode();
			number_of_vertices = gl_container_.getNumberOfVertices();
			number_of_indices = gl_container_.getNumberOfIndices();

			points = new ElementType[number_of_vertices * 3];
			std::memcpy(points, gl_container_.getPoints(), sizeof(ElementType) * number_of_vertices * 3);

			if (gl_container_.isColor()) {
				color = new uint8_t[number_of_vertices * 3];
				std::memcpy(color, gl_container_.getColor(), sizeof(uint8_t) * number_of_vertices * 3);
			}
			if (gl_container_.isNormal()) {
				normals = new ElementType[number_of_vertices * 3];
				std::memcpy(normals, gl_container_.getNormals(), sizeof(ElementType) * number_of_vertices * 3);
			}
			if (gl_container_.isIndice()) {
				indices = new unsigned int[number_of_indices];
				std::memcpy(indices, gl_container_.getIndices(), sizeof(unsigned int)*number_of_indices);
			}
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		void setGLContainer(pointcloud::Pointcloud<ElementType>& pointcloud_ ) 
		{
			mode = GL_POINTS;

			points = pointcloud_.getPointsPtr();

			if (pointcloud_.isColor()) {
				color = pointcloud_.getColorsPtr();
			}
			if (pointcloud_.isNormal()) {
				normals = pointcloud_.getNormalsPtr();
			}

			number_of_vertices = pointcloud_.getNumberOfVertices();

			if (pointcloud_.isTriangle()) {
				number_of_indices = pointcloud_.getNumberOfTriangles() * 3;
				indices = pointcloud_.getTrianglesPtr<unsigned int>();
			}
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] pointcloud_ Pointcloud
		*/
		void setGLContainer(GLParams mode_, pointcloud::Pointcloud<ElementType>& pointcloud_ ) 
		{
			setMode(mode_);

			points = pointcloud_.getPointsPtr();

			if (pointcloud_.isColor()) {
				color = pointcloud_.getColorsPtr();
			}
			if (pointcloud_.isNormal()) {
				normals = pointcloud_.getNormalsPtr();
			}

			number_of_vertices = pointcloud_.getNumberOfVertices();

			if (pointcloud_.isTriangle()) {
				number_of_indices = pointcloud_.getNumberOfTriangles() * 3;
				indices = pointcloud_.getTrianglesPtr<unsigned int>();
			}
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of vertices
		*/
		void setGLContainer(ElementType* points_, size_t number_of_vertices_)
		{
			mode = GL_POINTS;

			number_of_vertices = number_of_vertices_;

			points = points_;
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] number_of_vertices_ Number of vertices
		*/
		void setGLContainer(GLParams mode_, ElementType* points_, size_t number_of_vertices_)
		{
			setMode(mode_);

			number_of_vertices = number_of_vertices_;

			points = points_;
		}

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] indices_ Indices
			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_indices_ Number of indices
		*/
		void setGLContainer(ElementType* points_, unsigned int* indices_,
			size_t number_of_vertices_, size_t number_of_indices_)
		{
			mode = GL_POINTS;

			number_of_vertices = number_of_vertices_;

			switch (mode_) {
			case GLParams::LINES: number_of_indices = number_of_indices_ * 2; break;
			case GLParams::TRIANGLES: number_of_indices = number_of_indices_ * 3; break;
			}

			points = points_;
			indices = indices_;
		}	

		/**
			Set plot container

			@param[in] mode_ Specifies what kind of primitives to render
			@param[in] points_ Points
			@param[in] indices_ Indices
			@param[in] number_of_vertices_ Number of vertices
			@param[in] number_of_indices_ Number of indices
		*/
		void setGLContainer(GLParams mode_, ElementType* points_, unsigned int* indices_,
			size_t number_of_vertices_, size_t number_of_indices_)
		{
			setMode(mode_);

			number_of_vertices = number_of_vertices_;

			switch (mode_) {
			case GLParams::LINES: number_of_indices = number_of_indices_ * 2; break;
			case GLParams::TRIANGLES: number_of_indices = number_of_indices_ * 3; break;
			}

			points = points_;
			indices = indices_;
		}

		/**
			Get the OpenGl specifier for the mode what kind of primitives to render

			@param[in] mode_ Specifies what kind of primitives to render
		*/
		void setMode(GLParams mode_)
		{
			switch (mode_) {
			case GLParams::POINTS: mode = GL_POINTS; break;
			case GLParams::LINES: mode = GL_LINES; break;
			case GLParams::TRIANGLES: mode = GL_TRIANGLES; break;
			}
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
		uint8_t* getColor() const
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
			Get Indices
			
			@return indices
		*/
		unsigned int* getIndices() const
		{
			return indices;
		}

		/**
			Get number of vertices

			@return Number of vertices
		*/
		size_t getNumberOfVertices() const
		{
			return number_of_vertices;
		}

		/**
			Get number of triangles

			@return Number of triangles
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
			return (color != nullptr);
		}

		/**
			Returns true if normals are set

			@return True if normals are set
		*/
		bool isNormal() const
		{
			return (normals != nullptr);
		}

		/**
			Returns true if triangles are set

			@return True if triangles are set
		*/
		bool isIndice() const
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
		uint8_t* color;

		/**
			Normals
		*/
		ElementType* normals;

		/**
			Indices
		*/
		unsigned int* indices;

		/**
			Number of vertices
		*/
		size_t number_of_vertices;

		/**
			Number of indices
		*/
		size_t number_of_indices;
	};
}

#endif /* GL_GLPLOTCONTAINER_H_ */