/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017  Wolfgang Brandenburger (w.brandenburger@unibw.de). All rights reserved.
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

#include "trees.hpp"

#include "tools/utils.h"
#include "tools/io.h"
////#include "tools/math.h"
#include "tools/pointcloud.h"
////
////#include <opencv2/highgui.hpp>

//template<typename ElementType> class GLPlot;

//template<typename ElementType> std::vector<utils::GLPlot<ElementType>::Plot*> utils::GLPlot<ElementType>::plots;
//
//template<typename ElementType> size_t utils::GLPlot<ElementType>::number_of_plots;



////typedef unsigned char uchar;
////
////template<typename ElementType> void program(size_t cores_, io::PlyIO& plyIO_)
////{
////	/**
////		Read data
////	*/
////	utils::Timer time;
////
////	pointcloud::PointcloudAoS<ElementType> pointcloud(plyIO_.getInstances(), 3);
////
////	time.start();
////	if (plyIO_.readPly(pointcloud)) {
////		std::cout << "File with " << pointcloud.rows << " point has been read in "
////			<< time.stop() << " s into Pointcloud" << std::endl;
////	}
////
////	std::cout << pointcloud << std::endl;
////
////	utils::Matrix<ElementType> matrix;
////	pointcloud.getMatrix(matrix);
////
////	std::cout << matrix << std::endl;
////
////	/**
////		Build index
////	*/
////
////	trees::Index<ElementType> index(matrix, trees::KDTreeIndexParams(20));
////
////	time.start();
////	index.buildIndex();
////	std::cout << "KDTree has been built in " << time.stop() << " s" << std::endl;
////
////	/**
////		Search in pointcloud knn search
////	*/
////	int nn = 20;
////	int querynumber = pointcloud.getRows();
////	std::cout << querynumber << " " << pointcloud.getRows() << std::endl;
////	utils::Matrix<size_t> indices(new size_t[querynumber*nn], querynumber, nn);
////	utils::Matrix<ElementType> dists(new ElementType[querynumber*nn], querynumber, nn);
////
////	trees::TreeParams params;
////	params.cores = cores_;
////
////	//time.start();
////	//index.knnSearch(matrix, indices, dists, nn, params);
////	//std::cout << "Search has been performed in " << time.stop() << " s" << std::endl;
////
////
////
////	utils::randSeed();
////	size_t rand_index = utils::rand<size_t>(pointcloud.getRows(), 0);
////	std::vector<size_t> rand_index_list = { rand_index };
////	utils::Matrix<ElementType> point_center(new ElementType[3], 1, 3);
////	pointcloud.getSubset(rand_index_list, point_center);
////
////	std::cout << rand_index << std::endl;
////
////	std::cout << point_center << std::endl;
////	
////	for (size_t i = 3; i < 100; i++) {
////		utils::Matrix<size_t> indices_center(new size_t[i], 1, i);
////		utils::Matrix<ElementType> dists_center(new ElementType[i], 1, i);
////
////		utils::Matrix<size_t> indices_neighbors(new size_t[i*i], i, i);
////		utils::Matrix<ElementType> dists_neighbors(new ElementType[i*i], i, i);
////
////		index.knnSearch(point_center, indices_center, dists_center, i, params);
////
////		std::vector<size_t> indices_neighbor_list(i);
////		for (size_t j = 0; j < i; j++) {
////			indices_neighbor_list[j] = indices_center[0][j];
////		}
////
////		utils::Matrix<ElementType> point_neighbors(new ElementType[3 * i], i, 3);
////		pointcloud.getSubset(indices_neighbor_list, point_neighbors);
////
////		index.knnSearch(point_neighbors, indices_neighbors, dists_neighbors, i, params);
////
////
////		indices_center.clear();
////		dists_center.clear();
////		indices_neighbors.clear();
////		dists_neighbors.clear();
////
////		point_neighbors.clear();
////	}
////	point_center.clear();
////
////	int versuch = 2048;
////	int shift = versuch / 3;
////	
////	//std::vector<ElementType> array_x(versuch);
////	//std::vector<ElementType> array_y1(versuch);
////	//std::vector<ElementType> array_y2(versuch);
////	//for (int i = -shift; i < versuch - shift; i++) {
////	//	array_x[i + shift] = i;
////	//	array_y1[i + shift] = i;
////	//	array_y2[i + shift] = i / 2;
////	//	array_y3[i + shift] = std::pow(i,1.025);
////	//	array_y4[i + shift] = std::pow(i/100, 2);
////	//}
////	//utils::PlotVector<ElementType> plot1("Versuch",1);
////	//plot1.setFunction(array_y1);
////	//plot1.setFunction(array_y2);
////	//plot1.setIntervall(array_x);
////	//plot1.plot();
////
////	Eigen::Matrix<ElementType, Eigen::Dynamic, 1> eigen_x(versuch, 1);
////	Eigen::Matrix<ElementType, Eigen::Dynamic, 1> eigen_y1(versuch, 1);
////	Eigen::Matrix<ElementType, Eigen::Dynamic, 1> eigen_y2(versuch, 1);
////	for (int i = -shift; i < versuch - shift; i++) {
////		eigen_x[i + shift] = i;
////		eigen_y1[i + shift] = i;
////		eigen_y2[i + shift] = i / 2;
////	}
////	utils::PlotEigen<ElementType> plot1("Versuch 1",1);
////	plot1.setFunction(eigen_y1);
////	plot1.setFunction(eigen_y2);
////	plot1.setIntervall(eigen_x);
////	plot1.plot();	
////	
////	utils::Matrix<ElementType> mat_x(new ElementType[versuch * 1], versuch, 1);
////	utils::Matrix<ElementType> mat_y1(new ElementType[versuch * 1], versuch, 1);
////	utils::Matrix<ElementType> mat_y2(new ElementType[versuch * 1], versuch, 1);
////	for (int i = -shift; i < versuch - shift; i++) {
////		mat_x[i + shift][0] = i;
////		mat_y1[i + shift][0] = i;
////		mat_y2[i + shift][0] = i / 2;
////	}
////	utils::PlotMat<ElementType> plot2("Versuch 2",1);
////	plot2.setFunction(&mat_y1);
////	plot2.setFunction(&mat_y2);
////	plot2.setIntervall(&mat_x);
////	plot2.plot();	
////	cvWaitKey(0);
////
////
////
////
////	///**
////	//	Search in pointcloud radius search
////	//*/
////	//float radius = 0.005;
////	//int querynumber = 12;// pointcloudkdtree.rows;
////
////	//std::vector<std::vector<int>> indices(querynumber);
////	//std::vector<std::vector<float>> dists(querynumber);
////
////	///**
////	//	Generates a pointcloud with points whose neighbors shall searched for
////	//*/
////	//utils::randSeed();
////	//trees::Matrix<float> query(new float[querynumber*pointcloudkdtree.cols], querynumber, pointcloudkdtree.cols);
////	//for (int i = 0; i < querynumber; i++) {
////	//	int random = utils::rand<int>(pointcloudkdtree.rows - 1, 0);
////	//	for (int j = 0; j < pointcloudkdtree.cols; j++) {
////	//		query[i][j] = pointcloudkdtree[random][j];
////	//	}
////	//}
////
////	//trees::TreeParams params;
////	//params.cores = cores;
////
////	//time.start();
////	//index.radiusSearch(query, indices, dists, radius, params);
////	//std::cout << "Search has been performed in " << time.stop() << " s" << std::endl;
////	
////	/**
////		Colorize the pointcloud
////	*/
////	utils::randSeed();
////	for (size_t i = 0; i < indices.getRows(); i++) {
////		uchar  r = utils::rand<uchar >(255, 0);
////		uchar  g = utils::rand<uchar >(255, 0);
////		uchar  b = utils::rand<uchar >(255, 0);
////		for (size_t j = 0; j < indices.getCols(); j++) {
////			pointcloud.setColor(r, indices[i][j], 0);
////			pointcloud.setColor(g, indices[i][j], 1);
////			pointcloud.setColor(b, indices[i][j], 2);
////		}
////	}
////
////	plyIO_.writePly("C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/result.ply", pointcloud);
////
////	index.freeIndex();
////	indices.clear();
////	dists.clear();
////	//time.start();
////	//size_t i = 0;
////	//for (size_t counter = 0; counter < 10000000000; counter++) {
////	//	i++;
////	//}
////	//std::cout << "Counter " << time.stop() << " s" << std::endl;
////
////	//time.start();
////	//boost::atomic<int> ia(0);
////	//for (size_t counter = 0; counter < 10000000000; counter++) {
////	//	ia.fetch_add(1);
////	//}
////	//std::cout << "Counter" << time.stop() << " s" << std::endl;
////}

