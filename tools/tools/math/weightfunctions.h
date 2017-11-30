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

#ifndef MATH_WEIGHTFUNCTIONS_H_
#define MATH_WEIGHTFUNCTIONS_H_

#include "tools/utils/matrix.h"

#include "tools/math/standard.h"

namespace math
{
	/**
		Computes the weights from distances
		
		@param[in] data Data points
		@param[in] weights Matrix with the weigths
		@param[in,out] weight_function Defines the weight function
	*/
	template<typename ElementType> void getWeightsDistances(
		const utils::Matrix<ElementType>& distances, 
		utils::Matrix<ElementType>& weights, 
		WeightFunction weight_function)
	{
		switch (weight_function) {
		case WeightFunction::BOX: {
			math::WeightFunctionBox<ElementType> weight_function_box;
			weights = weight_function_box(distances);
			break;
			}
		case WeightFunction::LINEAR: {
			math::WeightFunctionLinear<ElementType> weight_function_linear(distances, true);
			weights = weight_function_linear(distances);
			break;
			}
		case WeightFunction::GAUSSIAN: {
			math::WeightFunctionGaussian<ElementType> weight_function_gaussian(utils::Matrix<ElementType>({ 0 }, 1, 1), distances);
			weights = weight_function_gaussian(distances);
			break;
			}
		}
	}

	template<typename ElementType> class WeightFunctionBox
	{
	public:
		/**
			Constructor
		*/
		WeightFunctionBox()
		{
		}

		/**
			Destructor
		*/
		~WeightFunctionBox()
		{
		}
		
		/**
			Copy constructor

			@param[in] weight_function_box An instance of class WeightFunctionBox
		*/
		WeightFunctionBox(const WeightFunctionBox<ElementType>&  weight_function_box) = delete;

		/**
			Copy constructor
	
			@param[in] weight_function_box An instance of class WeightFunctionBox
		*/
		WeightFunctionBox(const WeightFunctionBox<ElementType>&& weight_function_box) = delete;
	
		/**
			Operator =
	
			@param[in] weight_function_box An instance of class WeightFunctionBox
			@return Returns reference to the current instance
		*/
		WeightFunctionBox& operator=(const WeightFunctionBox<ElementType>& weight_function_box) = delete;
	
		/**
			Operator =
		
			@param[in] weight_function_box An instance of class WeightFunctionBox
			@return Returns reference to the current instance
		*/
		WeightFunctionBox& operator=(const WeightFunctionBox<ElementType>&& weight_function_box) = delete;

		/**
			Operator()

			@param[in] x Data point
			@return Weight
		*/
		ElementType operator()(ElementType x) const
		{
			return (ElementType)1.0;
		}

		/**
			Operator()

			@param[in] data Data points
			@param[in] number_of_elements
			@return Weights
		*/
		ElementType* operator()(ElementType* data, size_t number_of_elements) const
		{
			ElementType* weights = new ElementType[number_of_elements];
			std::memset(weights, (ElementType)1.0, sizeof(ElementType) * number_of_elements);

			return weights;
		}

		/**
			Operator()

			@param[in] matrix Matrix
			@return Weights
		*/
		utils::Matrix<ElementType> operator()(const utils::Matrix<ElementType>& matrix) const
		{
			return utils::Matrix<ElementType>((*this)(matrix.getPtr(), matrix.getRows()), matrix.getRows(), 1);
		}
	};

