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

//#define FLANN_USE_CUDA
//#include "flann/flann.h"
//#include "flann/flann.hpp"

#include "pcsimp.hpp"

#include "tools/utils.h"
#include "tools/io.h"
#include "tools/math.h"
#include "tools/pointcloud.h"

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
	//char *file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Unikirche/UnikircheII.ply";
		
	io::PlyIO plyIO;
	plyIO.initialze(file);
	
	/**
		Read data
	*/
	utils::Timer time;
	
	unsigned char flags = 0;
	if (plyIO.isPoints()) {
		flags |= pointcloud::POINTCLOUD_POINTS;
	}
	if (plyIO.isColor()) {
		flags |= pointcloud::POINTCLOUD_COLORS;
	}
	if (plyIO.isNormal()) {
		flags |= pointcloud::POINTCLOUD_NORMALS;
	}

	pointcloud::PointcloudSoA<float> pointcloud(plyIO.getInstances(),flags);
	
	time.start();
	if (plyIO.readPly(pointcloud)) {
		std::cout << "File with " << pointcloud.getRows() << " point has been read in "
			<< time.stop() << " s into Pointcloud" << std::endl;
	}
	std::cout << pointcloud << std::endl;

	//utils::GLViewer<float> viewer;
	//viewer.setViewer();
	//viewer.setPointcloud(pointcloud);
	//viewer.plot();
	//viewer.mainLoop();

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


	//double x_, y_, z_;
	//quat1.getEulerAngles(x_,y_,z_);
	//std::cout << x_ << " " << y_ << " " << z_ << std::endl;
	//double* matrixrot = new double[9];
	//quat.getRotationMatrix(matrixrot);

	/*
	std::cout << q1.lengthsqr() << std::endl;*/

	utils::GLView<float> glview;

	glview.setViewer();
	glview.setPointcloud(pointcloud);
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

	glview.mainLoop();
	//if (plyIO.getDataType() == 1) {
	//	/*program<float>(cores, plyIO);*/
	//}
	//else {
	//	/*program<double>(cores, plyIO);*/
	//}

	utils::Matrix<float> matrix;
	pointcloud.getMatrix(matrix);
	trees::Index<float> index(matrix, trees::KDTreeIndexParams(20));

	std::cout << "NOCH NICHT" << std::endl;

	//utils::Timer time;
	//
	//io::PlyIO plyIO;
	//plyIO.initialze(file);

	//pointcloud::PointcloudSoA<float> pointcloud(plyIO.getInstances());

	//time.start();
	//if (plyIO.readPly(pointcloud)) {
	//	std::cout << "File with " << pointcloud.getRows() << " point has been read in "
	//		<< time.stop() << " s into Pointcloud" << std::endl;
	//}

	//pcsimp::Matrix<float> pointcloudsimp(pointcloud.getPointsPtr(), pointcloud.getRows(), 3);
	//
	///**
	//	Build index
	//*/
	//time.start();
	//pcsimp::Index<float> index(pointcloudsimp, pcsimp::MLSIndexParams(30,0.05f));

	//std::cout << "Simplification-index has been built in " << time.stop() << " s" << std::endl;

	///**
	//Destroy the structures
	//*/
	//pointcloud.clear();
	//pointcloudsimp.clear();


	return(0);
}