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

#include <iostream>
#include <string>
#include <thread>

#include "pcsimp/pcsimp.hpp"

#include "tools/gl.h"
#include "tools/io.h"
#include "tools/math.h"
#include "tools/pointcloud.h"
#include "tools/parameters.h"
#include "tools/utils.h"

#include "trees/trees.hpp"
#include "eigen3/Eigen/Dense"
int main(int argc, char* argv[]) {

	std::cout << "----------------------- Main -----------------------" << std::endl;

	int i = 0;
	int cores = (unsigned int)std::thread::hardware_concurrency();
	while (i < argc) {
		if (!strcmp(argv[i], "--cores")) {
			i++;
			cores = std::stoi(argv[i]);
		}
		i++;
	}

	//char* file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Ettlingen/Ettlingen1.ply";
	char* file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Sonstiges/buny.ply";
	//char* file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Sonstiges/mesh.ply";
	//char *file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Unikirche/UnikircheII.ply";
	
	/**
		Read data
	*/
	utils::Timer time;
	pointcloud::PointcloudSoA<float> pointcloud_buny;

	time.start();
	if (io::readPly(file, pointcloud_buny)) {
		std::cout << "File with " << pointcloud_buny.getNumberOfVertices() << " point has been read in "
			<< time.stop() << " s into Pointcloud" << std::endl;
	}
	std::cout << pointcloud_buny << std::endl;

	pointcloud::PointcloudSoA<float> pointcloud_meshgrid;
	pointcloud::meshGrid<float>(pointcloud_meshgrid, 0, 3, 0, 3, 1);

	unsigned int* ptr;
	pointcloud_meshgrid.getTrianglesPtr(&ptr);
	for (size_t i = 0; i < pointcloud_meshgrid.getNumberOfTriangles(); i++) {
		std::cout << ptr[i * 3 + 0] << " " << ptr[i * 3 + 1] << " " << ptr[i * 3 + 2] << std::endl;
	}

	size_t* ptr1 = pointcloud_meshgrid.getTrianglesPtr();
	for (size_t i = 0; i < pointcloud_meshgrid.getNumberOfTriangles(); i++) {
		std::cout << ptr1[i * 3 + 0] << " " << ptr1[i * 3 + 1] << " " << ptr1[i * 3 + 2] << std::endl;
	}
	io::writePly("C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/result.ply", pointcloud_meshgrid);

	int versuch = 2048;
	int shift =  versuch / 3;
			
	float* array_x(new float[versuch]);
	float* array_y1(new float[versuch]);
	float* array_y2(new float[versuch]);
	float* array_y3(new float[versuch]);
	for (int i = -shift; i < versuch - shift; i++) {
		array_x[i + shift] = (float) i;
		array_y1[i + shift] = (float) i;
		array_y2[i + shift] = (float) i / 2;
		array_y3[i + shift] = (float) i*3 / 4;
	}

	pointcloud::Quaterion<float> quat1;
	pointcloud::Quaterion<float> quat2(1.2f, 0.12f, 0.45f);

	utils::GLView<float> glview;

	glview.setViewer();
	//glview.setPointcloud(pointcloud_buny.getPointsPtr(), pointcloud_buny.getNumberOfVertices());
	glview.setPointcloud(pointcloud_buny);
	glview.subPlot(2, 2, 0);

	glview.setPlot(versuch);
	glview.setY(array_y1);
	glview.setY(array_y2);
	glview.setY(array_y3);
	glview.setX(array_x);
	glview.subPlot(2, 2, 1);

	glview.setPlot(versuch);
	glview.setY(array_y1);
	glview.setY(array_y2);
	glview.setX(array_x);
	glview.subPlot(2, 2, 2);

	float* points = pointcloud_buny.getPointsPtr();
	glview.setPlot3D();
	//glview.setPointcloud(GLParams::POINTS, points, pointcloud_buny.getNumberOfVertices());
	glview.setPointcloud(GLParams::POINTS, pointcloud_meshgrid);
	glview.subPlot(2, 2, 3);
	delete[] points;



	glview.mainLoop();

	return(0);
}