	template<typename ElementType> class WeightFunctionLinear
	{
	public:
		/**
			Constructor
		*/
		WeightFunctionLinear() : a_(NULL), b_(NULL)
		{
		}

		/**
			Constructor

			@param[in] data Data points
			@param[in] number_of_elements Number of elements
		*/
		WeightFunctionLinear(ElementType* data, size_t number_of_elements, bool min = true) : WeightFunctionLinear()
		{
			a_ = data[math::min(data, number_of_elements)];
			b_ = data[math::max(data, number_of_elements)];
			if (!min) {
				swap(a_, b_);
			}
		}		

		/**
			Constructor

			@param[in] data Data points
		*/
		WeightFunctionLinear(const utils::Matrix<ElementType>& data, bool min = true) : WeightFunctionLinear()
		{
			a_ = data[math::min(data.getPtr(), data.getRows())][0];
			b_ = data[math::max(data.getPtr(), data.getRows())][0];
			if (!min) {
				swap(a_, b_);
			}
		}	

		/**
			Destructor
		*/
		~WeightFunctionLinear()
		{
		}
		
		/**
			Copy constructor

			@param[in] weight_function_linear An instance of class WeightFunctionLinear
		*/
		WeightFunctionLinear(const WeightFunctionLinear<ElementType>&  weight_function_linear) = delete;

		/**
			Copy constructor
	
			@param[in] weight_function_linear An instance of class WeightFunctionLinear
		*/
		WeightFunctionLinear(const WeightFunctionLinear<ElementType>&& weight_function_linear) = delete;
	
		/**
			Operator =
	
			@param[in] weight_function_linear An instance of class WeightFunctionLinear
			@return Returns reference to the current instance
		*/
		WeightFunctionLinear& operator=(const WeightFunctionLinear<ElementType>& weight_function_linear) = delete;
	
		/**
			Operator =
		
			@param[in] weight_function_linear An instance of class WeightFunctionLinear
			@return Returns reference to the current instance
		*/
		WeightFunctionLinear& operator=(const WeightFunctionLinear<ElementType>&& weight_function_linear) = delete;

		/**
			Operator()

			@param[in] x Data point
			@return Weight
		*/
		ElementType operator()(ElementType x) const
		{
			return (b_ - x) / (b_ - a_);
		}

		/**
			Operator()

			@param[in] data Data points
			@param[in] number_of_elements
			@return Weights
		*/
		ElementType* operator()(ElementType* data, size_t number_of_elements) const
		{
			ElementType* weights = new ElementType[number_of_elements];
			ElementType* weights_ptr = weights;
			ElementType* end = data + number_of_elements;
			
			while (data != end) {
				*weights_ptr = (*this)(*data);
				weights_ptr++;
				data++;
			}

			return weights;
		}

		/**
			Operator()

			@param[in] matrix Matrix
			@return Weights
		*/
		utils::Matrix<ElementType> operator()(const utils::Matrix<ElementType>& matrix) const
		{
			 return utils::Matrix<ElementType>((*this)(matrix.getPtr(), matrix.getRows()), matrix.getRows(), 1);
		}

		/**
			Get lower bound

			@return Lower bound
		*/
		ElementType getLowerBound()
		{
			return a_;
		}

		/**
			Get upper bound

			@return Upper bound
		*/
		ElementType getUpperBound()
		{
			return b_;
		}

	private:

		/**
			Lower bound
		*/
		ElementType a_;

		/**
			Upper bound
		*/
		ElementType b_;

	};

