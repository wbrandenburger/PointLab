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

#ifndef TREES_NN_INDEX_H_
#define TREES_NN_INDEX_H_

#include <assert.h>

#include "util/matrix.h"
#include "util/params.h"
#include "util/result_set.h"
#include "util/threadpool.h"

namespace trees
{

	template<typename ElementType>
	class NNIndex
	{
	
	public:

		/**
			Constructor
		*/
		NNIndex() : size(0), veclen(0)
		{
		}

		/**
			Set parameters based on the input pointcloud
			
			@param[in] dataset_ Pointcloud
		*/
		void setDataset(const Matrix<ElementType>& dataset_)
		{
			size = dataset_.rows;
			veclen = dataset_.cols;

			dataset = dataset_;
		}

		/**
			Build the tree of the specified index
		*/
		virtual void buildIndex() {
			freeIndex();

			buildIndexImpl();
		}

		/**
			Free allocated memory
		*/
		virtual void freeIndex() = 0;

		/**
			Build the tree of the specified index
		*/
		virtual void buildIndexImpl() = 0;

		/**
			Prepares the search process, computes initial distances and calls the search function
			
			@param[in,out] result_set_ Container which contains the found neighbors
			@param[in] vec_ Point which neighbors shall be found
			@param[in] params_ Input parameters for the search
		*/
		virtual void findNeighbors(ResultSet<ElementType>& result_set_, const ElementType* vec_, const TreeParams& params_) const = 0;

		/**
			Perform k-nearest neighbor search

			@param[in] queries_ The query points for which to find the nearest neighbors
			@param[in,out] indices_ The indices of the nearest neighbors found
			@param[in,out] dists_ Distances to the nearest neighbors found
			@param[in] knn_ Number of nearest neighbors to return
			@param[in] params_ Search parameters
		*/
		void knnSearch(const Matrix<ElementType>& queries_,
			Matrix<size_t>& indices_,
			Matrix<ElementType>& dists_,
			size_t knn_,
			const TreeParams& params_)
		{
			assert(queries_.cols == veclen());
			assert(indices_.rows >= queries_.rows);
			assert(dists_.rows >= queries_.rows);
			assert(indices_.cols >= knn_);
			assert(dists_.cols >= knn_);
			
			Threadpool pool(params_.cores);

			for (int i = 0; i < queries_.rows; i++) {
				while (!pool.runTask(boost::bind(&NNIndex<ElementType>::knnSearchThreadpool,
					this, 
					boost::ref(queries_), 
					boost::ref(indices_), 
					boost::ref(dists_), 
					knn_,
					boost::ref(params_), 
					i)));
			}

			pool.shutdown();
		}

		/**
			Perform k-nearest neighbor search

			@param[in] queries_ The query points for which to find the nearest neighbors
			@param[in,out] indices_ The indices of the nearest neighbors found
			@param[in,out] dists_ Distances to the nearest neighbors found
			@param[in] knn_ Number of nearest neighbors to return
			@param[in] params_ Search parameters
			@param[in] index_ Number of the row of the queries
		*/
		void knnSearchThreadpool(const Matrix<ElementType>& queries_,
								 Matrix<size_t>& indices_,
								 Matrix<ElementType>& dists_,
								size_t knn_,
								const TreeParams& params_,
								size_t index_)
		{
			KNNResultSet2<ElementType> result_set(knn_);
			result_set.clear();
			findNeighbors(result_set, queries_[index_], params_);
			size_t n = std::min(result_set.size(), knn_);
			result_set.copy(indices_[index_], dists_[index_], n);
		}

		/**
			Perform k-nearest neighbor search

			@param[in] queries_ The query points for which to find the nearest neighbors
			@param[in,out] indices_ The indices of the nearest neighbors found
			@param[in,out] dists_ Distances to the nearest neighbors found
			@param[in] knn_ Number of nearest neighbors to return
			@param[in] params_ Search parameters
		*/
		void knnSearch(const Matrix<ElementType>& queries_,
			Matrix<int>& indices_,
			Matrix<ElementType>& dists_,
			size_t knn_,
			const TreeParams& params_)
		{
			flann::Matrix<size_t> indices(new size_t[indices_.rows*indices_.cols], indices_.rows, indices_.cols);
			knnSearch(queries_, indices, dists_, knn_, params_);

			for (size_t i = 0; i < indices_.rows; ++i) {
				for (size_t j = 0; j < indices_.cols; ++j) {
					indices_[i][j] = indices[i][j];
				}
			}
			delete[] indices.ptr();
		}

	protected:
	
		/**
			Numbers of points in pointcloud
		*/
		size_t size;

		/**
			Number of dimensions of data
		*/
		size_t veclen;

		/**
			Pointcloud
		*/
		Matrix<ElementType> dataset;
	};

}

#endif /* PCSIMP_NN_INDEX_H_ */