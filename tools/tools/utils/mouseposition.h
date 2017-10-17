/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017  Wolfgang Brandenburger. All rights reserved.
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

#ifndef UTILS_MOUSEPOSITION_H_
#define UTILS_MOUSEPOSITION_H_

namespace utils
{
	/**
		Structure which holds the current mouse position
	*/
	class MousePosition
	{
		
	public:
		
		/**
			Constructor
		*/
		MousePosition() : x(NULL), y(NULL) {}
			
		/**
			Constructor

			@param[in] x_ Current position in x direction
			@param[in] y_ Current position in y direction
		*/
		MousePosition(int x_, int y_)
		{
			x = x_;
			y = y_;
		}

		/**
			Destructor
		*/
		~MousePosition() 
		{
			clear();
		}

		/**
			Set Position

			@param[in] x_ Current position in x direction
			@param[in] y_ Current position in y direction
		*/
		void setPosition(int x_, int y_)
		{
			x = x_;
			y = y_;
		}

		/**
			Set Position

			@param[in] x_ Current position in x direction
			@param[in] y_ Current position in y direction
			@param[in,out] x_difference_ Difference in x-direction
			@param[in,out] y_difference_ Difference in y-direction
		*/
		void setPosition(int x_, int y_, int& x_difference_, int& y_difference_)
		{
			x_difference_ = x_ - x;
			y_difference_ = y_ - y;
			
			x = x_;
			y = y_;
		}	

		/**
			Clear
		*/
		void clear()
		{
			x = NULL;
			y = NULL;
		}

		/**
			Operator ==

			@param[in] mouse_position_ Another instance of struct MousePosition
		*/
		bool operator==(const MousePosition&  mouse_position_) const
		{
			if ((*this).x == mouse_position_.x && (*this).y == mouse_position_.y) {
				return true;
			}

			return false;
		}

		/**
			Operator !=

			@param[in] mouse_position_ Another instance of struct MousePosition
		*/
		bool operator!=(const MousePosition&  mouse_position_) const
		{
			if ((*this).x != mouse_position_.x || (*this).y != mouse_position_.y) {
				return true;
			}

			return false;
		}

		/**
			Operator >

			@param[in] mouse_position_ Another instance of struct MousePosition
		*/
		bool operator>(const MousePosition&  mouse_position_) const
		{
			if ((*this).x > mouse_position_.getX()) {
				return true;
			}

			return false;
		}

		/**
			Get x
		*/
		int getX() const
		{
			return x;
		}

		/**
			Get y
		*/
		int getY() const
		{
			return y;
		}

	private:

		/**
			Current position in x direction
		*/
		int x;

		/**
			Current position in y direction
		*/
		int y;
	};

	/**
		Print currrent mouse position

		@param[in] out_ Instance of ostream
		@param[in] mouse_position_ Current mouse position
		@return Instance of ostream 
	 */
	std::ostream& operator<<(std::ostream& out_, const utils::MousePosition& mouse_position_)
	{
		out_ << mouse_position_.getX() << " " << mouse_position_.getY();

		return out_;
	}
}
#endif /* UTILS_MOUSEPOSITION_H_ */