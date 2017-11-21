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

#ifndef UTILS_PCA_H_
#define UTILS_PCA_H_

#include "tools/parameters.h"

#include "tools/utils/matrix.h"
#include "tools/pointcloud/pointcloud.h"

#include "trees/trees.hpp"

#include "eigen3/Eigen/Dense"

namespace utils
{
	/**
		
		@param[in,out] pointcloud Pointcloud
		@param[in] neighbors Number of neighbors which will be considered for computation normals
		@param[in] normal_computation Method which will be used for computing normals
	*/
	template<typename ElementType> void computeNormals(pointcloud::Pointcloud<ElementType> pointcloud, 
		size_t  neighbors,
		NormalComputation normal_computation = NormalComputation::PLANEPCA)
	{
		/**
			Ensure assigning normals to pointcloud
		*/
		pointcloud.setNormals();

		/**
			Build a kd-tree with the entire pointcloud
		*/
		////trees::Index<ElementType> kdtree_index(/*Pointcloud*/,trees::KDTreeIndexParams(/*Number of points in a node*/));
		////kdtree_index.buildIndex();
		/**
			Compute for every point in the pointcloud the corresponding normal
		*/
		for (size_t i = 0; i < pointcloud.getNumberOfVertices(); i++) {

			/**
				Search for the neighbors for the given point
			*/

			//indices
			//dists

			/**
				Compute the normals depending on input method
			*/
			////switch (normal_comutation) {
			////case NormalComputation::PLANESVD: pointcloud.setNormalPtr(/*Computation of normal*/, i); break;
			////case NormalComputation::PLANEPCA: pointcloud.setNormalPtr(/*Computation of normal*/, i); break;
			////case NormalComputation::VECTORSVD: pointcloud.setNormalPtr(/*Computation of normal*/, i); break;
			////case NormalComputation::QUADSVD: pointcloud.setNormalPtr(/*Computation of normal*/, i); break;
			////}
		}
	}

	template<typename ElementType> void normalPlaneSVD()
	{

	}


	///**	
	//	Computes the principal components of abitrary points. Every vector in the resultmatrix corresponds to a principal component

	//	@param[in] data_ Matrix with the points
	//	@param[in] instance_ The point which determines the frame
	//	@param[in,out] principal_components_ The Matrix with the principal components
	//	@param[in] distance_ Distance function
	//	@param[in] smooth_factor_ Factor which adjusts the smoothing quality
	//*/
	//template<typename ElementType, typename DistanceType> void principalComponentAnalysis (const utils::Matrix<ElementType>& data_,
	//	ElementType* const instance_, Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>& principal_components_, 
	//	const DistanceType& distance_, const float smooth_factor_ = 0)
	//{
	//	Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic> kovariance_matrix(data_.cols, data_.cols);

	//	utils::Difference<ElementType> difference;
	//	ElementType* difference_vector(new ElementType[data_.getCols()]);

	//	for (size_t rows = 0; rows < data_.getCols(); rows++) {
	//		for (size_t cols = 0; cols < data_.getCols(); cols++) {
	//			kovariance_matrix(rows, cols) = 0;
	//			for (size_t i = 0; i < data_.rows; i++) {
	//				difference(data_[i], instance_, difference_vector, data_.getCols());s
	//				if (smooth_factor_) {
	//					kovariance_matrix(rows, cols) = kovariance_matrix(rows, cols) + difference_vector[rows] * difference_vector[cols] *
	//						std::exp(-distance_(difference_vector, data_.getCols()) / smooth_factor_);
	//				}
	//				else {
	//					kovariance_matrix(rows, cols) = kovariance_matrix(rows, cols) + difference_vector[rows] * difference_vector[cols];
	//				}

	//			}
	//		}
	//	}
	//	
	//	Eigen::JacobiSVD <Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> svd(kovariance_matrix,/* Eigen::ComputeThinU | */Eigen::ComputeThinV);
	//	principal_components_ = svd.matrixV();
	//}
}

#endif /* UTILS_PCA_H_ */