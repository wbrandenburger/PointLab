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

#ifndef MATH_FUNCTION_H_
#define MATH_FUNCTION_H_

#include "eigen3/Eigen/Dense"

namespace math
{

	struct Polynomial2D 
	{
		/** 
			Constructor
		*/
		Polynomial2D() {}

		/**
			Constructor

			@param[in] degree_ Degree of the polynomial
			@param[in] parameter_ The coefficients of the polynomial
		*/
		Polynomial2D(Eigen::Matrix<double, Eigen::Dynamic, 1> parameter_, size_t degree_) :
			parameter(parameter_), degree(degree_) {}

		/**
			Destructor
		*/
		~Polynomial2D() {}

		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x_ Function value
			@result Corresponding value
		*/
		double operator()(const double& x_) const
		{
			double result = double();
			for (size_t i = 0; i <= degree; i++) {
				result += parameter(i)*std::pow(x_, i);
			}

			return result;
		}

		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x_ Function value
			@param[in] y_ Function value
			@result Corresponding value
		*/
		double operator()(const double& x_, const double& y_) const
		{
			double result = double();
			for (size_t i = 0; i <= degree; i++) {
				result += parameter(i)*std::pow(x_, i);
			}
			result -= y_;

			return result;
		}

	private:

		/**
			The coefficients of the polynomial
		*/
		Eigen::Matrix<double, Eigen::Dynamic, 1> parameter;

		/**
			Degree of the polynomial	
		*/
		size_t degree;
	};
	
	template<typename FunctionX, typename FunctionY> struct Polynomial2DExpand 
	{
		
		/**
			Constructor
		*/
		Polynomial2DExpand() {}

		/**
			Constructor

			@param[in] polynomial2D_ The basical polynom
			@param[in] function_x_ The function for x
			@param[in] function_y_ The function for y
		*/
		Polynomial2DExpand(Polynomial2D polynomial2D_, FunctionX function_x_, FunctionY function_y_) :
			polynomial2D(polynomial2D_), function_x(function_x_), function_y(function_y_) {}

		/**
			Constructor

			@param[in] degree_ Degree of the polynomial
			@param[in] parameter_ The coefficients of the polynomial
			@param[in] function_x_ The function for x
			@param[in] function_y_ The function for y
		*/
		Polynomial2DExpand(Eigen::Matrix<double, Eigen::Dynamic, 1> parameter_, size_t degree_,
			FunctionX function_x_, FunctionY function_y_) :
			polynomial2D(Polynomial2D(parameter_, degree_)),
			function_x(function_x_), function_y(function_y_) {}

		/**
			Deconstructor
		*/
		~Polynomial2DExpand() {}
		
		/**
			Operator()
			
			@param[in] value_ Function value
			@return Corresponding value 
		*/
		double operator()(const double value_) const
		{
			return polynomial2D(function_x(value_), function_y(value_));
		}

		/**
			Polynom
		*/
		Polynomial2D polynomial2D;

		/**
			Function in x
		*/
		FunctionX function_x;

		/**
			Function in y
		*/
		FunctionY function_y;
	};

	struct Polynomial3D
	{
		/**
			Constructor
		*/
		Polynomial3D() {}

		/**
			Constructor

			@param[in] degree_ Degree of the polynomial
			@param[in] parameter_ The coefficients of the polynomial
		*/
		Polynomial3D(Eigen::Matrix<double, Eigen::Dynamic, 1> parameter_, size_t degree_) :
			parameter(parameter_), degree(degree_) {}

		/**
			Destructor
		*/
		~Polynomial3D() {}


		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x_ Function value
			@param[in] y_ Function value
			@result Corresponding value
		*/
		double operator()(const double& x_, const double& y_) const
		{
			double result = double();
			size_t index = 0;
			for (size_t i = 0; i <= degree; i++) {
				for (size_t j = 0; j <= i; j++) {
					for (size_t k = 0; k <= i; k++) {
						if (j + k == i) {
							result += parameter(index)*std::pow(x_, j)*std::pow(y_, k);
							index++;
						}
					}
				}
			}

			return result;
		}


		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x_ Function value
			@param[in] y_ Function value
			@param[in] z_ Function value
			@result Corresponding value
		*/
		double operator()(const double& x_, const double& y_, const double& z_) const
		{
			double result = double();
			size_t index = 0;
			for (size_t i = 0; i <= degree; i++) {
				for (size_t j = 0; j <= i; j++) {
					for (size_t k = 0; k <= i; k++) {
						if (j + k == i) {
							result += parameter(index)*std::pow(x_, j)*std::pow(y_, k);
							index++;
						}
					}
				}
			}
			result -= z_;

			return result;
		}

	private:
		
		/**
			The coefficients of the polynomial
		*/
		Eigen::Matrix<double, Eigen::Dynamic, 1> parameter;

		/**
			Degree of the polynomial
		*/
		size_t degree;
	};

