/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017	Wolfgang Brandenburger
*					(w.brandenburger@unibw.de).
*					All rights reserved.
* Copyright 2008-2009  Marius Muja (mariusm@cs.ubc.ca). All rights reserved.
* Copyright 2008-2009  David G. Lowe (lowe@cs.ubc.ca). All rights reserved.
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

#ifndef TREES_ALL_INDICES_H_
#define TREES_ALL_INDICES_H_

#include "trees/algorithms/nn_index.h"
#include "trees/algorithms/kdtree_index.h"

#include "tools/utils.h"

namespace trees
{
	/**
		Create a pointer of a specified indextype

		@param[in] dataset_ Pointcloud
		@param[in] params_ Input parameters for the tree
		@return Returns a pointer of the created index
	*/
	template<template<typename> typename Index, typename ElementType>
	inline NNIndex<ElementType>* createIndex(const utils::Matrix<ElementType>& dataset_, const IndexParams& params_)
	{
		return new Index<ElementType>(dataset_, params_);
	}

	/**
		Create a pointer of a specified indextype

		@param[in] indexType_ Type of the index which should be build
		@param[in] dataset_ Pointcloud
		@param[in] params_ Input parameters for the tree
		@return Returns a pointer of the created index
	*/
	template<typename ElementType>
	inline NNIndex<ElementType>* createIndexByType(const treeIndex indexType_,
		const utils::Matrix<ElementType>& dataset_, const IndexParams& params_)
	{
		NNIndex<ElementType>* nnIndex;

		switch (indexType_) {
		case TREE_INDEX_KDTREE:
			nnIndex = createIndex<KDTreeIndex, ElementType>(dataset_, params_);
			break;
		}

		return nnIndex;
	}
}

#endif /* TREES_ALL_INDICES_H_ */
