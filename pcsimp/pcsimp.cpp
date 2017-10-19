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

	utils::GLViewer<float> viewer;
	viewer.setViewer();
	viewer.setPointcloud(pointcloud);
	viewer.plot();
	viewer.mainLoop();

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




	//if (plyIO.getDataType() == 1) {
	//	/*program<float>(cores, plyIO);*/
	//}
	//else {
	//	/*program<double>(cores, plyIO);*/
	//}



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