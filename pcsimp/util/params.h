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

#ifndef PCSIMP_UTILS_PARAMS_H_
#define PCSIMP_UTILS_PARAMS_H_

#include <map>

#include "tools/utils/any.h"

namespace pcsimp
{
	typedef std::map<std::string, utils::Any> IndexParams;

	struct SimpParams {

		SimpParams(int cores_ = 1, float eps_ = std::numeric_limits<float>::epsilon()) 
		{
			cores = cores_;
			eps = eps_;
		}

		//Number of cores
		int cores;

		//Machine epsilon
		float eps;
	};

	inline bool has_param(const IndexParams& params, std::string name)
	{
		return params.find(name) != params.end();
	}

	template<typename T>
	T get_param(const IndexParams& params, std::string name)
	{
		IndexParams::const_iterator it = params.find(name);
		if (it != params.end()) {
			return it->second.cast<T>();
		}
	}

	template<typename T>
	T get_param(const IndexParams& params, std::string name, const T& default_value)
	{
		IndexParams::const_iterator it = params.find(name);
		if (it != params.end()) {
			return it->second.cast<T>();
		}
		else {
			return default_value;
		}
	}
}

#endif /* PCSIMP_UTILS_PARAMS_H_ */