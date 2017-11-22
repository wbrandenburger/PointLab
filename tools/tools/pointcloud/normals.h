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

#ifndef POINTCLOUD_NORMALS_H_
#define POINTCLOUD_NORMALS_H_

#include "tools/parameters.h"

#include "tools/utils/matrix.h"
#include "tools/pointcloud/pointcloud.h"

#include "tools/math/standard.h"

#include "trees/trees.hpp"

#include "eigen3/Eigen/Dense"

namespace pointcloud
{


	/**
		
		@param[in,out] pointcloud Pointcloud
		@param[in] neighbors Number of neighbors which will be considered for computation normals
		@param[in] normal_computation Method which will be used for computing normals
	*/
	template<typename ElementType> void computeNormals(pointcloud::Pointcloud<ElementType>& pointcloud, 
		size_t  neighbors,
		NormalComputation normal_computation = NormalComputation::PLANESVD)
	{
		/**
			Ensure assigning normals to pointcloud
		*/
		pointcloud.setNormals();

		/**
			Build a kd-tree with the entire pointcloud
		*/
		utils::Matrix<ElementType> pointcloud_matrix;
		pointcloud.getMatrix(pointcloud_matrix);
		trees::Index<ElementType> kdtree_index(pointcloud_matrix,trees::KDTreeIndexParams(std::round(neighbors / 2)));
		kdtree_index.buildIndex();

		/**
			Search for the neighbors 
		*/
		utils::Matrix<size_t> indices(pointcloud.getNumberOfVertices(), neighbors);
		utils::Matrix<ElementType> dists(pointcloud.getNumberOfVertices(), neighbors);
		
		int cores = (unsigned int)std::thread::hardware_concurrency();
		trees::TreeParams tree_parameter(cores);

		kdtree_index.knnSearch(pointcloud_matrix, indices, dists, neighbors, tree_parameter);

		/**
			Compute for every point in the pointcloud the corresponding normal
		*/
		utils::Matrix<ElementType> points;
		for (size_t i = 0; i < 1/*pointcloud.getNumberOfVertices()*/; i++) {
			pointcloud.getSubset(indices[i], neighbors, points);

			/**
				Compute the normals depending on input method
			*/
			ElementType normal[3];
			std::memset(normal, (ElementType)0, sizeof(ElementType) * 3);

			WeightFunctionGaussian<ElementType> a(points);

			switch (normal_computation) {
			case NormalComputation::PLANESVD: normalPlaneSVD(normal,points); break;
			case NormalComputation::PLANEPCA: break;
			case NormalComputation::VECTORSVD: break;
			case NormalComputation::QUADSVD: break;
			}
			pointcloud.setNormalPtr(normal, i);

			points.reset();
		}
	}
	
	template<typename ElementType> class WeightFunctionGaussian
	{
	public:
		/**
		Constructor
		*/
		WeightFunctionGaussian(const utils::Matrix<ElementType>& data)
		{
			math::var<ElementType>(data);
		}

	private:
		ElementType std_;
	};

	/**
		 A classical method is to fit a local plane S=n_{x}x+n_{y}y+n_{z}z+d 
		 to the points in the neighborhood

		 @param[in] normal
		 @param[in] points_matrix
	*/
	template<typename ElementType> void normalPlaneSVD(ElementType* normal, 
		utils::Matrix<ElementType> points)
	{
		/**
			Build a nx4 matrix A = [x y z 1] with n = number of points
		*/
		Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic> A(points.getRows(), 4);
		for (size_t i = 0; i < points.getRows(); i++) {
			A(i, 0) = points[i][0];
			A(i, 1) = points[i][1];
			A(i, 2) = points[i][2];
			A(i, 3) = (ElementType)1;
		}
		/**
			Build a nxn matrix with the weights
		*/
		utils::L2<ElementType> distance;
		Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic> P(points.getRows(), points.getRows());

		for (size_t i = 0; i < points.getRows(); i++) {
			P(i, i) = 1;
		}

		/**
			Compute the singular value decomposition of A
		*/
		Eigen::JacobiSVD<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> svd_eigen(
			A, Eigen::ComputeThinU | Eigen::ComputeThinV);

		/**
			The last vector of matrix v computed by the singular value decomposition corresponds to the
			smallest singular value
		*/
		Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic> v_eigen = svd_eigen.matrixV();
		normal[0] = v_eigen(0, 3);
		normal[1] = v_eigen(1, 3);
		normal[2] = v_eigen(2, 3);
	}
}

#endif /* POINTCLOUD_NORMALS_H_ */