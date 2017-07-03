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

#ifndef UTILS_NODES_H_
#define UTILS_NODES_H_

namespace utils
{

	/**
		KdTree-Node-Structure
	*/
	template <typename ElementType>
	struct KdTreeNode {
		/**
			Dimension used for subdivision
		*/
		int divfeat;
		
		/**
			Value used for subdivision
		*/
		ElementType divval;
		
		/**
			The indices to the child nodes.
		*/
		int child1;
		int child2;
		
		/**
			Constructor
		*/
		__host__
		__device__
		KdTreeNode() {
			child1 = NULL;
			child2 = NULL;
		}
	};

	template <typename DistanceType>
	struct Branch {

		int nodeIdx;
		DistanceType mindist;
		DistanceType* dists;
		int idx;

		__host__
		__device__
			Branch() : nodeIdx(NULL), mindist(NULL), dists(nullptr), idx(0) {}

		__host__
		__device__
		Branch(int nodeIdx_, DistanceType mindist_) : nodeIdx(nodeIdx_), mindist(mindist_), dists(nullptr), idx(0) {}

		__host__
		__device__
			Branch(int nodeIdx_, DistanceType mindist_, DistanceType* dists_, int idx_) : nodeIdx(nodeIdx_),
			mindist(mindist_), dists(dists_), idx(idx_) {}

		__host__
		__device__
			void clear()
		{
			delete[] dists;
		}

		__host__
		__device__
			bool operator<(const Branch<DistanceType> element_) const
		{
			return mindist < element_.mindist;
		}

		__host__
		__device__
			bool operator>(const Branch<DistanceType> element_) const
		{
			return mindist > element_.mindist;
		}

		__host__
		__device__
			bool operator==(DistanceType value_) const
		{
			return mindist == value_;
		}

		__host__
		__device__
			bool operator!=(DistanceType value_) const
		{
			return mindist != value_;
		}

		__host__
		__device__
			bool operator!() const
		{
			return !mindist;
		}

		__host__
		__device__
			void operator=(DistanceType value_)
		{
			mindist = value_;
		}
	};
}

#endif /* UTILS_NODES_H_*/