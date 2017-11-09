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

#ifndef TREES_KDTREE_INDEX_H_
#define TREES_KDTREE_INDEX_H_

#include <vector>

#include "trees/defines.h"
#include "trees/algorithms/nn_index.h"

#include "tools/utils.h"

#include "trees/utils/params.h"
#include "trees/utils/result_set.h"

namespace trees
{

	/**
		Input parameters for the tree
	*/
	struct KDTreeIndexParams : public IndexParams
	{
		/**
			Constructor

			@param[in] neighbor_ Maximal number  of neighbors in one node
			@param[in] ordered_ Flag whether th epointcloud shall be sorted
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

			@param[in] dataset_ Pointcloud
			@param[in] params_ Input parameters for the tree
		*/
		KDTreeIndex(const utils::Matrix<ElementType>& dataset_, const IndexParams& params_ = KDTreeIndexParams()) : root_node(nullptr), dataset_nodes(nullptr)
		{
			neighbor = get_param(params_, "neighbor", 30);
			ordered = get_param(params_, "ordered", true);

			setDataset(dataset_);

			dataset_points = utils::Matrix<ElementType>(new ElementType[size*veclen], size, veclen);
			std::copy(dataset[0], dataset[0] + size*veclen, dataset_points[0]);
		}
	
	private:

		/**
			Structures for the bounding box
		*/
		struct Interval
		{
			ElementType low, high;
		};

		typedef std::vector<Interval> BoundingBox;
		
		/**
			Structure for a node in the tree
		*/
		struct Node
		{
			/**
				Constructor
			*/
			Node() : points(NULL), divfeat(NULL), divlow(NULL), divhigh(NULL),
				indices(nullptr), child1(nullptr), child2(nullptr), parent(nullptr) {}

			/**
				Destructor
			*/
			~Node()
			{
				if (child1) child1->~Node();
				if (child2) child2->~Node();

				if (indices) { delete[] indices; }
			}
			/**
				Removes child

				@param[in] node_ The node which call the removing process
			*/
			void removeChild(Node* node_)
			{
				if (child1 == node_) { child1 = nullptr; }
				if (child2 == node_) { child2 = nullptr; }

				if (!child1 && !child2) {
					if (parent) { parent->removeChild(this); }
				}
			}


			/**
				Figure out the number of points

				@param[in,out] count_ Number of points
			*/
			void count(size_t& count_)
			{
				if (child1) { child1->count(count_); }
				if (child2) { child2->count(count_); }

				if (points) {
					count_ += points;
				}
			}

			/**
				Indices of points in leaf node
			*/
			size_t* indices;

			/**
				Number of points
			*/
			size_t points;
			/**
				Dimension used for subdivision
			*/
			int divfeat;
			/**
				The values used for subdivision
			*/
			ElementType divlow, divhigh;
			/**
				The child nodes
			*/
			Node* child1, *child2;
			/**
				Parent
			*/
			Node* parent;

		};

		typedef Node* NodePtr;

		/**
			Free allocated memory
		*/
		void freeIndex()
		{
			if (dataset_nodes) {
				delete[] dataset_nodes;
				dataset_nodes = nullptr;
			}
			dataset_points.clearMemory();
			if (root_node) { root_node->~Node(); }
			pool.clear();
		}

		/**
			Get the dataset
		*/
		void getDataset(utils::Matrix<ElementType>& dataset_)
		{
			dataset_ = dataset_points;
		}

		/**
			Prepares the building processs of the tree and calls the initial divide function
		*/
		void buildIndexImpl()
		{
			// Create a permutable array of indices to the input vectors.
			vind.resize(size);
			for (size_t i = 0; i < size; i++) {
				vind[i] = i;
			}

			dataset_nodes = new NodePtr[size];

			computeBoundingBox(root_bbox);
			root_node = divideTree(nullptr, 0, size, root_bbox);
			
			if (ordered) {
				utils::Matrix<ElementType> dataset_points_temp = utils::Matrix<ElementType>(new ElementType[size*veclen], size, veclen);
				for (size_t i = 0; i < size; ++i) {
					std::copy(dataset_points[vind[i]], dataset_points[vind[i]] + veclen, dataset_points_temp[i]);
				}
				dataset_points = dataset_points_temp;

				NodePtr* dataset_nodes_temp = new NodePtr[size];
				for (size_t i = 0; i < size; ++i) {
					dataset_nodes_temp[i] = dataset_nodes[vind[i]];
				}
				delete[] dataset_nodes;
				dataset_nodes = dataset_nodes_temp;
			}
		}

