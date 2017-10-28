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

#ifndef MATH_ZERO_H_
#define MATH_ZERO_H_

namespace math
{
	/**
		Swap Elements

		@param[in,out] a_ First element
		@param[in,out] b_ Second element
	*/
	template<typename ElementType> inline void swapElement(ElementType& a_, ElementType& b_)
	{
		ElementType temp = a_;
		a_ = b_;
		b_ = temp;
	}

	template<typename ElementType> struct NewtonMethod
	{
		/**
			Operator () Computes a zero point bewteen  a given interval

			@param[in] function_ Structure with overloaded operator ()
			@param[in] a_ Left or right interval bound
			@param[in] b_ Left or rigth interval bound
			@param[in] eps_ Value which determines the acurracy
		*/
		template<typename FunctionHandle>
		ElementType operator() (FunctionHandle function_, ElementType a_, ElementType b_, ElementType eps_)
		{
			if (std::abs(a_) < std::abs(b_)) {
				swapElement(a_, b_);
			}
			
			while (std::abs(function_(b_)) > eps_ && std::abs(function_(a_)) > eps_) {
				/**
					Newton method x_n+1 = x_n - f(x_n)/f'(x_n)
				*/
				ElementType s = b_ - function_(b_)*(b_ - a_) / (function_(b_) - function_(a_));
				a_ = b_;
				b_ = s;
			}

			return b_;
		}
	};

	template<typename ElementType> struct QuadraticInverseInterpolation
	{	
		/**
			Operator () Computes a zero point bewteen  a given interval

			@param[in] function_ Structure with overloaded operator ()
			@param[in] a_ Left or right interval bound
			@param[in] b_ Left or rigth interval bound
			@param[in] eps_ Value which determines the acurracy
		*/
		template<typename FunctionHandle> 
		ElementType operator()(FunctionHandle function_, ElementType a_, ElementType b_, ElementType eps_)
		{
			if (std::abs(a_) < std::abs(b_)) {
				swapElement(a_, b_);
			}

			/**
				Newton method x_n+1 = x_n - f(x_n)/f'(x_n) to determine a third supporting point 
			*/
			ElementType s = b_ - function_(b_)*(b_ - a_) / (function_(b_) - function_(a_));
			ElementType c = b_;
			
			/**
				If a and s are on different sides the inverse interpolationpolynom are used and
				if they are on the same side newton method are used. This is because the newton method
				creates a fictive line with a zero point whereas the inverse interpolationspolynom interpolates only
				through the three supporting points
			*/
			if (function_(a_)*function_(s) < 0) {
				b_ = s;
			}
			else {
				a_ = s;
			}

			if (std::abs(a_) < std::abs(b_)) {
				swapElement(a_, b_);
			}

			while (std::abs(function_(b_)) > eps_ && std::abs(function_(a_)) > eps_) {
				if (function_(a_) != function_(c) && function_(b_) != function_(c)) {
					/**
						Inverse interpolationpolynom with y = 0 and a, b and c as supporting points
					*/
					s = a_*function_(b_)*function_(c) / ((function_(a_) - function_(b_))*(function_(a_) - function_(c))) +
						b_*function_(a_)*function_(c) / ((function_(b_) - function_(a_))*(function_(b_) - function_(c))) +
						c*function_(a_)*function_(b_) / ((function_(c) - function_(a_))*(function_(c) - function_(b_)));
				}
				else { 
					s = b_ - function_(b_)*(b_ - a_) / (function_(b_) - function_(a_));
				}
				c = b_;
				/**
					If a and s are on different sides the inverse interpolationpolynom are used and
					if they are on the same side newton method are used. This is because the newton method 
					creates a fictive line with a zero point whereas the inverse interpolationspolynom interpolates only
					through the three supporting points
				*/
				if (function_(a_)*function_(s) < 0) {
					b_ = s;
				}
				else {
					a_ = s;
				}

				if (std::abs(a_) < std::abs(b_)) {
					swapElement(a_, b_);
				}
			}

			return std::abs(function_(b_)) < eps_ ? b_ : a_;
		}
	};
}

#endif /* MATH_ZERO_H_ */