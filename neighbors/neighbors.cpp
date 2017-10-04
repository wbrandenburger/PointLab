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