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

#include "tools/parameters.h"

#define REPETITION 1000

namespace math
{
	
	/**
		Operator () Computes a zero point bewteen  a given interval
	
		@param[in] function Structure with overloaded operator ()
		@param[in] a Left or right interval bound
		@param[in] b Left or rigth interval bound
		@param[in] eps Value which determines the acurracy
	*/
	template<typename ElementType, typename FunctionType> ElementType NewtonMethod(
		FunctionType function, 
		ElementType min, 
		ElementType max, 
		ElementType eps)
	{
		if (min > max) {
			swap(min, max);
		}

		size_t repetition = 0;
		while (std::abs(function(max)) > eps && std::abs(function(min)) > eps) 
		{
			/**
				Newton method x_n+1 = x_min + f(x_min)/f'(x_min) with f'(x_min) = (f(x_max) - f(x_min)/(x_max - x_min)
			*/
			ElementType s = min - function(min)*(max - min) / (function(max) - function(min));
			max = min;
			min = s;

			/**
				Abort if the computation of zero tooks too much repetitions
			*/
			repetition++;
			if (repetition > REPETITION) {
				return NULL;
			}
		}

		return function(min) < function(max) ? min : max;
	}


	/**
		Operator () Computes a zero point bewteen  a given interval
	
		@param[in] function Structure with overloaded operator ()
		@param[in] a Left or right interval bound
		@param[in] b Left or rigth interval bound
		@param[in] eps Value which determines the acurracy
	*/
	template<typename ElementType, typename FunctionType> ElementType QuadraticInverseInterpolation(
		FunctionType function, 
		ElementType a, 
		ElementType b, 
		ElementType eps)
	{
		if (b < a) {
			swap(a, b);
		}

		/**
			Newton method x_n+1 = x_n - f(x_n)/f'(x_n) with f'(x_n) = (f(x_2) - f(x_1)/(x_2 - x_1) to determine a third supporting point 
		*/
		ElementType s = b - function(b)*(a - b) / (function(a) - function(b));
		ElementType c = b;
		
		/**
			If a and s are on different sides the inverse interpolationpolynom are used and
			if they are on the same side newton method are used. This is because the newton method
			creates a fictive line with a zero point whereas the inverse interpolationspolynom interpolates only
			through the three supporting points
		*/
		if (function(a)*function(s) < 0) {
			b = s;
		}
		else {
			a = s;
		}

		if (std::abs(a) < std::abs(b)) {
			swap(a, b);
		}

		size_t repetition = 0;
		while (std::abs(function(b)) > eps && std::abs(function(a)) > eps) {
			if (function(a) != function(c) && function(b) != function(c)) {
				/**
					Inverse interpolationpolynom with y = 0 and a, b and c as supporting points
				*/
				s = a*function(b)*function(c) / ((function(a) - function(b))*(function(a) - function(c))) +
					b*function(a)*function(c) / ((function(b) - function(a))*(function(b) - function(c))) +
					c*function(a)*function(b) / ((function(c) - function(a))*(function(c) - function(b)));
			}
			else { 
				s = b - function(b)*(b - a) / (function(b) - function(a));
			}
			c = b;
			/**
				If a and s are on different sides the inverse interpolationpolynom are used and
				if they are on the same side newton method are used. This is because the newton method 
				creates a fictive line with a zero point whereas the inverse interpolationspolynom interpolates only
				through the three supporting points
			*/
			if (function(a)*function(s) < 0) {
				b = s;
			}
			else {
				a = s;
			}
	
			if (b < a) {
				swap(a, b);
			}
	
			/**
				Abort if the computation of zero tooks too much repetitions
			*/
			repetition++;
			if (repetition > REPETITION) {
				exitFailure(__FILE__, __LINE__);
			}
		}
		return std::abs(function(b)) < eps ? b : a;
	}
}

#endif /* MATH_ZERO_H_ */