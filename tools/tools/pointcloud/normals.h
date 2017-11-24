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
#include "tools/utils/threadpool.h"

#include "tools/pointcloud/pointcloud.h"

#include "tools/math/function.h"
#include "tools/math/standard.h"
#include "tools/math/weightfunctions.h"

#include "trees/trees.hpp"

#include "eigen3/Eigen/Dense"

namespace pointcloud
{
	/**
		Structure which holds tree parameters
	*/
	struct NormalParams 
	{
	public:
		/**
			Constructor
		*/
		NormalParams() :
			normal_computation_(NormalComputation::PLANESVD), 
			weight_function_(WeightFunction::LINEAR), 
			cores_(1)
		{
		}

		/**
			Constructor

			@param[in] normal_computation Method which will be used for computing normals
			@param[in] weight_function Defines the weight function
			@param[in] cores Number of cores
		*/
		NormalParams(NormalComputation normal_computation,
			WeightFunction weight_function,
			size_t cores)  : NormalParams()
		{
			normal_computation_ = normal_computation;
			weight_function_ = weight_function;
			cores_ = cores;
		}

		/**
			Set method which will be used for computing normals
		*/
		void setNormalComputation(NormalComputation normal_computation)
		{
			normal_computation_ = normal_computation;
		}

		/**
			Set the weight function
		*/
		void setWeightFunction(WeightFunction weight_function)
		{
			weight_function_ = weight_function;
		}
				
		/**
			Set number of cores
		*/
		void setCores(size_t cores)
		{
			cores_ = cores;
		}

		/**
			Get method which will be used for computing normals

			@return Method which will be used for computing normals
		*/
		NormalComputation getNormalComputation()
		{
			return normal_computation_;
		}

		/**
			Get the weight function

			@return Weight function
		*/
		WeightFunction getWeightFunction()
		{
			return weight_function_;
		}
				
		/**
			Get number of cores
			
			@return Number of cores
		*/
		size_t getCores()
		{
			return cores_;
		}

	private:
		/**
			Method for computing normals
		*/
		NormalComputation normal_computation_;

		/**
			WeightFunction weight_function
		*/
		WeightFunction weight_function_;

		/**
			Cores
		*/
		size_t cores_;
	};

	/**
		Computes the normals of a pointcloud and sets the normals
		
		@param[in,out] pointcloud Pointcloud
		@param[in] neighbors Number of neighbors which will be considered for computation normals
		@param[in] normal_params Parameter for computing normals
	*/
	template<typename ElementType> void computeNormals(pointcloud::Pointcloud<ElementType>& pointcloud, 
		size_t  neighbors,
		NormalParams normal_params = NormalParams())
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
			Build container which include the indices of the neighbors
		*/
		trees::TreeParams tree_params;
		tree_params.setCores(normal_params.getCores());

		utils::Matrix<size_t> indices(pointcloud.getNumberOfVertices(), neighbors);
		utils::Matrix<ElementType> dists(pointcloud.getNumberOfVertices(), neighbors);
		
		utils::Matrix<ElementType> points;
		pointcloud.getMatrix(points);
		
		/**
			Search for the neighbors 
		*/
		kdtree_index.knnSearch(points, indices, dists, neighbors, tree_params);

