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

	utils::Timer time;
	
	io::PlyIO plyIO;
	plyIO.initialze(file);

	pointcloud::PointcloudSoA<float> pointcloud(plyIO.getInstances(), 3);

	time.start();
	if (plyIO.readPly(pointcloud)) {
		std::cout << "File with " << pointcloud.rows << " point has been read in "
			<< time.stop() << " s into Pointcloud" << std::endl;
	}

	pcsimp::Matrix<float> pointcloudsimp(pointcloud.getPointsPtr(), pointcloud.rows, pointcloud.cols);
	
	/**
		Build index
	*/
	time.start();
	pcsimp::Index<float> index(pointcloudsimp, pcsimp::MLSIndexParams(30,0.05f));

	std::cout << "Simplification-index has been built in " << time.stop() << " s" << std::endl;

	/**
	Destroy the structures
	*/
	pointcloud.clear();
	pointcloudsimp.clear();



	//typedef utils::HeapWrapperConcurrent<int> Heap;

	//size_t n = 4080;
	//size_t coresheap = 15;
	//utils::Threadpool pool(coresheap);
	//Heap heapConcurrent(n, coresheap, true);





	//utils::randSeed();
	//int value;
	//size_t index;

	//for (size_t i = 0; i < heapConcurrent.size * heapConcurrent.cores; i++) {
	//	while (!pool.runTask(boost::bind(&Heap::push, &heapConcurrent, utils::randInt(n, 0), i)));
	//}

	//for (size_t i = 0; i < 4032; i++) {
	//	while (!pool.runTask(boost::bind(&Heap::pop, &heapConcurrent, boost::ref(value), boost::ref(index))));
	//}
	//pool.waitTasks();

	//std::cout << heapConcurrent << std::endl;
	////time.start();
	////for (size_t i = 0; i < 100000; i++) {
	////	while (!pool.runTask(boost::bind(&Heap::update, &heapConcurrent, utils::randInt(n, 0),/* i % (heapConcurrent.size * heapConcurrent.cores)*/utils::randInt(n, 0))));
	////}
	////pool.waitTasks();
	////std::cout << time.stop() << " " << heapConcurrent.size << " " << heapConcurrent.count << " " << std::endl;

	////std::cout << heapConcurrent << std::endl;

	//if (heapConcurrent.checkHeap()) {
	//	std::cout << "Heapbedingung erfüllt" << std::endl;
	//}

	//heapConcurrent.clear();

	//pool.shutdown();

	////heapConcurrent.clear();

	//////std::cout << heapConcurrent.checkLock() << std::endl;

	//////////////time.start();


	//////////////for (size_t i = 0; i < heapConcurrent.size; i++) {
	//////////////	while (!pool.runTask(boost::bind(&Heap::update, &heapConcurrent, utils::randInt(n, 0), utils::randInt(n, 0))));
	//////////////}
	//////////////pool.waitTasks();

	//////////////std::cout << time.stop() << " " << heapConcurrent.size << " " << heapConcurrent.count << " ";

	//////////////if (heapConcurrent.checkHeap()) {
	//////////////	std::cout << " Heapbedingung erfüllt" << std::endl;
	//////////////}

	//////////////std::cout << time.stop() << " " << heapConcurrent.size << " " << heapConcurrent.count << " ";

	//////////////if (heapConcurrent.checkHeap()) {
	//////////////	std::cout << "Heapbedingung erfüllt" << std::endl;
	//////////////}

	//



	return(0);
}