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

#include "tools/utils.h"
//#include "tools/io.h"
//#include "tools/pointcloud.h"


int main(int argc, char* argv[]) {

	//std::cout << "----------------------- Main -----------------------" << std::endl;

	//int i = 0;
	//int cores = (unsigned int)std::thread::hardware_concurrency();
	//while (i < argc) {
	//	if (!strcmp(argv[i], "--cores")) {
	//		i++;
	//		cores = std::stoi(argv[i]);
	//	}
	//	i++;
	//}

	//char *file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Sonstiges/plane.ply";

	//utils::Timer time;

	//io::PlyIO plyIO;
	//plyIO.initialze(file);

	//pointcloud::PointcloudSoA<float> pointcloud(plyIO.getInstances(), 3);

	//time.start();
	//if (plyIO.readPly(pointcloud)) {
	//	std::cout << "File with " << pointcloud.rows << " point has been read in "
	//		<< time.stop() << " s into Pointcloud" << std::endl;
	//}

	//flann::Matrix<float> pointcloudflann(pointcloud.getPointsPtr(), pointcloud.rows, pointcloud.cols);
	//
	///**
	//	Allocate indices- and dists-matrices
	//*/
	//int nn = 20;
	//int querynumber = pointcloudflann.rows;

	//flann::Matrix<size_t> indices(new size_t[querynumber*nn], querynumber, nn);
	//flann::Matrix<float> dists(new float[querynumber*nn], querynumber, nn);

	///**
	//	Build index and perform knn-search with KDTREEIndex
	//*/
	//time.start();
	//flann::Index<flann::L2_3D<float>> indexCPU(pointcloudflann, flann::KDTreeSingleIndexParams(20));
	//indexCPU.buildIndex();

	//std::cout << "kd-tree has been built in " << time.stop() << " s" << std::endl;

	//flann::SearchParams params;
	//params.checks = FLANN_CHECKS_UNLIMITED;
	//params.cores = cores;

	//time.start();
	//indexCPU.knnSearch(pointcloudflann, indices, dists, nn, params);
	//std::cout << "search has been performed in " << time.stop() << " s" << std::endl;

	///**
	//	Build index and perform knn-search with KDTREECUDAIndex
	//*/
	//time.start();
	//flann::Index<flann::L2_3D<float>> indexCUDA(pointcloudflann, flann::KDTreeCudaIndexParams(20));
	//indexCUDA.buildIndex();

	//std::cout << "kd-tree has been built in " << time.stop() << " s" << std::endl;

	//time.start();
	//indexCUDA.knnSearch(pointcloudflann, indices, dists, nn, params);
	//	//indexCUDA.radiusSearch(query, indices, dists, 0.0003, params);
	//std::cout << "search has been performed in " << time.stop() << " s" << std::endl;

	///**
	//	Destroy the structures
	//*/
	//pointcloud.clear();
	//delete[] pointcloudflann.ptr();
	//delete[] indices.ptr();
	//delete[] dists.ptr();

	//return(0);
}