		/**
			Free allocated memory for build process
		*/
		void freeBuild() 
		{
			if (dataset_nodes) {
				delete[] dataset_nodes;
				dataset_nodes = nullptr;
			}
			if (root_node) { root_node->~Node(); }
			pool.clear();
		}

		/**
			Rebuilds the index

			@param[in] dataset_ Pointcloud
		*/
		void rebuild(const utils::Matrix<ElementType>& dataset_)
		{
			setDataset(dataset_);

			dataset_points.setMatrix(new ElementType[size*veclen], size, veclen);
			std::copy(dataset[0], dataset[0] + size*veclen, dataset_points[0]);

			buildIndex();
		}


		/**
			Computes the bounding box of the entire pointcloud

			@param[in,out] bbox_ Structure which contain the bounds of the bounding box
		*/
		void computeBoundingBox(BoundingBox& bbox_)
		{
			bbox_.resize(veclen);
			for (size_t i = 0; i<veclen; ++i) {
				bbox_[i].low = (ElementType)dataset_points[0][i];
				bbox_[i].high = (ElementType)dataset_points[0][i];
			}
			for (size_t k = 1; k<size; ++k) {
				for (size_t i = 0; i<veclen; ++i) {
					if (dataset_points[k][i]<bbox_[i].low) bbox_[i].low = (ElementType)dataset_points[k][i];
					if (dataset_points[k][i]>bbox_[i].high) bbox_[i].high = (ElementType)dataset_points[k][i];
				}
			}
		}


