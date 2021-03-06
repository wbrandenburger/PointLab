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

#ifndef TREES_PARAMS_H_
#define TREES_PARAMS_H_

#include <map>
#include <assert.h>

#include "trees/defines.h"

#include "tools/utils.h"

namespace trees
{
	typedef std::map<std::string, utils::Any> IndexParams;

	/**
		Structure which holds tree parameters
	*/
	struct TreeParams {
		
		/**
			Constructor
		*/
		TreeParams() : 
			cores_(1),
			eps_(std::numeric_limits<float>::epsilon())
		{
		}

		/**
			Constructor

			@param[in] cores_ Number of cores
			@param[in] eps_  Machine epsilon
		*/
		TreeParams(size_t cores, float eps = std::numeric_limits<float>::epsilon()) : TreeParams() 
		{
			cores_ = cores;
			eps_ = eps;
		}

		/** 
			Set number of cores
			
			@param[in] cores Number of cores
		*/
		void setCores(size_t cores)
		{
			cores_ = cores;
		}
		
		/**
			Set epsilon
			
			@param[in] Accuracy
		*/
		void setEpsilon(float eps)
		{
			eps_ = eps;
		}
		
		/** 
			Get number of cores
			
			@return cores Number of cores
		*/
		size_t getCores() const
		{
			return cores_;
		}
		
		/**
			Get epsilon
			
			@return Accuracy
		*/
		float getEpsilon() const
		{
			return eps_;
		}	
		
		/**
			Number of cores
		*/
		size_t cores_;

		/**
			Machine epsilon
		*/
		float eps_;
	};

	/**
		Figure out whether the map has a certain field

		@param[in] params_ Map with fields
		@params[in] name_ Field which will be searched in the map
		@return Returns true if the field is found in the map
	*/
	inline bool has_param(const IndexParams& params_, std::string name_)
	{
		return params_.find(name_) != params_.end();
	}

	/**
		Returns the value of a certain field in a map

		@param[in] params_ Map with fields
		@param[in] name_ Field which will be searched in the map
		@return Returns the value of the found field 
	*/
	template<typename T>
	T get_param(const IndexParams& params_, std::string name_)
	{
		IndexParams::const_iterator it = params_.find(name_);
		if (it != params_.end()) {
			return it->second.cast<T>();
		}
		else {
			assert(it != params_.end());
		}
	}

	/**
		Returns the value of a certain field in a map

		@param[in] params_ Map with fields
		@param[in] name_ Field which will be searched in the map
		@param[in] default_value_ Returns a default value if there is no field in the map
		@return Returns the value of the found field or the default value
	*/
	template<typename T>
	T get_param(const IndexParams& params_, std::string name_, const T& default_value_)
	{
		IndexParams::const_iterator it = params_.find(name_);
		if (it != params_.end()) {
			return it->second.cast<T>();
		}
		else {
			return default_value_;
		}
	}
}

#endif /* PCSIMP_UTILS_PARAMS_H_ */
