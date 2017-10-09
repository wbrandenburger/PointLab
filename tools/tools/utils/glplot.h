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

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>

#include "eigen3/Eigen/Dense"

#include "tools/utils/matrix.h"

namespace utils
{
#define WINDOW_TITLE_PREFIX "Window 1"

	int CurrentWidth = 800, CurrentHeight = 600, WindowHandle = 0;

	void Initialize(int, char*[]);
	void InitWindow(int, char*[]);
	void ResizeFunction(int, int);
	void RenderFunction(void);

	void Initialize(int argc, char* argv[])
	{
		InitWindow(argc, argv);

		std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void InitWindow(int argc, char* argv[])
	{
		glutInit(&argc, argv);

		glutInitContextVersion(4, 4);
		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
		glutInitContextProfile(GLUT_CORE_PROFILE);

		glutSetOption(
			GLUT_ACTION_ON_WINDOW_CLOSE,
			GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

		glutInitWindowSize(CurrentWidth, CurrentHeight);

		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

		WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

		if (WindowHandle < 1) {
			std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
			std::exit(EXIT_FAILURE);
		}

		glutReshapeFunc(ResizeFunction);
		glutDisplayFunc(RenderFunction);

	}

	void ResizeFunction(int Width, int Height)
	{
		CurrentWidth = Width;
		CurrentHeight = Height;
		glViewport(0, 0, CurrentWidth, CurrentHeight);
	}

	void RenderFunction(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glutSwapBuffers();
		glutPostRedisplay();
	}



	struct point {
		GLfloat x;
		GLfloat y;
	};

	point graph[2000];

	void plot(void)
	{
		for (size_t i = 0; i < 2000; i++) {
			float x = (i - 1000.0) / 100.0;
			graph[i].x = x;
			graph[i].y = sin(x * 10.0) / (1.0 + x * x);
		}


		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_STATIC_DRAW);


	}

}

#endif /* UTILS_GLPLOT_H_*/	