	template<typename ElementType> class WeightFunctionGaussian
	{
	public:
		/**
			Constructor
		*/
		WeightFunctionGaussian() : dim_(0), mean_(nullptr), var_(nullptr)
		{
		}

		/**
			Constructor
			Computes the mean and the variances of the data points

			@param[in] mean Mean
			@param[in] data Data points
			@param[in] number_of_elements Number of elements
		*/
		WeightFunctionGaussian(ElementType mean, ElementType* data, size_t number_of_elements) : WeightFunctionGaussian()
		{
			dim_ = 1;

			mean_ = &mean;

			var_ = &math::computeVar<ElementType>(mean, data, number_of_elements);
		}

		/**
			Constructor
			Computes the mean and the variances of the data points

			@param[in] data Data points
		*/
		WeightFunctionGaussian(ElementType* data, size_t number_of_elements) : WeightFunctionGaussian()
		{
			dim_ = 1;

			mean_ = &math::computeMean<ElementType>(data, number_of_elements);

			var_ = &math::computeVar<ElementType>(mean, data, number_of_elements);
		}

		/**
			Constructor
			Computes the mean and the variances of the data points

			@param[in] mean Mean
			@param[in] data Data points
		*/
		WeightFunctionGaussian(const utils::Matrix<ElementType>& mean, const utils::Matrix<ElementType>& data) : WeightFunctionGaussian()
		{
			dim_ = data.getCols();

			mean_ = mean.getAllocatedPtr();

			utils::Matrix<ElementType> var = math::computeVar<ElementType>(mean, data);

			var_ = var.getDiag();
		}

		/**
			Constructor
			Computes the mean and the variances of the data points

			@param[in] data Data points
		*/
		WeightFunctionGaussian(const utils::Matrix<ElementType>& data) : WeightFunctionGaussian()
		{
			dim_ = data.getCols();

			utils::Matrix<ElementType> mean = math::computeMean<ElementType>(data);

			mean_ = mean.getAllocatedPtr();
	
			utils::Matrix<ElementType> var = math::computeVar<ElementType>(mean, data);

			var_ = var.getDiag();
		}

		/**
			Destructor
		*/
		~WeightFunctionGaussian()
		{
			clearMemory();
		}


		/**
			Clear Memory
		*/
		void  clearMemory()
		{
			if (var_) {
				delete[] var_;
				var_ = nullptr;
			}
			if (mean_) {
				delete[] mean_;
				mean_ = nullptr;
			}
		}

		/**
			Copy constructor

			@param[in] weight_function_gaussian An instance of class  WeightFunctionGaussian
		*/
		WeightFunctionGaussian(const WeightFunctionGaussian<ElementType>&  weight_function_gaussian) = delete;

		/**
			Copy constructor
	
			@param[in] weight_function_gaussian An instance of class WeightFunctionGaussian
		*/
		WeightFunctionGaussian(const WeightFunctionGaussian<ElementType>&& weight_function_gaussian) = delete;
	
		/**
			Operator =
	
			@param[in] weight_function_gaussian An instance of class WeightFunctionGaussian
			@return Returns reference to the current instance
		*/
		WeightFunctionGaussian& operator=(const WeightFunctionGaussian<ElementType>& weight_function_gaussian) = delete;
	
		/**
			Operator =
		
			@param[in] weight_function_gaussian An instance of class WeightFunctionGaussian
			@return Returns reference to the current instance
		*/
		WeightFunctionGaussian& operator=(const WeightFunctionGaussian<ElementType>&& weight_function_gaussian) = delete;

		/**
			Operator()
			
			@param[in] x Data point
			@return Weight
		*/
		ElementType operator()(ElementType x) const
		{
			return std::exp(-std::pow(x - mean_[0], 2) / (2 * var_[0]));
		}
		
		/**
			Operator()

			@param[in] x Data point
			@param[in] y Data point
			@return Weight
		*/
		ElementType operator()(ElementType x, ElementType y) const
		{
			return std::exp(-(std::pow(x - mean_[0], 2) / (2 * var_[0]) +
				std::pow(y - mean_[1], 2) / (var_[1])));
		}

		/**
			Operator()

			@param[in] x Data point
			@param[in] y Data point
			@param[in] z Data point
			@return Weight
		*/
		ElementType operator()(ElementType x, ElementType y, ElementType z) const
		{
			return std::exp(-(std::pow(x - mean_[0], 2) / (2 * var_[0]) +
				std::pow(y - mean_[1], 2) / (2 * var_[1]) +
				std::pow(z - mean_[2], 2) / (2 * var_[2])));
		}

		/**
			Operator()

			@param[in] x Data array
			@return Weight
		*/
		ElementType operator()(ElementType* x) const
		{
			ElementType norm = 0;
			for (size_t i = 0; i < dim_; i++)
			{
				norm += std::pow(x[i] - mean_[i], 2) / (2 * var_[i]);
			}
			return std::exp(-norm);
		}

		/**
			Operator()

			@param[in] matrix Matrix
			@return Weight
		*/
		utils::Matrix<ElementType> operator()(const utils::Matrix<ElementType>& matrix) const
		{
			utils::Matrix<ElementType> weight(matrix.getRows(),1);

			for (size_t i = 0; i < matrix.getRows(); i++) {
				weight[i][0] = (*this)(matrix[i]);
			}

			return weight;
		}

		/**
			Get dimension

			@return Dimension
		*/
		size_t getDim()
		{
			return dim_;
		}

		/**
			Get mean

			@return Mean
		*/
		ElementType* getMean()
		{
			return mean_;
		}

		/**
			Get variances

			@return Variances
		*/
		ElementType* getVar()
		{
			return var_;
		}
	private:

		/**
			Dimension
		*/
		size_t dim_;

		/**
			Mean
		*/
		ElementType* mean_;

		/**
			Variances 
		*/
		ElementType* var_;
	};
}

#endif /* MATH_WEIGHTFUNCTIONS_H_ */