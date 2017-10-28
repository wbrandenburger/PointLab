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

#ifndef MATH_ADJUSTMENT_H_
#define MATH_ADJUSTMENT_H_

#include "utils/matrix.h"

#include "eigen3/Eigen/Dense"

namespace math
{

	template<typename ElementType, typename DistanceType> class PolynomialAdjustment
	{

	public:

		/**
			Constructor
		*/
		PolynomialAdjustment() {}

		/**
			Constructor

			@param[in] degree_ Degree of the polynomial
			@param[in] distance Distance function
		*/
		PolynomialAdjustment(size_t degree_, const DistanceType& distance_) : degree(degree_), distance(distance_) {}

		/**
			Operator() Adjustment of a line, surface, volume

			@param[in] data_ Matrix with the points
			@param[in] instance_ The point which determines the frame
			@param[in,out] parameter_ The coefficients of the polynomial
		*/
		void operator()(/*const*/ utils::Matrix<ElementType>& data_, ElementType* const instance_,
			Eigen::Matrix<double, Eigen::Dynamic, 1>& parameter_,  double smooth_factor_ = 0)/* const*/
		{
			size_t polynom = data_.getCols();

			Eigen::Matrix<double, Eigen::Dynamic, 1> observation_vector(data_.getRows());
			for (size_t rows = 0; rows < data_.getRows(); rows++) {
				observation_vector(rows, 0) = data_[rows][data_.getCols()-1];
			}

			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> weight_matrix(data_.getRows(), data_.getRows());
			for (size_t rows = 0; rows < data_.getRows(); rows++) {
				for (size_t cols = 0; cols < data_.getRows(); cols++) {
					if (rows == cols) {
						if (smooth_factor_) {
							weight_matrix(rows, cols) = std::exp(- (double) distance(data_[rows], instance_, data_.getCols()) / smooth_factor_);
						}
						else {
							weight_matrix(rows, cols) = 1;
						}
					}
					else {
						weight_matrix(rows, cols) = 0;
					}
				}
			}
		
			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> design_matrix;

			switch (polynom) {
			case 2: polynomial2D(data_, design_matrix);
				break;
			case 3: polynomial3D(data_, design_matrix);
				break;
			case 4: polynomial4D(data_, design_matrix);
				break;
			}

			std::cout << design_matrix << std::endl;
			std::cout << weight_matrix << std::endl;
			std::cout << observation_vector << std::endl;

			std::cout << (design_matrix.transpose()*design_matrix).inverse() << std::endl;
			parameter_ = (design_matrix.transpose()*weight_matrix*design_matrix).inverse()
				*design_matrix.transpose()*weight_matrix*observation_vector;
		}

	private:

		/**
			Operator() Adjustment of a line

			@param[in] data_ Matrix with the points
			@param[in,out] design_matrix_ The design matrix
		*/
		void polynomial2D(/*const*/ utils::Matrix<ElementType>& data_,
			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& design_matrix_) /*const*/
		{

			size_t number_parameters = degree + 1;

			design_matrix_ = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(data_.getRows(), number_parameters);

			for (size_t rows = 0; rows < data_.getRows(); rows++) {
				for (size_t i = 0; i <= degree; i++) {
					design_matrix_(rows, i) = std::pow( (double) data_[rows][0], i);
				}
			}


		}

		/**
			Operator() Adjustment of a surface

			@param[in] data_ Matrix with the points
			@param[in,out] design_matrix_ The design matrix
		*/
		void polynomial3D(/*const*/ utils::Matrix<ElementType>& data_,
			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& design_matrix_) /*const*/
		{
			size_t number_parameters = 0;
			for (size_t i = 0; i <= degree; i++) {
				number_parameters += i + 1;
			}
			
			design_matrix_ = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(data_.getRows(), number_parameters);

			for (size_t rows = 0; rows < data_.getRows(); rows++) {
				size_t index = 0;
				for (size_t i = 0; i <= degree; i++) {
					for (size_t j = 0; j <= i; j++) {
						for (size_t k = 0; k <= i; k++) {
							if (j + k == i) {
								design_matrix_(rows, index) = std::pow((double)data_[rows][0], j) *
									std::pow((double)data_[rows][1], k);
								index++;
							}
						}
					}
				}
			}

		}

		/**
			Operator() Adjustment of a volume

			@param[in] data_ Matrix with the points
			@param[in,out] design_matrix_ The design matrix
		*/
		void polynomial4D(/*const*/ utils::Matrix<ElementType>& data_,
			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& design_matrix_) /*const*/
		{
			size_t number_parameters = 0;
			size_t auxiliary_counter = 1;
			for (size_t i = 0; i <= degree; i++) {
				number_parameters += auxiliary_counter;
				auxiliary_counter += i + 2;
			}

			design_matrix_ = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(data_.getRows(), number_parameters);

			for (size_t rows = 0; rows < data_.getRows(); rows++) {
				size_t index = 0;
				for (size_t i = 0; i <= degree; i++) {
					for (size_t j = 0; j <= i; j++) {
						for (size_t k = 0; k <= i; k++) {
							for (size_t l = 0; l <= i; l++) {
								if (j + k + l == i) {
									design_matrix_(rows, index) = std::pow( (double) data_[rows][0], i) *
										std::pow( (double) data_[rows][1], j) * std::pow( (double) data_[rows][2], k);
									index++;
								}
							}
						}
					}
				}
			}
		}

	private:

		/**
			Degree of the polynomial
		*/
		size_t degree;

		/**
			Distance function
		*/
		DistanceType distance;
	};
}

#endif /* MATH_ADJUSMENT_H_ */