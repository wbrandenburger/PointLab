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
#include "math/zero.h"


#include "tools/utils.h"
#include "tools/io.h"
#include "tools/pointcloud.h"

io::PlyIO plyIO;

struct data {

	double operator() (double x)
	{
		return std::pow(x, 2) - 2;
	}

};

template<typename ElementType> void program(io::PlyIO& plyIO_)
{
	utils::Timer time;

	pointcloud::PointcloudAoS<ElementType> pointcloud(plyIO_.getInstances(), 3);

	time.start();
	if (plyIO.readPly(pointcloud)) {
		std::cout << "File with " << pointcloud.rows << " point has been read in "
			<< time.stop() << " s into Pointcloud" << std::endl;
	}


	trees::Matrix<ElementType> pointcloudkdtree(pointcloud.getPointsPtr(), pointcloud.rows, pointcloud.cols);
	pointcloudkdtree.clear();

	std::cout << pointcloud << std::endl;

	
	pointcloud::PointcloudSoA<ElementType> pointcloudcopy(pointcloud);
	std::cout << pointcloudcopy << std::endl;
	
	std::vector<size_t> list = { 0,1,2,3,4};
	pointcloud::PointcloudSoA<ElementType> subsetSoA;
	pointcloudcopy.getSubset(list,subsetSoA);
	pointcloud::PointcloudAoS<ElementType> subsetAoS;
	pointcloud.getSubset(list, subsetAoS);
	
	std::cout << subsetSoA << std::endl;

	std::cout << subsetAoS << std::endl;

	pointcloud::computeMean(pointcloud);

	//subset.clear();

	time.start();
	if (plyIO.writePly("C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/result.ply", pointcloudcopy)) {
		std::cout << "File with " << pointcloudcopy.rows << " point has been written in "
			<< time.stop() << " s into Pointcloud" << std::endl;
	}

	pointcloud.clear();
	pointcloudcopy.clear();
}

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
	char *file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/result.ply";
	//char *file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Unikirche/UnikircheII.ply";

	utils::Timer time;

	plyIO.initialze(file);
	
	if (plyIO.getDataType() == 1) {
		program<float>(plyIO);
	}
	else {
		program<double>(plyIO);
	}
	
	
	///////////////////**
	//////////////////	Build index
	//////////////////*/
	//////////////////
	//////////////////trees::Index<float> index(pointcloudkdtree, trees::KDTreeIndexParams(20));

	//////////////////time.start();
	//////////////////index.buildIndex();
	//////////////////std::cout << "KDTree has been built in " << time.stop() << " s" << std::endl;

	///////////////////**
	//////////////////	Search in pointcloud knn search
	//////////////////*/
	//////////////////int nn = 20;
	//////////////////int querynumber = pointcloudkdtree.rows;

	//////////////////trees::Matrix<size_t> indices(new size_t[querynumber*nn], querynumber, nn);
	//////////////////trees::Matrix<float> dists(new float[querynumber*nn], querynumber, nn);

	///////////////////**
	//////////////////	Generates a pointcloud with points whose neighbors shall searched for
	//////////////////*/
	//////////////////utils::randSeed();
	//////////////////trees::Matrix<float> query(new float[querynumber*pointcloudkdtree.cols], querynumber, pointcloudkdtree.cols);
	//////////////////for (int i = 0; i < querynumber; i++) {
	//////////////////	int random = utils::rand<int>(pointcloudkdtree.rows - 1, 0);
	//////////////////	for (int j = 0; j < pointcloudkdtree.cols; j++) {
	//////////////////		query[i][j] = pointcloudkdtree[random][j];
	//////////////////	}
	//////////////////}

	//////////////////trees::TreeParams params;
	//////////////////params.cores = cores;

	////////////////////for (size_t i = 0; i < 98000;/*pointcloudkdtree.rows;*/ i++) {
	////////////////////	index.remove(i);
	////////////////////}

	//////////////////time.start();
	//////////////////index.knnSearch(query, indices, dists, nn, params);
	//////////////////std::cout << "Search has been performed in " << time.stop() << " s" << std::endl;

	//////////////////index.freeIndex();

	///////////////////**
	//////////////////	Colorize the pointcloud
	//////////////////*/
	//////////////////utils::randSeed();
	//////////////////for (int i = 0; i < indices.rows; i++) {
	//////////////////	int r = utils::rand<int>(255, 0);
	//////////////////	int g = utils::rand<int>(255, 0);
	//////////////////	int b = utils::rand<int>(255, 0);
	//////////////////	for (int j = 0; j < indices.cols; j++) {
	//////////////////		pointcloud.colors[indices[i][j]][0] = r;
	//////////////////		pointcloud.colors[indices[i][j]][1] = g;
	//////////////////		pointcloud.colors[indices[i][j]][2] = b;
	//////////////////	}
	//////////////////}

	//////////////////io::writeply("C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/result.ply", pointcloud);

	/////**
	////	Search in pointcloud radius search
	////*/
	////float radius = 0.005;
	////int querynumber = 12;// pointcloudkdtree.rows;

	////std::vector<std::vector<int>> indices(querynumber);
	////std::vector<std::vector<float>> dists(querynumber);

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
	////index.radiusSearch(query, indices, dists, radius, params);
	////std::cout << "Search has been performed in " << time.stop() << " s" << std::endl;

	/////**
	////	Colorize the pointcloud
	////*/
	////utils::randSeed();
	////for (int i = 0; i < indices.size(); i++) {
	////	int r = utils::rand<int>(255, 0);
	////	int g = utils::rand<int>(255, 0);
	////	int b = utils::rand<int>(255, 0);
	////	for (int j = 0; j < indices[i].size(); j++) {
	////		pointcloud.colors[indices[i][j]][0] = r;
	////		pointcloud.colors[indices[i][j]][1] = g;
	////		pointcloud.colors[indices[i][j]][2] = b;
	////	}
	////}

	////io::writeply("C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/result.ply", pointcloud);

	/**
		Destroy the structures
	*/







	typedef utils::HeapConcurrent<int> Heap;

	size_t n = 28;
	size_t coresheap = 4;
	utils::Threadpool pool(coresheap);
	Heap heapConcurrent(n, coresheap, true);

	

	time.start();

	utils::randSeed();
	for (size_t i = 0; i < heapConcurrent.size * heapConcurrent.cores; i++) {
		while (!pool.runTask(boost::bind(&Heap::push, &heapConcurrent, utils::randInt(n, 0), i)));
	}
	pool.waitTasks();
	pool.shutdown();

	//utils::randSeed();
	//for (size_t i = 0; i <  heapConcurrent.size * heapConcurrent.cores; i++) {
	//	heapConcurrent.push(utils::randInt(n, 0));
	//}

	std::cout << time.stop() << " " << heapConcurrent.size << " " << heapConcurrent.count << " " << std::endl;

	if (heapConcurrent.checkHeap()) {
		std::cout << "Heapbedingung erfüllt" << std::endl;
	}

	std::cout << heapConcurrent << std::endl;

	////std::cout << heapConcurrent.checkLock() << std::endl;

	////////////time.start();


	////////////for (size_t i = 0; i < heapConcurrent.size; i++) {
	////////////	while (!pool.runTask(boost::bind(&Heap::update, &heapConcurrent, utils::randInt(n, 0), utils::randInt(n, 0))));
	////////////}
	////////////pool.waitTasks();

	////////////std::cout << time.stop() << " " << heapConcurrent.size << " " << heapConcurrent.count << " ";

	////////////if (heapConcurrent.checkHeap()) {
	////////////	std::cout << " Heapbedingung erfüllt" << std::endl;
	////////////}

	////////////std::cout << time.stop() << " " << heapConcurrent.size << " " << heapConcurrent.count << " ";

	////////////if (heapConcurrent.checkHeap()) {
	////////////	std::cout << "Heapbedingung erfüllt" << std::endl;
	////////////}

	//



	return(0);
}

