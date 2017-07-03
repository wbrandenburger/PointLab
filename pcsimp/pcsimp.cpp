#include <iostream>
#include <string>
#include <thread>

#include <map>

//#define FLANN_USE_CUDA
//#include "flann/flann.h"
//#include "flann/flann.hpp"

#include "pcsimp.hpp"
#include "util/any.h"

#include "tools/utils.h"
#include "tools/io.h"

typedef std::map<std::string, pcsimp::any> IndexParams;

template<typename T>
T get_params(const IndexParams& params, std::string name, const T& default_value)
{
	IndexParams::const_iterator it = params.find(name);
	if (it != params.end()) {
		return it->second.cast<T>();
	}
	else {
		return default_value;
	}
}

int main(int argc, char* argv[]) {

	IndexParams versuch;
	versuch["Was"] = 15;
	std::cout << get_params(versuch, "Was", 1) << std::endl;

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

	///**
	//Destroy the structures
	//*/
	//pointcloud.clear();
	//delete[] indices.ptr();
	//delete[] dists.ptr();


	return(0);
}