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

	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);

	utils::GLView<float> glview;

	glview.setViewer();
	glview.setPointcloud(pointcloud);
	glview.plot();

	glview.setPlot(versuch);
	glview.setY(array_y1);
	glview.setY(array_y2);
	glview.setY(array_y3);
	glview.setX(array_x);
	glview.plot();

	glview.setPlot(versuch);
	glview.setY(array_y1);
	glview.setY(array_y2);
	glview.setX(array_x);
	glview.plot();

	glview.subPlot(3, 3, 8);
	glview.subPlot(4, 2, 3);
	glview.subPlot(6, 1, 3);

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