		/**
			Divides the points  between left_ and right_ and calls revursively divideTree or
			creates the leaf node with the respective points

			@param[in] left_ Left bound of the list which shall be split
			@param[in] right_ Right bound of the list which shall be split
			@param[in] bbox_ Bounding Box of the entire pointcloud
			@return Pointer to a node of the tree
		*/
		NodePtr divideTree(NodePtr parent_, int left_, int right_, BoundingBox& bbox_)
		{
			NodePtr node = new (pool) Node; // allocate memory
			node->parent = parent_;
											   /* If too few exemplars remain, then make this a leaf node. */
			if ((right_ - left_) <= neighbor) {
				node->child1 = node->child2 = nullptr;    /* Mark as leaf node. */
				
				node->points = right_ - left_;
				node->indices= new size_t[node->points];
				for (size_t i = left_; i < right_; i++) {
					node->indices[i - left_] = i;

					dataset_nodes[vind[i]] = node;
				}

				// compute bounding-box of leaf points
				for (size_t i = 0; i<veclen; ++i) {
					bbox_[i].low = (ElementType)dataset_points[vind[left_]][i];
					bbox_[i].high = (ElementType)dataset_points[vind[left_]][i];
				}
				for (int k = left_ + 1; k<right_; ++k) {
					for (size_t i = 0; i<veclen; ++i) {
						if (bbox_[i].low>dataset_points[vind[k]][i]) bbox_[i].low = (ElementType)dataset_points[vind[k]][i];
						if (bbox_[i].high<dataset_points[vind[k]][i]) bbox_[i].high = (ElementType)dataset_points[vind[k]][i];
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
				node->child1 = divideTree(node, left_, left_ + idx, left_bbox);

				BoundingBox right_bbox(bbox_);
				right_bbox[cutfeat].low = cutval;
				node->child2 = divideTree(node, left_ + idx, right_, right_bbox);

				node->divlow = left_bbox[cutfeat].high;
				node->divhigh = right_bbox[cutfeat].low;

				for (size_t i = 0; i<veclen; ++i) {
					bbox_[i].low = std::min(left_bbox[i].low, right_bbox[i].low);
					bbox_[i].high = std::max(left_bbox[i].high, right_bbox[i].high);
				}
			}

			return node;
		}

		/**
			Computes the minimal and maximal value of one dimension in a list of the pointcloud

			@param[in] ind_ Pointer to the list
			@param[in] count_ Number of elements in the list which shall be included
			@param[in] dim_ Dimension of pointcloud which shall be examined
			@param[in,out] min_elem_ Minimal value
			@param[in,out] max_elem_ Maximal value

		*/
		void computeMinMax(size_t* ind_, int count_, size_t dim_, ElementType& min_elem_, ElementType& max_elem_)
		{
			min_elem_ = dataset_points[ind_[0]][dim_];
			max_elem_ = dataset_points[ind_[0]][dim_];
			for (int i = 1; i<count_; ++i) {
				ElementType val = dataset_points[ind_[i]][dim_];
				if (val<min_elem_) min_elem_ = val;
				if (val>max_elem_) max_elem_ = val;
			}
		}

		/**
			Computes the index, dimension and value of the point where the list will be split
			
			@param[in] ind_ Pointer to the list
			@param[in] count_ Number of elements in the list which shall be included
			@param[in,out] index_ Index of the point where the list will be split
			@param[in,out] cutfeat_ Dimension of the point where the list will be split
			@param[in,out] cutval_ Value of the point where the list will be split
			@param[in] bbox_ Bounding Box of the entire pointcloud
		*/
		void middleSplit(size_t* ind_, int count_, int& index_, int& cutfeat_, ElementType& cutval_, const BoundingBox& bbox_)
		{
			// find the largest span from the approximate bounding box
			ElementType max_span = bbox_[0].high - bbox_[0].low;
			cutfeat_ = 0;
			cutval_ = (bbox_[0].high + bbox_[0].low) / 2;
			for (size_t i = 1; i<veclen; ++i) {
				ElementType span = bbox_[i].high - bbox_[i].low;
				if (span>max_span) {
					max_span = span;
					cutfeat_ = i;
					cutval_ = (bbox_[i].high + bbox_[i].low) / 2;
				}
			}

			// compute exact span on the found dimension
			ElementType min_elem, max_elem;
			computeMinMax(ind_, count_, cutfeat_, min_elem, max_elem);
			cutval_ = (min_elem + max_elem) / 2;
			max_span = max_elem - min_elem;

			// check if a dimension of a largest span exists
			size_t k = cutfeat_;
			for (size_t i = 0; i<veclen; ++i) {
				if (i == k) continue;
				ElementType span = bbox_[i].high - bbox_[i].low;
				if (span>max_span) {
					computeMinMax(ind_, count_, i, min_elem, max_elem);
					span = max_elem - min_elem;
					if (span>max_span) {
						max_span = span;
						cutfeat_ = (int) i;
						cutval_ = (min_elem + max_elem) / 2;
					}
				}
			}
			int lim1, lim2;
			planeSplit(ind_, count_, cutfeat_, cutval_, lim1, lim2);

			if (lim1>count_ / 2) index_ = lim1;
			else if (lim2<count_ / 2) index_ = lim2;
			else index_ = count_ / 2;
			
			if (index_ < 0 && index_ > count_) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}

		/**
			Sorts the list in elements which are smaller than cutval_ and greater than cutval_

			@param[in] ind_ Pointer to the list
			@param[in] count_ Number of elements in the list which shall be included
			@param[in] cutfeat_ Dimension of the point where the list will be split
			@param[in,out] lim1_ Left index which is the split index
			@param[in,out] lim2_ Right index which is the plit index
		*/
		void planeSplit(size_t* ind_, int count_, int cutfeat_, ElementType cutval_, int& lim1_, int& lim2_)
		{
			int left = 0;
			int right = count_ - 1;
			for (;; ) {
				while (left <= right && dataset_points[ind_[left]][cutfeat_]<cutval_) ++left;
				while (left <= right && dataset_points[ind_[right]][cutfeat_] >= cutval_) --right;
				if (left>right) break;
				std::swap(ind_[left], ind_[right]); ++left; --right;
			}

			lim1_ = left;
			right = count_ - 1;
			for (;; ) {
				while (left <= right && dataset_points[ind_[left]][cutfeat_] <= cutval_) ++left;
				while (left <= right && dataset_points[ind_[right]][cutfeat_]>cutval_) --right;
				if (left>right) break;
				std::swap(ind_[left], ind_[right]); ++left; --right;
			}
			lim2_ = left;
		}

		/**
			Removes point from kdtree

			@param[in] index_ Index of the point in the pointcloud
			@return True when removing of point was successful
		*/
		bool remove(size_t index_)
		{
			if (index_ >= size) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			bool flag = false;

			size_t index = ordered ? vind[index_] : index_;
			NodePtr node = dataset_nodes[index];

			if (node)
			{
				for (size_t i = 0; i < node->points; i++){
					if (node->indices[i] == index) {
						if (i != node->points - 1) {
							std::copy(node->indices + i + 1, node->indices + node->points, node->indices + i);
						}
						node->points--;

						flag = true;
					}
				}
			
				if (!node->points) {
					node->parent->removeChild(node);
				}

				dataset_nodes[index] = nullptr;
			}

			return flag;
		}

	public:
		
		/**
			Prepares the search process, computes initial distances and calls the search function

			@param[in,out] result_set_ Container which contains the found neighbors
			@param[in] vec_ Point which neighbors shall be found
			@param[in] params_ Input parameters for the search
		*/
		void findNeighbors(ResultSet<ElementType>& result_set_, const ElementType* vec_, const TreeParams& params_) const
		{
			float epsError = 1 + params_.eps;

			std::vector<ElementType> dists(veclen, 0);
			ElementType distsq = computeInitialDistances(vec_, dists);

			searchLevel(result_set_, vec_, root_node, distsq, dists, epsError);

		}
		
	private:

		/**
			Computes an initial value for mindistsq_

			@param[in] vec_ Point which neighbors shall be found
			@param[in,out] dists
			@return Initial value for mindistsq_
		*/
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

		/**
			Performs an exact search in the tree starting from a node.

			@param[in,out] result_set_ Container which contains the found neighbors
			@param[in] vec_ Point which neighbors shall be found
			@param[in] node_ Node which will be examined
			@param[in] mindistsq_ The distance of a node to vec_
			@param[in,out] dists_ The distances of a node in the certain dimensions to vec_
			@param[in] epsError_ Error value

		*/
		void searchLevel(ResultSet<ElementType>& result_set_, const ElementType* vec_, const NodePtr node_, ElementType mindistsq_,
			std::vector<ElementType>& dists_, const float epsError_) const
		{
			/* If this is a leaf node, then do check and return. */
			if ((node_->child1 == nullptr) && (node_->child2 == nullptr)) {
				ElementType worst_dist = result_set_.worstDist();
				for (int i = 0; i<node_->points; ++i) {	
					ElementType* point = ordered ? dataset_points[node_->indices[i]] : dataset_points[vind[node_->indices[i]]];
					
					ElementType dist = distance(const_cast<ElementType*>(vec_), point, veclen);
					if (dist<worst_dist) {
						result_set_.addPoint(dist, vind[node_->indices[i]]);
					}
				}
				return;
			}

			/* Which child branch should be taken first? */
			int idx = node_->divfeat;
			ElementType val = vec_[idx];
			ElementType diff1 = val - node_->divlow;
			ElementType diff2 = val - node_->divhigh;

			NodePtr best_child;
			NodePtr other_child;
			ElementType cut_dist;
			if ((diff1 + diff2)<0) {
				best_child = node_->child1;
				other_child = node_->child2;
				cut_dist = distance(val, node_->divhigh);
			}
			else {
				best_child = node_->child2;
				other_child = node_->child1;
				cut_dist = distance(val, node_->divlow); 
			}

			/* Call recursively to search next level down. */
			if (best_child) {
				searchLevel(result_set_, vec_, best_child, mindistsq_, dists_, epsError_);
			}

			if (other_child) {
				ElementType dst = dists_[idx];
				mindistsq_ = mindistsq_ + cut_dist - dst;
				dists_[idx] = cut_dist;
				if (mindistsq_*epsError_ <= result_set_.worstDist()) {
					searchLevel(result_set_, vec_, other_child, mindistsq_, dists_, epsError_);
				}
				dists_[idx] = dst;
			}
		}

	private:

		/**
			Maximal number of neighbors in a node
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
		std::vector<size_t> vind;
		
		/**
			Pooled memory allocator.
		*/
		utils::PooledAllocator pool;

		/**
			Flag ordered pointcloud
		*/
		bool ordered;

		/**
			Pointcloud
		*/
		utils::Matrix<ElementType> dataset_points;

		/**
			List with the indices to nodes
		*/
		NodePtr* dataset_nodes;

		/**
			Distance structure
		*/
		utils::L2<ElementType> distance;
	};

}

#endif /* TREES_KDTREE_INDEX_H_ */