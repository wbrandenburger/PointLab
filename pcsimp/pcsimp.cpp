#include <iostream>
#include <string>
#include <thread>

//#define FLANN_USE_CUDA
//#include "flann/flann.h"
//#include "flann/flann.hpp"

#include "pcsimp.hpp"

#include "tools/utils.h"
#include "tools/io.h"

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
	utils::Pointcloud<float> pointcloud;

	time.start();
	if (io::readply<float>(file, pointcloud)) {
		std::cout << "File with " << pointcloud.points.rows << " point has been read in "
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



	return(0);
}