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

#ifndef PCSIMP_ALL_INDICES_H_
#define PCSIMP_ALL_INDICES_H_

#include "algorithms/nn_index.h"
#include "algorithms/mls_index.h"

#include "tools/utils/matrix.h"

namespace pcsimp
{

	template<template<typename> typename Index, typename ElementType>
	inline NNIndex<ElementType>* createIndex(const utils::Matrix<ElementType>& dataset_, const IndexParams& params_)
	{
		return new Index<ElementType>(dataset_, params_);
	}

	template<typename ElementType>
	inline NNIndex<ElementType>* createIndexByType(const pcsimpAlgorithm indexType_,
		const utils::Matrix<ElementType>& dataset_, const IndexParams& params_)
	{
		NNIndex<ElementType>* nnIndex;

		switch (indexType_) {
		case PCSIMP_INDEX_MLS:
			nnIndex = createIndex<MLSIndex, ElementType>(dataset_, params_);
			break;
		}

		return nnIndex;
	}
}

#endif /* PCSIMP_ALL_INDICES_H_ */