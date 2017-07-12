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

#ifndef TREES_KDTREE_INDEX_H_
#define TREES_KDTREE_INDEX_H_

#include <vector>
#include <assert.h>

#include "defines.h"
#include "algorithms/nn_index.h"

#include "util/allocator.h"
#include "util/matrix.h"
#include "util/result_set.h"
#include "util/params.h"

namespace trees
{

	struct KDTreeIndexParams : public IndexParams
	{
		/**
			Constructor

			@param neighbor_ Number of used neighbors for computing surface
		*/
		KDTreeIndexParams(int neighbor_ = 30, bool ordered_ = true)
		{
			(*this)["index"] = TREE_INDEX_KDTREE;
			(*this)["neighbor"] = neighbor_;
			(*this)["ordered"] = ordered_;

		}
	};


	template<typename ElementType>
	class KDTreeIndex : public NNIndex<ElementType>
	{
	public:

		/**
			Constructor

			@param data_ Pointcloud
			@param params_ Input parameters for the tree
		*/
		KDTreeIndex(const Matrix<ElementType>& dataset_, const IndexParams& params_ = KDTreeIndexParams())
		{
			neighbor = get_param(params_, "neighbor", 30);
			ordered = get_param(params_, "ordered", true);

			setDataset(dataset_);
		}

		struct Interval
		{
			ElementType low, high;
		};

		typedef std::vector<Interval> BoundingBox;
		
		struct Node
		{
			/**
			* Indices of points in leaf node
			*/
			int left, right;
			/**
			* Dimension used for subdivision.
			*/
			int divfeat;
			/**
			* The values used for subdivision.
			*/
			ElementType divlow, divhigh;
			/**
			* The child nodes.
			*/
			Node* child1, *child2;

			~Node()
			{
				if (child1) child1->~Node();
				if (child2) child2->~Node();
			}
		};

		typedef Node* NodePtr;

	private:

		void buildIndexImpl()
		{
			// Create a permutable array of indices to the input vectors.
			vind.resize(size);
			for (size_t i = 0; i < size; i++) {
				vind[i] = i;
			}

			computeBoundingBox(root_bbox);
			root_node = divideTree(0, size, root_bbox);

			if (ordered) {
				dataset_ordered = Matrix<ElementType>(new ElementType[size*veclen], size, veclen);
				for (size_t i = 0; i<size; ++i) {
					std::copy(dataset[vind[i]], dataset[vind[i]] + veclen, dataset_ordered[i]);
				}
			}
		}

		void freeIndex() 
		{
			//if (ordered){ 
			//	dataset_ordered.clear();
			//}
			//if (root_node) root_node->~Node();
			//pool.free();
		}

		void computeBoundingBox(BoundingBox& bbox)
		{
			bbox.resize(veclen);
			for (size_t i = 0; i<veclen; ++i) {
				bbox[i].low = (ElementType)dataset[0][i];
				bbox[i].high = (ElementType)dataset[0][i];
			}
			for (size_t k = 1; k<size; ++k) {
				for (size_t i = 0; i<veclen; ++i) {
					if (dataset[k][i]<bbox[i].low) bbox[i].low = (ElementType)dataset[k][i];
					if (dataset[k][i]>bbox[i].high) bbox[i].high = (ElementType)dataset[k][i];
				}
			}
		}


		/**
		* Create a tree node that subdivides the list of vecs from vind[first]
		* to vind[last].  The routine is called recursively on each sublist.
		* Place a pointer to this new tree node in the location pTree.
		*
		* Params: pTree = the new node to create
		*                  first = index of the first vector
		*                  last = index of the last vector
		*/
		NodePtr divideTree(int left_, int right_, BoundingBox& bbox_)
		{
			NodePtr node = new (pool) Node(); // allocate memory

											   /* If too few exemplars remain, then make this a leaf node. */
			if ((right_ - left_) <= neighbor) {
				node->child1 = node->child2 = NULL;    /* Mark as leaf node. */
				node->left = left_;
				node->right = right_;

				// compute bounding-box of leaf points
				for (size_t i = 0; i<veclen; ++i) {
					bbox_[i].low = (ElementType)dataset[vind[left_]][i];
					bbox_[i].high = (ElementType)dataset[vind[left_]][i];
				}
				for (int k = left_ + 1; k<right_; ++k) {
					for (size_t i = 0; i<veclen; ++i) {
						if (bbox_[i].low>dataset[vind[k]][i]) bbox_[i].low = (ElementType)dataset[vind[k]][i];
						if (bbox_[i].high<dataset[vind[k]][i]) bbox_[i].high = (ElementType)dataset[vind[k]][i];
					}
				}
			}
			else {
				int idx;
				int cutfeat;
				ElementType cutval;
				middleSplit(&vind[0] + left_, right_ - left_, idx, cutfeat, cutval, bbox_);

				node->divfeat = cutfeat;

				BoundingBox left_bbox(bbox_);
				left_bbox[cutfeat].high = cutval;
				node->child1 = divideTree(left_, left_ + idx, left_bbox);

				BoundingBox right_bbox(bbox_);
				right_bbox[cutfeat].low = cutval;
				node->child2 = divideTree(left_ + idx, right_, right_bbox);

				node->divlow = left_bbox[cutfeat].high;
				node->divhigh = right_bbox[cutfeat].low;

				for (size_t i = 0; i<veclen; ++i) {
					bbox_[i].low = std::min(left_bbox[i].low, right_bbox[i].low);
					bbox_[i].high = std::max(left_bbox[i].high, right_bbox[i].high);
				}
			}

			return node;
		}