int main(int argc, char* argv[]) {

	std::cout << "----------------------- Main -----------------------" << std::endl;

	size_t i = 0;
	size_t cores = (unsigned int)std::thread::hardware_concurrency();
	while (i < argc) {
		if (!strcmp(argv[i], "--cores")) {
			i++;
			cores = std::stoi(argv[i]);
		}
		i++;
	}

	char* file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Ettlingen/Ettlingen1.ply";
	//char *file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Unikirche/UnikircheII.ply";
	
	io::PlyIO plyIO;
	plyIO.initialze(file);
	
	if (plyIO.getDataType() == 1) {
		/*program<float>(cores, plyIO);*/
	}
	else {
		/*program<double>(cores, plyIO);*/
	}

	/**
		Read data
	*/
	utils::Timer time;
	
	pointcloud::PointcloudSoA<float> pointcloud(plyIO.getInstances(), 3);
	
	time.start();
	if (plyIO.readPly(pointcloud)) {
		std::cout << "File with " << pointcloud.rows << " point has been read in "
			<< time.stop() << " s into Pointcloud" << std::endl;
	}
	std::cout << pointcloud << std::endl;


	utils::GLViewer<float> viewer;
	//viewer.setPointcloud(pointcloud);
	//viewer.plot();
	//viewer.mainLoop();


		//int versuch = 2048;
		//int shift =  versuch / 3;
		//
		//std::vector<float> array_x(versuch);
		//std::vector<float> array_y1(versuch);
		//std::vector<float> array_y2(versuch);
		//std::vector<float> array_y3(versuch);
		//for (int i = -shift; i < versuch - shift; i++) {
		//	array_x[i + shift] = i;
		//	array_y1[i + shift] = i;
		//	array_y2[i + shift] = i / 2;
		//	array_y3[i + shift] = i*3 / 4;
		//}
		//utils::GLPlotVector<float> plot;
		//plot.setPlot();
		//plot.setY(array_y1);
		//plot.setY(array_y2);
		//plot.setY(array_y3);
		//plot.setX(array_x);
		//plot.plot();

		//plot.setPlot();
		//plot.setY(array_y1);
		//plot.setY(array_y2);
		//plot.setX(array_x);
		//plot.plot();
		//
		//plot.setPlot();
		//plot.setY(array_y1);
		//plot.setY(array_y2);
		//plot.setX(array_x);
		//plot.plot();

		//plot.mainLoop();

	return(0);
}

