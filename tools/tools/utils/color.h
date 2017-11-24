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

#ifndef UTILS_COLOR_H_
#define UTILS_COLOR_H_

namespace utils
{
	/**
		Computes a color scheme within the range of red and blue

		@param[in,out] r_ Red
		@param[in,out] g_ Green
		@param[in,out] b_ Blue
		@param[in,out] index_ Index
		@param[in,out] number_of_colors_ Number of Colors 
	*/
	void colorSchemeRGB(int& r_, int& g_, int& b_, size_t index_, size_t number_of_colors_) {
		
		float index = (float)number_of_colors_ / 2.0f;
		float increment = index_ > index ? 1.0f / std::ceil(index) : 1.0f / std::floor(index);

		if (index_ < index) {
			r_ =  255 - (int)(255.0f * increment * (float) index_);
			g_ = (int)(255.0f * increment * (float) index_);
			b_ = 0;
		}
		else {
			r_ = 0;
			g_ =  255 - (int)(255.0f * increment * (float) index_);
			b_ = (int)(255.0f * increment * (float) index_);
		}
	}

	/**
		Computes a color scheme within the range of red and blue

		@param[in,out] r_ Red
		@param[in,out] g_ Green
		@param[in,out] b_ Blue
		@param[in,out] index_ Index
		@param[in,out] number_of_colors_ Number of Colors
	*/
	void colorSchemeRGB(float& r_, float& g_, float& b_, size_t index_, size_t number_of_colors_) {

		float index = std::ceil((float)number_of_colors_ / 2.0f);
		float increment = index_ > index ? 1.0f / std::ceil(index) : 1.0f / std::floor(index);

		if (index_ < index) {
			r_ = 1.0f - increment * (float) index_;
			g_ = increment * (float) index_;
			b_ = 0.0f;
		}
		else {
			r_ = 0.0f;
			g_ = 1.0f - increment * (float)index_;
			b_ = increment * (float) index_;
		}
	}

	/**
		Computes a color scheme within the range of red and blue

		@param[in,out] r_ Red
		@param[in,out] g_ Green
		@param[in,out] b_ Blue
		@param[in,out] index_ Index
		@param[in,out] number_of_colors_ Number of Colors 
	*/
	void colorSchemeRG(int& r_, int& g_, int& b_, size_t index_, size_t number_of_colors_) {
		
		float increment =  1.0f / (float) number_of_colors_;

		r_ =  255 - (int)(255.0f * increment * (float) index_);
		g_ = (int)(255.0f * increment * (float) index_);
		b_ = 0;
	}

	/**
		Computes a color scheme within the range of red and blue

		@param[in,out] r_ Red
		@param[in,out] g_ Green
		@param[in,out] b_ Blue
		@param[in,out] index_ Index
		@param[in,out] number_of_colors_ Number of Colors
	*/
	void colorSchemeRG(float& r_, float& g_, float& b_, size_t index_, size_t number_of_colors_) {

		float increment = 1.0f / (float) number_of_colors_;

		r_ = 1.0f - increment * (float) index_;
		g_ = increment * (float) index_;
		b_ = 0.0f;
	}

}
#endif /* UTILS_COLOR_H_ */