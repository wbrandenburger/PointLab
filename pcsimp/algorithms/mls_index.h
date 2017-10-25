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

#ifndef PCSIMP_MLS_INDEX_H_
#define PCSIMP_MLS_INDEX_H_

#include "defines.h"
#include "algorithms/nn_index.h"

#include "tools/utils/matrix.h"


namespace pcsimp
{

	struct MLSIndexParams : public IndexParams
	{
		/**
			Constructor

			@param neighbor_ number of used neighbors for computing surface
			@param weight_ the weight of the furthest points when computing Least Squares
		*/
		MLSIndexParams(int neighbor_ = 30, float weight_ = 0.05f)
		{
			(*this)["algorithm"] = PCSIMP_INDEX_MLS;
			(*this)["neighbor"] = neighbor_;
			(*this)["weight"] = weight_;
		}
	};

	template<typename ElementType>
	class MLSIndex : public NNIndex<ElementType>
	{
	public:

		/**
			Constructor

			@param data_ pointcloud which sould be simplified
			@param params_ input parameters for the simplification process
		*/
		MLSIndex(const utils::Matrix<ElementType>& dataset_, const IndexParams& params_ = MLSIndexParams())
		{
			neighbor = get_param(params_, "neighbor", 30);
			weight = get_param(params_, "weight", 0.05f);
		}

	private:

		/**
			Number of used neigbors for computing surface
		*/
		int neighbor;

		/**
			The weight of the furthest points when computing Least Squares
		*/
		float weight;

	};

}

#endif /* PCSIMP_MLS_INDEX_H_ */