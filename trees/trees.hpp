/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017  Wolfgang Brandenburger (w.brandenburger@unibw.de). All rights reserved.
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

#ifndef TREES_HPP_
#define TREES_HPP_

#include "defines.h"

#include "algorithms/all_indices.h"

#include "utils/params.h"

namespace trees
{
	template<typename ElementType>
	class Index
	{
	public:

		/**
			Constructor

			@param[in] dataset_ Pointcloud
			@param[in] params_ Input parameters
		*/
		Index(const utils::Matrix<ElementType>& dataset_, const IndexParams& params_) : params(params_)
		{
			treeIndex indexType = get_param<treeIndex>(params, "index");
			nnIndex = createIndexByType<ElementType>(indexType, dataset_, params);
		}

		/**
			Frees allocated memory
		*/
		void freeIndex() {
			nnIndex->freeIndex();
		}


		/**
			Get the dataset
		*/
		void getDataset(utils::Matrix<ElementType>& dataset_)
		{
			nnIndex->getDataset(dataset_);
		}

		/**
			Build the tree
		*/
		void buildIndex()
		{
			nnIndex->buildIndex();
		}

		/** 
			Rebuilds the index

			@param[in] dataset_ Pointcloud
		*/
		void rebuild(const utils::Matrix<ElementType>& dataset_)
		{
			nnIndex->rebuild(dataset_);
		}

		/**
			Removes point from kdtree

			@param[in] index_ Index of the point in the pointcloud
		*/
		bool remove(size_t index_)
		{
			return nnIndex->remove(index_);
		}

		/**
			Perform k-nearest neighbor search
			
			@param[in] queries_ The query points for which to find the nearest neighbors
			@param[in,out] indices_ The indices of the nearest neighbors found
			@param[in,out] dists_ Distances to the nearest neighbors found
			@param[in] knn_ Number of nearest neighbors to return
			@param[in] params_ Search parameters
		*/
		void knnSearch(const utils::Matrix<ElementType>& queries_,
			utils::Matrix<size_t>& indices_,
			utils::Matrix<ElementType>& dists_,
			size_t knn_,
			const TreeParams& params_) 
		{
			nnIndex->knnSearch(queries_, indices_, dists_, knn_, params_);
		}

		/**
			Perform k-nearest neighbor search

			@param[in] queries_ The query points for which to find the nearest neighbors
			@param[in,out] indices_ The indices of the nearest neighbors found
			@param[in,out] dists_ Distances to the nearest neighbors found
			@param[in] knn_ Number of nearest neighbors to return
			@param[in] params_ Search parameters
		*/
		void knnSearch(const utils::Matrix<ElementType>& queries_,
			utils::Matrix<int>& indices_,
			utils::Matrix<ElementType>& dists_,
			size_t knn_,
			const TreeParams& params_)
		{
			nnIndex->knnSearch(queries_, indices_, dists_, knn_, params_);
		}


		/**
			Perform radius search

			@param[in] queries_ The query points for which to find the nearest neighbors
			@param[in,out] indices_ The indices of the nearest neighbors found
			@param[in,out] dists_ Distances to the nearest neighbors found
			@param[in] radius_ The radius used for search
			@param[in] params_ Search parameters
		*/
		void radiusSearch(const utils::Matrix<ElementType>& queries_,
			std::vector< std::vector<size_t>>& indices_,
			std::vector<std::vector<ElementType>>& dists_,
			float radius_,
			const TreeParams& params_)
		{
			nnIndex->radiusSearch(queries_, indices_, dists_, radius_, params_);
		}

		/**
			Perform radius search

			@param[in] queries_ The query points for which to find the nearest neighbors
			@param[in,out] indices_ The indices of the nearest neighbors found
			@param[in,out] dists_ Distances to the nearest neighbors found
			@param[in] radius_ The radius used for search
			@param[in] params_ Search parameters
		*/
		void radiusSearch(const utils::Matrix<ElementType>& queries_,
			std::vector< std::vector<int>>& indices_,
			std::vector<std::vector<ElementType>>& dists_,
			float radius_,
			const TreeParams& params_)
		{
			nnIndex->radiusSearch(queries_, indices_, dists_, radius_, params_);
		}

	private:

		/**
			Index
		*/
		NNIndex<ElementType>* nnIndex;

		/**
			Input parameters
		*/
		IndexParams params;

	};
}

#endif /* TREES_HPP_ */