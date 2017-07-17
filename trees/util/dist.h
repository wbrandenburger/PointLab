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

#ifndef TREES_DIST_H_
#define TREES_DIST_H_

namespace trees
{

	/**
		Structure which computes L1-Distances
	*/
	template<typename ElementType> struct L1 {

		/**
			Operator () which computes the L1-Distance between two points

			@param a_ Pointer to the first point
			@param b_ Pointer to the second point
			@param dim_ Number of dimensions
		*/
		ElementType operator()(ElementType* a_, ElementType* b_, size_t dim_) const
		{
			ElementType result = ElementType();
			ElementType diff0, diff1, diff2;

			ElementType* last = a_ + dim_;
			ElementType* lastgroup = last - 3;

			while (a_ < lastgroup) {
				diff0 = std::abs(a_[0] - b_[0]);
				diff1 = std::abs(a_[1] - b_[1]);
				diff2 = std::abs(a_[2] - b_[2]);

				result += diff0 + diff1 + diff2;

				a_ += 3;
				b_ += 3;
			}

			while (a_ < last) {
				diff0 = std::abs(*a_++ - *b_++);
				result += diff0;
			}

			return result;
		}

		/**
			Operator () which computes the distance of one dimension between two points
	
			@param a_ Value of the first point
			@param b Value of the second point
		*/
		ElementType operator()(const ElementType& a_, const ElementType& b_) const {

			return std::abs(a_ - b_);
		}

	};

	/**
		Structure which computes unrolled L2-Distances 
	*/
	template<typename ElementType> struct L2Simple
	{
	
		/**
			Operator () which computes the L2-Distance between two points

			@param a_ Pointer to the first point
			@param b_ Pointer to the second point
			@param dim_ Number of dimensions
		*/
		ElementType operator()(ElementType* a_, ElementType* b_, size_t dim_) const
		{
			ElementType result = ElementType();
			ElementType diff;
			for (size_t i = 0; i < dim_; i++) {
				diff = *a_++-*b_++:
				result += diff*diff;
			}

			return result;
		}

		/**
			Operator () which computes the distance of one dimension between two points

			@param a_ Value of the first point
			@param b Value of the second point		
		*/
		ElementType operator()(const ElementType& a_, const ElementType& b_) const {
			
			return (a_ - b_)*(a_ - b_);
		}
	
	};

	/**
		Structure which computes L2-Distances
	*/
	template<typename ElementType> struct L2 {

		/**
			Operator () which computes the L2-Distance between two points

			@param a_ Pointer to the first point
			@param b_ Pointer to the second point
			@param dim_ Number of dimensions
		*/
		ElementType operator()(ElementType* a_, ElementType* b_, size_t dim_) const
		{
			ElementType result = ElementType();
			ElementType diff0, diff1, diff2;

			ElementType* last = a_ + dim_;
			ElementType* lastgroup = last - 3;

			while (a_ < lastgroup) {
				diff0 = (a_[0] - b_[0]);
				diff1 = (a_[1] - b_[1]);
				diff2 = (a_[2] - b_[2]);

				result += diff0*diff0 + diff1*diff1 + diff2*diff2;

				a_ += 3;
				b_ += 3;
			}

			while (a_ < last) {
				diff0 = (*a_++ - *b_++);
				result += diff0*diff0;
			}

			return result;
		}

		/**
			Operator () which computes the distance of one dimension between two points

			@param a_ Value of the first point
			@param b Value of the second point
		*/
		ElementType operator()(const ElementType& a_, const ElementType& b_) const {

			return (a_ - b_)*(a_ - b_);
		}

	};

	/**
		Structure which computes the max distance
	*/
	template<typename ElementType> struct MaxDistance {

		/**
			Operator () which computes the max distance between two points

			@param a_ Pointer to the first point
			@param b_ Pointer to the second point
			@param dim_ Number of dimensions
		*/
		ElementType operator()(ElementType* a_, ElementType* b_, size_t dim_) const
		{
			ElementType result = ElementType();
			ElementType diff0, diff1, diff2;

			ElementType* last = a_ + dim_;
			ElementType* lastgroup = last - 3;

			while (a_ < lastgroup) {
				diff0 = std::abs(a_[0] - b_[0]);
				diff1 = std::abs(a_[1] - b_[1]);
				diff2 = std::abs(a_[2] - b_[2]);

				if (result < diff0) { result = diff0; }
				if (result < diff1) { result = diff1; }
				if (result < diff2) { result = diff2; }

				a_ += 3;
				b_ += 3;
			}

			while (a_ < last) {
				diff0 = std::abs(*a_++ - *b_++);
				if (result < diff0) { result = diff0; }
			}

			return result;
		}

	};

}

#endif /* TREES_DIST_H_ */