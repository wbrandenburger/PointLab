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

#include "tools/utils/heap.h"

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

	char *file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Sonstiges/plane.ply";
	//char *file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Unikirche/UnikircheII.ply";

	utils::Timer time;
	utils::Pointcloud<float> pointcloud;

	time.start();
	if (io::readply<float>(file, pointcloud)) {
		std::cout << "File with " << pointcloud.points.rows << " point has been read in "
			<< time.stop() << " s into Pointcloud" << std::endl;
	}

	trees::Matrix<float> pointcloudkdtree(pointcloud.getPointsPtr(), pointcloud.rows, pointcloud.cols);
	
	/**
		Build index
	*/
	
	trees::Index<float> index(pointcloudkdtree, trees::KDTreeIndexParams(1));

	time.start();
	index.buildIndex();
	std::cout << "KDTree has been built in " << time.stop() << " s" << std::endl;

	/////**
	////	Search in pointcloud
	////*/
	////int nn = 2000;
	////int querynumber = 12;// pointcloudkdtree.rows;

	////trees::Matrix<size_t> indices(new size_t[querynumber*nn], querynumber, nn);
	////trees::Matrix<float> dists(new float[querynumber*nn], querynumber, nn);

	/////**
	////	Generates a pointcloud with points whose neighbors shall searched for
	////*/
	////utils::randSeed();
	////trees::Matrix<float> query(new float[querynumber*pointcloudkdtree.cols], querynumber, pointcloudkdtree.cols);
	////for (int i = 0; i < querynumber; i++) {
	////	int random = utils::rand<int>(pointcloudkdtree.rows - 1, 0);
	////	for (int j = 0; j < pointcloudkdtree.cols; j++) {
	////		query[i][j] = pointcloudkdtree[random][j];
	////	}
	////}

	////trees::TreeParams params;
	////params.cores = cores;

	////time.start();
	////index.knnSearch(query, indices, dists, nn, params);
	////std::cout << "Search has been performed in " << time.stop() << " s" << std::endl;
	////
	/////**
	////	Colorize the pointcloud
	////*/
	////utils::randSeed();
	////for (int i = 0; i < indices.rows; i++) {
	////	int r = utils::rand<int>(255, 0);
	////	int g = utils::rand<int>(255, 0);
	////	int b = utils::rand<int>(255, 0);
	////	for (int j = 0; j < indices.cols; j++) {
	////		pointcloud.colors[indices[i][j]][0] = r;
	////		pointcloud.colors[indices[i][j]][1] = g;
	////		pointcloud.colors[indices[i][j]][2] = b;
	////	}
	////}

	////io::writeply("C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/result.ply", pointcloud);

	/**
		Search in pointcloud
	*/
	float radius = 0.005;
	int querynumber = 12;// pointcloudkdtree.rows;

	std::vector<std::vector<int>> indices(querynumber);
	std::vector<std::vector<float>> dists(querynumber);

	/**
		Generates a pointcloud with points whose neighbors shall searched for
	*/
	utils::randSeed();
	trees::Matrix<float> query(new float[querynumber*pointcloudkdtree.cols], querynumber, pointcloudkdtree.cols);
	for (int i = 0; i < querynumber; i++) {
		int random = utils::rand<int>(pointcloudkdtree.rows - 1, 0);
		for (int j = 0; j < pointcloudkdtree.cols; j++) {
			query[i][j] = pointcloudkdtree[random][j];
		}
	}

	trees::TreeParams params;
	params.cores = cores;

	time.start();
	index.radiusSearch(query, indices, dists, radius, params);
	std::cout << "Search has been performed in " << time.stop() << " s" << std::endl;

	/**
		Colorize the pointcloud
	*/
	utils::randSeed();
	for (int i = 0; i < indices.size(); i++) {
		int r = utils::rand<int>(255, 0);
		int g = utils::rand<int>(255, 0);
		int b = utils::rand<int>(255, 0);
		for (int j = 0; j < indices[i].size(); j++) {
			pointcloud.colors[indices[i][j]][0] = r;
			pointcloud.colors[indices[i][j]][1] = g;
			pointcloud.colors[indices[i][j]][2] = b;
		}
	}

	io::writeply("C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/result.ply", pointcloud);

	/**
		Destroy the structures
	*/
	pointcloud.clear();


	int* heaplist = new int[15];

	heaplist[0] = 35;
	heaplist[1] = 99;
	heaplist[2] = 12;
	heaplist[3] = 56;
	heaplist[4] = 85;
	heaplist[5] = 23;
	heaplist[6] = 96;
	heaplist[7] = 43;
	heaplist[8] = 75;
	heaplist[9] = 24;
	heaplist[10] = 59;
	heaplist[11] = 83;
	heaplist[12] = 37;
	heaplist[13] = 72;
	heaplist[14] = 4;
	
	utils::Heap<int, true> heap(15);
	
	for (int i = 0; i < 15; i++) {
		heap.push(heaplist[i]);
	}


	std::cout << heap << std::endl;

	return(0);
}