		/**
			Compute the normals
		*/
		if (normal_params.getCores() != 1) {
			utils::Threadpool pool(normal_params.getCores());
			for (size_t i = 0; i < pointcloud.getNumberOfVertices(); i++) {
				while (!pool.runTask(boost::bind(&computeNormal<ElementType>,
					i,
					std::ref(pointcloud),
					indices[i],
					neighbors,
					normal_params)));
			}

			pool.shutdown();
		}
		else {
			for (size_t i = 0; i < pointcloud.getNumberOfVertices(); i++) {
				computeNormal<ElementType>(
					i,
					pointcloud,
					indices[i],
					neighbors,
					normal_params);
			}
		}
	}

	/**
		Computes the normal of a point and sets the normal
		
		@param[in] index Index of the query point in the pointcloud
		@param[in,out] pointcloud Pointcloud
		@param[in] indices Indices of the neighbors of one point
		@param[in] neighbors Number of neighbors
		@param[in] normal_params Parameter for computing normals
	*/
	template<typename ElementType> void computeNormal(size_t index,
		pointcloud::Pointcloud<ElementType>& pointcloud,
		size_t* indices,
		size_t neighbors,
		NormalParams normal_params)
	{
		/**
			Get the neighbors of a point
		*/
		utils::Matrix<ElementType> points;
		pointcloud.getSubset(indices, neighbors, points);
		
		/**
			Compute the normal
		*/
		ElementType normal[3];
		std::memset(normal, (ElementType)0, sizeof(ElementType) * 3);
		computeNormal<ElementType>(normal, points, normal_params);

		/**
			Set the normal
		*/
		pointcloud.setNormalPtr(normal, index);
	}

	/**
		Normal computation switch which depends on the chosen method for computing normals
		
		@param[in,out] normal Normal
		@param[in] points Matrix with the points
		@param[in] normal_params Parameter for computing normals
	*/
	template<typename ElementType> void computeNormal(ElementType* normal,
		const utils::Matrix<ElementType>& points,
		NormalParams normal_params = NormalParams())
	{
		switch (normal_params.getNormalComputation()) {
		case NormalComputation::PLANESVD: normalPlaneSVD(normal, points, normal_params.getWeightFunction()); break;
		case NormalComputation::PLANEPCA: normalPlanePCA(normal, points, normal_params.getWeightFunction()); break;
		case NormalComputation::VECTORSVD: normalVectorSVD(normal, points, normal_params.getWeightFunction()); break;
		case NormalComputation::QUADSVD: normalQuadSVD(normal, points, normal_params.getWeightFunction()); break;
		}
	}

	/**
		 A classical method is to fit a local plane S=n_{x}x+n_{y}y+n_{z}z+d 
		 to the points in the neighborhood

		 @param[in,out] normal Normal
		 @param[in] points Matrix with the points
		 @param[in] weight_function Defines the weight function
	*/
	template<typename ElementType> void normalPlaneSVD(ElementType* normal, 
		const utils::Matrix<ElementType>& points,
		WeightFunction weight_function = WeightFunction::LINEAR)
	{
		/**
			Define the reference point. This is the first point in points
		*/
		utils::Matrix<ElementType> point = points.getRowMatrix(0);;
		
		/**
			Computation of the distances to the reference point and define the weigths
		*/
		utils::Matrix<ElementType> distances = std::sqrt(math::euclideanDistance<ElementType>(points - point));
		
		utils::Matrix<ElementType> weights;
		switch (weight_function) {
		case WeightFunction::GAUSSIAN: {
			math::WeightFunctionGaussian<ElementType> weight_function_gaussian(utils::Matrix<ElementType>({ 0 }, 1, 1), distances);
			weights = weight_function_gaussian(distances);
			break;
			}
		case WeightFunction::LINEAR: {
			math::WeightFunctionLinear<ElementType> weight_function_linear(distances, true);
			weights = weight_function_linear(distances);
			break;
			}
		}
		
		/**
			Build a nx4 design matrix = [x y z 1] with n = number of points
		*/
		utils::Matrix<ElementType> design_matrix(points.getRows(), 4);
		for (size_t i = 0; i < points.getRows(); i++) {
			design_matrix[i][0] = points[i][0];
			design_matrix[i][1] = points[i][1];
			design_matrix[i][2] = points[i][2];
			design_matrix[i][3] = (ElementType)1;
		}
		
		/**
			Add the weights to the design matrix
		*/
		design_matrix = weights*design_matrix;

		/**
			Compute the singular value decomposition of A
		*/
		Eigen::Map<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic,Eigen::RowMajor>> design_eigen(
			design_matrix.getPtr(), design_matrix.getRows(), design_matrix.getCols());
		Eigen::JacobiSVD<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> svd_eigen(
			design_eigen, Eigen::ComputeThinU | Eigen::ComputeThinV);

		/**
			The last vector of matrix v computed by the singular value decomposition corresponds to the
			smallest singular value
		*/
		Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic> v_eigen = svd_eigen.matrixV();
		normal[0] = v_eigen(0, 3);
		normal[1] = v_eigen(1, 3);
		normal[2] = v_eigen(2, 3);
	}

	/**
		Instead of minimizing the fitting error, one can minimize the variance by removing the empirical
		mean from the data matrix Q and then performing a SVD on the modified data matrix

		 @param[in,out] normal Normal
		 @param[in] points Matrix with the points
		 @param[in] weight_function Defines the weight function
	*/
	template<typename ElementType> void normalPlanePCA(ElementType* normal,
		const utils::Matrix<ElementType>& points, 
		WeightFunction weight_function = WeightFunction::LINEAR)
	{
	}

	/**
		An equally straight - forward alternative to fitting a local plane S into Q is to maximize
		the angle(minimize the inner product) between the tangential vectors from 
		p to q and the normal vector n

		@param[in,out] normal Normal
		@param[in] points Matrix with the points
		@param[in] weight_function Defines the weight function
	*/
	template<typename ElementType> void normalVectorSVD(ElementType* normal,
		const utils::Matrix<ElementType>& points, 
		WeightFunction weight_function = WeightFunction::LINEAR)
	{
	}
	
	/**
		Some methods apprximate not only the orientation of the tangent plane but also the curvature
		by fitting a quadric surface

		@param[in,out] normal Normal
		@param[in] points Matrix with the points
		@param[in] weight_function Defines the weight function
	*/
	template<typename ElementType> void normalQuadSVD(ElementType* normal,
		const utils::Matrix<ElementType>& points, 
		WeightFunction weight_function = WeightFunction::LINEAR)
	{
	}
}

#endif /* POINTCLOUD_NORMALS_H_ */