	template<typename FunctionX, typename FunctionY, typename FunctionZ> struct Polynomial3DExpand 
	{
		
		/**
			Constructor
		*/
		Polynomial3DExpand() {}

		/**
			Constructor

			@param[in] polynomial3D_ The basical polynom
			@param[in] function_x_ The function for x
			@param[in] function_y_ The function for y
			@param[in] function_z_ The function for z
		*/
		Polynomial3DExpand(Polynomial3D polynomial3D_,
			FunctionX function_x_, FunctionY function_y_, FunctionZ function_z_) :
			polynomial3D(polynomial3D_),
			function_x(function_x_), function_y(function_y_), function_z(function_z_) {}

		/**
			Constructor

			@param[in] degree_ Degree of the polynomial
			@param[in] parameter_ The coefficients of the polynomial
			@param[in] function_x_ The function for x
			@param[in] function_y_ The function for y
			@param[in] function_z_ The function for z
		*/
		Polynomial3DExpand(Eigen::Matrix<double, Eigen::Dynamic, 1> parameter_, size_t degree_,
			FunctionX function_x_, FunctionY function_y_, FunctionZ function_z_) :
			polynomial3D(Polynomial3D(parameter_, degree_)),
			function_x(function_x_), function_y(function_y_), function_z(function_z_) {}
		
		/**
			Deconstructor
		*/
		~Polynomial3DExpand() {}
		
		/**
			Operator()
			
			@param[in] value_ Function value
			@return Corresponding value 
		*/
		double operator()(const double value_) const
		{
			return polynomial3D(function_x(value_),function_y(value_), function_z(value_));
		}

		/**
			Polynom
		*/
		Polynomial3D polynomial3D;

		/**
			Function in x
		*/
		FunctionX function_x;

		/**
			Function in y
		*/
		FunctionY function_y;

		/**
			Function in z
		*/
		FunctionZ function_z;
	};

	struct Polynomial4D
	{
		/**
			Constructor
		*/
		Polynomial4D() {}

		/**
			Constructor

			@param[in] degree_ Degree of the polynomial
			@param[in] parameter_ The coefficients of the polynomial
		*/
		Polynomial4D(Eigen::Matrix<double, Eigen::Dynamic, 1> parameter_, size_t degree_) :
			parameter(parameter_), degree(degree_) {}

		/**
			Destructor
		*/
		~Polynomial4D() {}


		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x_ Function value
			@param[in] y_ Function value
			@param[in] z_ Function value
			@result Corresponding value
		*/
		double operator()(const double& x_, const double& y_, const double& z_) const
		{
			double result = double();
			size_t index = 0;
			for (size_t i = 0; i <= degree; i++) {
				for (size_t j = 0; j <= i; j++) {
					for (size_t k = 0; k <= i; k++) {
						for (size_t l = 0; l <= i; l++) {
							if (j + k + l == i) {
								result += parameter(index)*std::pow(x_, i)*std::pow(y_, j)*std::pow(z_, k);
								index++;
							}
						}
					}
				}
			}

			return result;
		}

		/**
			Operator() Computation of a value corresponding to a function value

			@param[in] x_ Function value
			@param[in] y_ Function value
			@param[in] z_ Function value
			@param[in] value_ Function value
			@result Corresponding value
		*/
		double operator()(const double& x_, const double& y_, const double& z_, const double& value_) const
		{
			double result = double();
			size_t index = 0;
			for (size_t i = 0; i <= degree; i++) {
				for (size_t j = 0; j <= i; j++) {
					for (size_t k = 0; k <= i; k++) {
						for (size_t l = 0; l <= i; l++) {
							if (j + k + l == i) {
								result += parameter(index)*std::pow(x_, i)*std::pow(y_, j)*std::pow(z_, k);
								index++;
							}
						}
					}
				}
			}
			result -= value_;

			return result;
		}

	private:

		/**
			The coefficients of the polynomial
		*/
		Eigen::Matrix<double, Eigen::Dynamic, 1> parameter;
		
		/**
			Degree of the polynomial
		*/
		size_t degree;
	};

	template<typename FunctionX, typename FunctionY, typename FunctionZ, typename FunctionValue> struct Polynomial4DExpand 
	{
		
		/**
			Constructor
		*/
		Polynomial4DExpand() {}

		/**
			Constructor

			@param[in] polynomial4D_ The basical polynom
			@param[in] function_x_ The function for x
			@param[in] function_y_ The function for y
			@param[in] function_z_ The function for z
			@param[in] function_value_ The function for value
		*/
		Polynomial4DExpand(Polynomial4D polynomial4D_,
			FunctionX function_x_, FunctionY function_y_, FunctionZ function_z_, FunctionValue function_value_) :
			polynomial4D(polynomial4D_),
			function_x(function_x_), function_y(function_y_), function_z(function_z_), function_value(function_value_) {}

		/**
			Constructor

			@param[in] degree_ Degree of the polynomial
			@param[in] parameter_ The coefficients of the polynomial
			@param[in] function_x_ The function for x
			@param[in] function_y_ The function for y
			@param[in] function_z_ The function for z
			@param[in] function_value_ The function for value
		*/
		Polynomial4DExpand(Eigen::Matrix<double, Eigen::Dynamic, 1> parameter_, size_t degree_,
			FunctionX function_x_, FunctionY function_y_, FunctionZ function_z_, FunctionValue function_value_) :
			polynomial4D(Polynomial3D(parameter_, degree_)),
			function_x(function_x_), function_y(function_y_), function_z(function_z_), function_value(function_value_) {}
		
		/**
			Deconstructor
		*/
		~Polynomial4DExpand() {}
		
		/**
			Operator()
			
			@param[in] value_ Function value
			@return Corresponding value 
		*/
		double operator()(const double value_) const
		{
			return polynomial4D(function_x(value_),function_y(value_), function_z(value_), function_value(value_));
		}

		/**
			Polynom
		*/
		Polynomial4D polynomial4D;

		/**
			Function in x
		*/
		FunctionX function_x;

		/**
			Function in y
		*/
		FunctionY function_y;

		/**
			Function in z
		*/
		FunctionZ function_z;

		/**
			Function in value
		*/
		FunctionValue function_value;
	};
}

#endif /* MATH_FUNCTION_H_ */