		void computeMinMax(int* ind, int count, int dim, ElementType& min_elem, ElementType& max_elem)
		{
			min_elem = dataset[ind[0]][dim];
			max_elem = dataset[ind[0]][dim];
			for (int i = 1; i<count; ++i) {
				ElementType val = dataset[ind[i]][dim];
				if (val<min_elem) min_elem = val;
				if (val>max_elem) max_elem = val;
			}
		}

		void middleSplit(int* ind, int count, int& index, int& cutfeat, ElementType& cutval, const BoundingBox& bbox)
		{
			// find the largest span from the approximate bounding box
			ElementType max_span = bbox[0].high - bbox[0].low;
			cutfeat = 0;
			cutval = (bbox[0].high + bbox[0].low) / 2;
			for (size_t i = 1; i<veclen; ++i) {
				ElementType span = bbox[i].high - bbox[i].low;
				if (span>max_span) {
					max_span = span;
					cutfeat = i;
					cutval = (bbox[i].high + bbox[i].low) / 2;
				}
			}

			// compute exact span on the found dimension
			ElementType min_elem, max_elem;
			computeMinMax(ind, count, cutfeat, min_elem, max_elem);
			cutval = (min_elem + max_elem) / 2;
			max_span = max_elem - min_elem;

			// check if a dimension of a largest span exists
			size_t k = cutfeat;
			for (size_t i = 0; i<veclen; ++i) {
				if (i == k) continue;
				ElementType span = bbox[i].high - bbox[i].low;
				if (span>max_span) {
					computeMinMax(ind, count, i, min_elem, max_elem);
					span = max_elem - min_elem;
					if (span>max_span) {
						max_span = span;
						cutfeat = i;
						cutval = (min_elem + max_elem) / 2;
					}
				}
			}
			int lim1, lim2;
			planeSplit(ind, count, cutfeat, cutval, lim1, lim2);

			if (lim1>count / 2) index = lim1;
			else if (lim2<count / 2) index = lim2;
			else index = count / 2;

			assert(index > 0 && index < count);
		}


		void middleSplit_(int* ind, int count, int& index, int& cutfeat, ElementType& cutval, const BoundingBox& bbox)
		{
			const float eps_val = 0.00001f;
			DistanceType max_span = bbox[0].high - bbox[0].low;
			for (size_t i = 1; i<veclen; ++i) {
				DistanceType span = bbox[i].high - bbox[i].low;
				if (span>max_span) {
					max_span = span;
				}
			}
			DistanceType max_spread = -1;
			cutfeat = 0;
			for (size_t i = 0; i<veclen; ++i) {
				DistanceType span = bbox[i].high - bbox[i].low;
				if (span>(DistanceType)((1 - eps_val)*max_span)) {
					ElementType min_elem, max_elem;
					computeMinMax(ind, count, cutfeat, min_elem, max_elem);
					DistanceType spread = (DistanceType)(max_elem - min_elem);
					if (spread>max_spread) {
						cutfeat = i;
						max_spread = spread;
					}
				}
			}
			// split in the middle
			DistanceType split_val = (bbox[cutfeat].low + bbox[cutfeat].high) / 2;
			ElementType min_elem, max_elem;
			computeMinMax(ind, count, cutfeat, min_elem, max_elem);

			if (split_val<min_elem) cutval = (DistanceType)min_elem;
			else if (split_val>max_elem) cutval = (DistanceType)max_elem;
			else cutval = split_val;

			int lim1, lim2;
			planeSplit(ind, count, cutfeat, cutval, lim1, lim2);

			if (lim1>count / 2) index = lim1;
			else if (lim2<count / 2) index = lim2;
			else index = count / 2;

			assert(index > 0 && index < count);
		}


