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

#include "pcsimp/pcsimp.hpp"

#include "tools/gl.h"
#include "tools/io.h"
#include "tools/math.h"
#include "tools/pointcloud.h"
#include "tools/parameters.h"
#include "tools/utils.h"

#include "trees/trees.hpp"

int main(int argc, char* argv[]) {

	std::cout << "----------------------- Main -----------------------" << std::endl;
	
	/**
		Parameter
	*/
	utils::Timer time;
	int cores = (unsigned int)std::thread::hardware_concurrency();
	size_t neighbors = 30;
	
	/**
		Read pointcloud
	*/
	pointcloud::PointcloudSoA<float> pointcloud;

	char* file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Ettlingen/Ettlingen1.ply";
	//char* file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Sonstiges/buny.ply";
	//char* file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Sonstiges/mesh.ply";
	//char *file = "C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/Unikirche/UnikircheII.ply";

	time.start();
	if (io::readPly(file, pointcloud)) {
		std::cout << "File with " << pointcloud.getNumberOfVertices() << " point has been read in "
			<< time.stop() << " s into Pointcloud" << std::endl;
	}
	std::cout << pointcloud << std::endl;

	/**
		----------------------- Computation of the normals -----------------------
	*/
	time.start();
	
		pointcloud::NormalParams normal_params;
			normal_params.setCores(cores);
			normal_params.setNormalComputation(NormalComputation::PLANESVD);
			normal_params.setWeightFunction(WeightFunction::LINEAR);
		pointcloud::computeNormals<float>(pointcloud, neighbors, normal_params);

	std::cout << "Computation of Normals in " << time.stop() << " s" << std::endl;
		
	/**
		----------------------- Computation of the surfaces -----------------------
	*/

		/**
			Build kdtree
		*/
		utils::Matrix<float> pointcloud_matrix;
		pointcloud.getMatrix(pointcloud_matrix);
		trees::Index<float> kdtree_index(pointcloud_matrix, trees::KDTreeIndexParams(neighbors));
		kdtree_index.buildIndex();

		utils::randSeed();
		do{
			/**
				Search for the neighbors of a specific point
			*/
			size_t random_point = utils::randInt(pointcloud.getNumberOfVertices(),0);
			utils::Matrix<float> point(pointcloud.getAllocatedPointPtr(random_point), 3, 1);
			utils::Matrix<float> normal(pointcloud.getAllocatedNormalPtr(random_point), 3, 1);

			trees::TreeParams tree_params;
			tree_params.setCores(normal_params.getCores());

			utils::Matrix<size_t> indices(1, neighbors);
			utils::Matrix<float> dists(1, neighbors);

			kdtree_index.knnSearch(point.transpose(), indices, dists, neighbors, tree_params);

			/**
				Get the neighbors of the reference point
			*/
			pointcloud::PointcloudAoS<float> pointcloud_points;
			pointcloud.getSubset(indices.getPtr(), neighbors, pointcloud_points);
			utils::Matrix<float> points;
			pointcloud_points.getMatrix(points);

			/**
				Compute the distances and the variance of these distances
			*/
			pointcloud::SurfaceParams surface_params;
			size_t polynomial_degree = 3;
			surface_params.setAccuracy(1.0f / 1000.0f);
			surface_params.setRootsApproximation(RootsApproximation::QUAD);
			surface_params.setSurfaceComputation(SurfaceComputation::SURFPOLYMLS);
			surface_params.setPolynomialDegree(polynomial_degree);

			utils::Matrix<double> parameter = pointcloud::computeSurface<double>(
												point.getType<double>(), 
												points.getType<double>(),
												normal.getType<double>(),
												surface_params);
			
			utils::Matrix<double> point_ref = pointcloud::pointMLS<double>(
				point.getType<double>(),
				points.getType<double>(),
				normal.getType<double>(),
				surface_params);

			pointcloud_points.setPointPtr( point_ref.getTypePtr<float>(), 0);
			pointcloud_points.setColorPtr({ 255,0,0 }, 0);

			/**
				Create a grid which shall be fitted to the pointcloud
			*/
			utils::Matrix<float> points_mesh;
			utils::Matrix<unsigned int> lines_mesh;
			gl::glMeshGrid<float>(points, points_mesh, lines_mesh, 10);

			math::Polynomial3D<float> polynomial(parameter.getType<float>(), polynomial_degree);
			polynomial(points_mesh);

			math::Polynomial3D<float> polynom(parameter.getType<float>(), 3);
			math::Polynomial3DIntersection<float> polynom_intersection(polynom, normal, point);

			utils::BoundingBox<float> bounding_box(points);

			float min = /*std::sqrt(*/((point - bounding_box.getMinMatrix()).transpose() * normal).getValue();
			float max = /*std::sqrt(*/((point - bounding_box.getMaxMatrix()).transpose() * normal).getValue();

			if (min > max) {
				swap(min, max);
			}

			size_t number_of_elements = 1000;
			utils::Matrix<float> intersection = math::getFunctionMatrix<float, math::Polynomial3DIntersection<float>>(
				polynom_intersection,
				min,
				max,
				number_of_elements);

			float t = math::NewtonMethod<float, math::Polynomial3DIntersection<float>>(
				polynom_intersection, 
				min, 
				max, 
				std::abs(max - min) / 1000);

			std::cout << t << std::endl;
			if (t != NULL) {
				pointcloud_points.setPointPtr((normal*t+point).getPtr(), 1);
				pointcloud_points.setColorPtr({ 0,255,0 }, 1);
			}
			/**
				Show results
			*/
			gl::GLView<float> glview(argc,argv);

			glview.setViewer();
			glview.setPointcloud(pointcloud);
			glview.subPlot(2, 2, 0);

			glview.setPlot3D();
			glview.setPointcloud(GLParams::POINTS, pointcloud_points);
			glview.setPointcloud(GLParams::LINES, points_mesh, lines_mesh);
			glview.subPlot(2, 2, 1);

			glview.setPlot(number_of_elements);
			glview.setX(intersection.getAllocatedColPtr(0));
			glview.setY(intersection.getAllocatedColPtr(1));
			glview.subPlot(2, 2, 2);

			glview.mainLoop();
		} while (true);

	/**
		Write results
	*/
	io::writePly("C:/Users/Wolfgang Brandenburg/OneDrive/Dokumente/3DModelle/result.ply", pointcloud);

	return(0);
}