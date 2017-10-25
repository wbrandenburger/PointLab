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

#ifndef POINTCLOUD_QUATERION_H_
#define POINTCLOUD_QUATERION_H_

#include "eigen3/Eigen/Dense"

namespace pointcloud
{
	/**
		Constant pi
	*/
	const double PI = 3.14159265359;

	template<typename ElementType> class Quaterion
	{
	public:

		/**
			Constructor
		*/
		Quaterion() : v(nullptr)
		{
			v = new ElementType[4];
			std::memset(v, 0, sizeof(ElementType) * 4);

			v[0] = 1;
		}

		/**
			Constructor

			@param[in] angle__ Rotation around a specific axis
			@param[in] axis_x_ Part of the axis
			@param[in] axis_y_ Part of the axis
			@param[in] axis_z_ Part of the axis
		*/
		Quaterion(ElementType angle_, ElementType axis_x_, ElementType axis_y_, ElementType axis_z_) : v(nullptr)
		{
			v = new ElementType[4];

			setRotationAxisPrivate(angle_, axis_x_, axis_y_, axis_z_);
		}

		/**
			Constructor

			@param[in] x_ Euler angle around the x-axis in rad
			@param[in] y_ Euler angle around the y-axis in rad
			@param[in] z_ Euler angle around the z-axis in rad
		*/
		Quaterion(ElementType x_, ElementType y_, ElementType z_) : v(nullptr)
		{
			v = new ElementType[4];

			setEulerAnglePrivate(x_, y_, z_);
		}

		/**
			Destructor
		*/
		~Quaterion()
		{
			clear();
		}

		/**
			Clear
		*/
		void clear() 
		{
			if (v) {
				delete[] v;
				v = nullptr;
			}
		}

		/**
			Copy constructor

			@param[in] Quaterion_ An instance of class Quaterion
		*/
		Quaterion(const Quaterion<ElementType>& quaterion_) : v(nullptr)
		{
			v = new ElementType[4];
			std::memcpy(v, quaterion_.getQuaterion(), sizeof(ElementType) * 4);
		};

		/**
			Copy constructor

			@param[in] Quaterion_ An instance of class Quaterion
		*/
		Quaterion(Quaterion<ElementType>&& quaterion_) : v(nullptr)
		{
			v = new ElementType[4];
			std::memcpy(v, quaterion_.getQuaterion(), sizeof(ElementType) * 4);

			quaterion_.clear();
		};

		/**
			Operator =
	
			@param[in] class_ An instance of class Quaterion
		*/
		Quaterion& operator=(const Quaterion<ElementType>& quaterion_)
		{
			clear();
			v = new ElementType[4];
			std::memcpy(v, quaterion_.getQuaterion(), sizeof(ElementType) * 4);

			return *this;
		};
			
		/**
			Operator =
	
			@param[in] class_ An instance of class Quaterion
		*/
		Quaterion& operator=(Quaterion<ElementType>&& quaterion_)
		{
			clear();

			v = new ElementType[4];
			std::memcpy(v, quaterion_.getQuaterion(), sizeof(ElementType) * 4);

			quaterion_.clear();

			return *this;
		};
		
		/**
			Set Quaterion

			@param[in] w_ Rotation around a specific axis
			@param[in] x_ Part of the axis
			@param[in] y_ Part of the axis
			@param[in] z_ Part of the axis
		*/
		void setQuaterion(ElementType w_, ElementType x_, ElementType y_, ElementType z_)
		{
			v[0] = w_;
			v[1] = x_;
			v[2] = y_;
			v[3] = z_;
		}

		/**
			Set Quaterion

			@param[in] angle_ Rotation around a specific axis
			@param[in] axis_x_ Part of the axis
			@param[in] axis_y_ Part of the axis
			@param[in] axis_z_ Part of the axis
		*/
		void setRotationAxis(ElementType angle_, ElementType axis_x_, ElementType axis_y_, ElementType axis_z_)
		{
			setRotationAxisPrivate(angle_, axis_x_, axis_y_, axis_z_);
		}

		/**
			Set Quaterion

			@param[in] x_ Euler angle around the x-axis in rad
			@param[in] y_ Euler angle around the y-axis in rad
			@param[in] z_ Euler angle around the z-axis in rad
		*/
		void setEulerAngle(ElementType x_, ElementType y_, ElementType z_)
		{
			setEulerAnglePrivate(x_, y_, z_);
		}

		/**
			Operator()

			@param[in] index_ Index
			@return Quaterion of the given index
		*/
		ElementType operator[](size_t index_) const
		{
			return v[index_];
		}

		/**
			Operator *= 
			
			@param[in] quaterion_ An instance of class Quaterion
		*/
		Quaterion& operator*=(const Quaterion<ElementType>& r_)
		{
			ElementType* new_v = new ElementType[4];
			new_v[0] = r_[0] * v[0] - r_[1] * v[1] - r_[2] * v[2] - r_[3] * v[3];
			new_v[1] = r_[0] * v[1] + r_[1] * v[0] - r_[2] * v[3] + r_[3] * v[2];
			new_v[2] = r_[0] * v[2] + r_[1] * v[3] + r_[2] * v[0] - r_[3] * v[1];
			new_v[3] = r_[0] * v[3] - r_[1] * v[2] + r_[2] * v[1] + r_[3] * v[0];

			delete[] v;
			v = new_v;

			return *this;
		}

		/**
			Operator *= 
			
			@param[in] quaterion_ An instance of class Quaterion
		*/
		Quaterion operator*(const Quaterion<ElementType>& r_)
		{
			Quaterion<ElementType> quaterion;
			quaterion.setQuaterion(r_[0] * v[0] - r_[1] * v[1] - r_[2] * v[2] - r_[3] * v[3],
				r_[0] * v[1] + r_[1] * v[0] - r_[2] * v[3] + r_[3] * v[2],
				r_[0] * v[2] + r_[1] * v[3] + r_[2] * v[0] - r_[3] * v[1],
				r_[0] * v[3] - r_[1] * v[2] + r_[2] * v[1] + r_[3] * v[0]);

			return quaterion;
		}

		/**
			Squared length of the quaterion

			@return length
		*/
		ElementType lengthsqr() 
		{
			return (v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);		
		}

		/**
			Conjugate of the quaterion
		*/
		void conjugate()
		{
			v[1] = -v[1];
			v[2] = -v[2];
			v[3] = -v[3];
		}
		
		/**
			Inverse of the quaterion
		*/
		void inverse()
		{
			ElementType length_sqr = lengthsqr();

			conjugate();
			for (size_t i = 0; i < 4; i++) {
				v[i] /= lengthsqr();
			}
		}

		/**
			Get quaterion

			@return Quaterion
		*/
		ElementType* getQuaterion() const
		{
			return v;
		}

		/**
			Get Euler angle

			@param[in,out] x_ Euler angle around the x-axis in rad
			@param[in,out] y_ Euler angle around the y-axis in rad
			@param[in,out] z_ Euler angle around the z-axis in rad
		*/
		void getEulerAngles(ElementType& x_, ElementType& y_, ElementType& z_) const
		{
			ElementType sqw = v[0]*v[0];
			ElementType sqx = v[1]*v[1];
			ElementType sqy = v[2]*v[2];
			ElementType sqz = v[3]*v[3];

			x_ = std::atan2((ElementType)2.0 * (v[0]*v[1] + v[2]*v[3]), (ElementType)1.0 - (ElementType)2.0 * (sqx + sqy));
			y_ = std::asin( (ElementType)2.0 * (v[0]*v[2] - v[1]*v[3]));
			z_ = std::atan2((ElementType)2.0 * (v[0]*v[3] + v[1]*v[2]), (ElementType)1.0 - (ElementType)2.0 * (sqy + sqz));
		} 
		
		/**
			Get the orthogonal matrix corresponding to a rotation by the unit quaterion

			@param[in,out] matrix_ Pointer to a matrix with nine elements
		*/
		void getRotationMatrix(ElementType* matrix_)
		{
			ElementType sqw = v[0] * v[0];
			ElementType sqx = v[1] * v[1];
			ElementType sqy = v[2] * v[2];
			ElementType sqz = v[3] * v[3];

			matrix_[0] = (ElementType)1.0 - (ElementType)2.0*(sqy + sqz); 
			matrix_[1] = (ElementType)2.0*(v[1] * v[2] - v[0] * v[3]); 
			matrix_[2] = (ElementType)2.0*(v[0] * v[2] + v[1] * v[3]);
			matrix_[3] = (ElementType)2.0*(v[1] * v[2] + v[0] * v[3]); 
			matrix_[4] = (ElementType)1.0 - (ElementType)2.0*(sqx + sqz); 
			matrix_[5] = (ElementType)2.0*(v[2] * v[3] - v[0] * v[1]);
			matrix_[6] = (ElementType)2.0*(v[1] * v[3] - v[0] * v[2]); 
			matrix_[7] = (ElementType)2.0*(v[0] * v[1] + v[2] * v[3]); 
			matrix_[8] = (ElementType)1.0 - (ElementType)2.0*(sqx + sqy);
		}

	private:
		
		/**
			Set Quaterion

			@param[in] angle_ Rotation around a specific axis
			@param[in] axis_x_ Part of the axis
			@param[in] axis_y_ Part of the axis
			@param[in] axis_z_ Part of the axis
		*/
		void setRotationAxisPrivate(ElementType angle_, ElementType axis_x_, ElementType axis_y_, ElementType axis_z_)
		{
			v[0] = std::cos(angle_ * (ElementType)0.5);
			v[1] = axis_x_*std::sin(angle_ * (ElementType)0.5);
			v[2] = axis_y_*std::sin(angle_ * (ElementType)0.5);
			v[3] = axis_z_*std::sin(angle_ * (ElementType)0.5);
		}

		/**
			Set Quaterion

			@param[in] x_ Euler angle around the x-axis in rad
			@param[in] y_ Euler angle around the y-axis in rad
			@param[in] z_ Euler angle around the z-axis in rad
		*/
		void setEulerAnglePrivate(ElementType x_, ElementType y_, ElementType z_)
		{
			while (x_ < -(ElementType)PI || x_ >(ElementType)PI) {
				x_ += x_ < -(ElementType)PI ? (ElementType)PI : -(ElementType)PI;
			}
			while (y_ < -(ElementType)PI || y_ >(ElementType)PI) {
				y_ += y_ < -(ElementType)PI ? (ElementType)PI : -(ElementType)PI;
			}
			while (z_ < -(ElementType)PI || z_ >(ElementType)PI) {
				z_ += z_ < -(ElementType)PI ? (ElementType)PI : -(ElementType)PI;
			}

			ElementType cx = std::cos(x_ * (ElementType)0.5);
			ElementType sx = std::sin(x_ * (ElementType)0.5);
			ElementType cy = std::cos(y_ * (ElementType)0.5);
			ElementType sy = std::sin(y_ * (ElementType)0.5);
			ElementType cz = std::cos(z_ * (ElementType)0.5);
			ElementType sz = std::sin(z_ * (ElementType)0.5);

			v[0] = cx * cy * cz + sx * sy * sz;
			v[1] = sx * cy * cz - cx * sy * sz;
			v[2] = cx * sy * cz + sx * cy * sz;
			v[3] = cx * cy * sz - sx * sy * cz;
		}

	private:

		ElementType* v;
	};

	/**
		Operator << Prints the values of the quaterion

		@param[in,out] out_ Outstream in which the quaterion box will be printed
		@param[in] quaterion Quaterion which shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const pointcloud::Quaterion<ElementType>& quaterion_)
	{
		out_ << quaterion_[0] << " " << quaterion_[1] << " " << quaterion_[2] << " " << quaterion_[3];

		return out_;
	}
}

#endif /* POINTCLOUD_QUATERION_H_ */