		/**
		*  Subdivide the list of points by a plane perpendicular on axe corresponding
		*  to the 'cutfeat' dimension at 'cutval' position.
		*
		*  On return:
		*  dataset[ind[0..lim1-1]][cutfeat]<cutval
		*  dataset[ind[lim1..lim2-1]][cutfeat]==cutval
		*  dataset[ind[lim2..count]][cutfeat]>cutval
		*/
		void planeSplit(int* ind, int count, int cutfeat, ElementType cutval, int& lim1, int& lim2)
		{
			int left = 0;
			int right = count - 1;
			for (;; ) {
				while (left <= right && dataset[ind[left]][cutfeat]<cutval) ++left;
				while (left <= right && dataset[ind[right]][cutfeat] >= cutval) --right;
				if (left>right) break;
				std::swap(ind[left], ind[right]); ++left; --right;
			}

			lim1 = left;
			right = count - 1;
			for (;; ) {
				while (left <= right && dataset[ind[left]][cutfeat] <= cutval) ++left;
				while (left <= right && dataset[ind[right]][cutfeat]>cutval) --right;
				if (left>right) break;
				std::swap(ind[left], ind[right]); ++left; --right;
			}
			lim2 = left;
		}
		ElementType computeInitialDistances(const ElementType* vec_, std::vector<ElementType>& dists_) const
		{
			ElementType distsq = 0.0;

			for (size_t i = 0; i < veclen; ++i) {
				if (vec_[i] < root_bbox[i].low) {
					dists_[i] = (vec_[i] - root_bbox[i].low)*(vec_[i] - root_bbox[i].low);
					distsq += dists_[i];
				}
				if (vec_[i] > root_bbox[i].high) {
					dists_[i] = (vec_[i] - root_bbox[i].high)*(vec_[i] - root_bbox[i].high);
					distsq += dists_[i];
				}
			}

			return distsq;
		}

	public:
		
		void findNeighbors(ResultSet<ElementType>& result_set_, const ElementType* vec_, const TreeParams& params_) const
		{
			float epsError = 1 + params_.eps;

			std::vector<ElementType> dists(veclen, 0);
			ElementType distsq = computeInitialDistances(vec_, dists);

			searchLevel(result_set_, vec_, root_node, distsq, dists, epsError);

		}
		
	private:

		/**
		* Performs an exact search in the tree starting from a node.
		*/
		void searchLevel(ResultSet<ElementType>& result_set_, const ElementType* vec_, const NodePtr node_, ElementType mindistsq_,
			std::vector<ElementType>& dists_, const float epsError_) const
		{
			/* If this is a leaf node, then do check and return. */
			if ((node_->child1 == NULL) && (node_->child2 == NULL)) {
				ElementType worst_dist = result_set_.worstDist();
				for (int i = node_->left; i<node_->right; ++i) {	
					ElementType* point = ordered ? dataset_ordered[i] : dataset[vind[i]];
					ElementType dist = 0;
					for (int j = 0; j < veclen; j++) {
						dist += (vec_[j] - point[j])*(vec_[j] - point[j]);
					}

					if (dist<worst_dist) {
						result_set_.addPoint(dist, vind[i]);
					}
				}
				return;
			}

			/* Which child branch should be taken first? */
			int idx = node_->divfeat;
			ElementType val = vec_[idx];
			ElementType diff1 = val - node_->divlow;
			ElementType diff2 = val - node_->divhigh;

			NodePtr bestChild;
			NodePtr otherChild;
			ElementType cut_dist;
			if ((diff1 + diff2)<0) {
				bestChild = node_->child1;
				otherChild = node_->child2;
				cut_dist = (val-node_->divhigh)*(val-node_->divhigh);
			}
			else {
				bestChild = node_->child2;
				otherChild = node_->child1;
				cut_dist = (val - node_->divlow)*(val - node_->divlow);
			}

			/* Call recursively to search next level down. */
			searchLevel(result_set_, vec_, bestChild, mindistsq_, dists_, epsError_);

			ElementType dst = dists_[idx];
			mindistsq_ = mindistsq_ + cut_dist - dst;
			dists_[idx] = cut_dist;
			if (mindistsq_*epsError_ <= result_set_.worstDist()) {
				searchLevel(result_set_, vec_, otherChild, mindistsq_, dists_, epsError_);
			}
			dists_[idx] = dst;
		}

	private:

		/**
			Number of used neigbors in a node
		*/
		int neighbor;

		/**
			BoundingBox root_bbox;
		*/
		BoundingBox root_bbox;

		/**
			Pointer to the root node of the dataset
		*/
		NodePtr root_node;

		/**
			Array of indices to vectors in the dataset.
		*/
		std::vector<int> vind;
		
		/**
			Pooled memory allocator.
		*/
		PooledAllocator pool;

		/**
			Flag ordered pointcloud
		*/
		bool ordered;

		/**
			Ordered pointcloud
		*/
		Matrix<ElementType> dataset_ordered;

	};

}

#endif /* TREES_KDTREE_INDEX_H_ */