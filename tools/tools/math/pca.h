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

#ifndef MATH_PCA_H_
#define MATH_PCA_H_

#include "tools/utils/matrix.h"

#include "eigen3/Eigen/Dense"

namespace math
{
	/**
			Computes the principal components of a dataset
			
			@param[in] data_ n datapoints with a dimension of m
	*/
	template<typename ElementType, typename DistanceType> void principalComponentAnalysis (const utils::Matrix<ElementType>& data_,
		ElementType* const instance_, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& singular_values_,
		Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& singular_vectors_,
		const DistanceType& distance_, const double smooth_factor_ = 0)
	{
		Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> kovariance_matrix(data_.cols, data_.cols);

		utils::Difference<ElementType> difference;
		ElementType* difference_vector(new ElementType[data_.cols]);

		for (size_t rows = 0; rows < data_.cols; rows++) {
			for (size_t cols = 0; cols < data_.cols; cols++) {
				kovariance_matrix(rows, cols) = 0;
				for (size_t i = 0; i < data_.rows; i++) {
					difference(data_[i], instance_, difference_vector, data_.cols);
					if (smooth_factor_ != 0) {
						kovariance_matrix(rows, cols) = kovariance_matrix(rows, cols) + (double) difference_vector[rows] * (double) difference_vector[cols] *
							std::exp(- (double) distance_(difference_vector, data_.cols) / smooth_factor_);
					}
					else {
						kovariance_matrix(rows, cols) = kovariance_matrix(rows, cols) + difference_vector[rows] * difference_vector[cols];
					}

				}
			}
		}

		delete difference_vector;

		Eigen::JacobiSVD <Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> svd(kovariance_matrix,/* Eigen::ComputeThinU | */Eigen::ComputeThinV);
		singular_values_ = svd.singularValues();
		singular_vectors_= svd.matrixV();
	}
}

#endif /* MATH_PCA_H_ */