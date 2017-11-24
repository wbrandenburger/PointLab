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

#ifndef POINTCLOUD_MOVINGPOINT_H_
#define POINTCLOUD_MOVINGPOINT_H_

namespace pointcloud
{
	template<typename ElementType> class MovingPoint
	{
	public:

		/**
			Constructor
		*/
		MovingPoint()
		{
		}

		/**
			Destructor
		*/
		~MovingPoint()
		{
		}
		
		/**
			Copy constructor

			@param[in] class An instance of class  MovingPoint
		*/
		MovingPoint(const  MovingPoint<ElementType>& class) = delete;

		/**
			Copy constructor
	
			@param[in] class An instance of class  MovingPoint
		*/
		MovingPoint(const  MovingPoint<ElementType>&& class) = delete;
	
		/**
			Operator =
	
			@param[in] class An instance of class  MovingPoint
			@return Returns reference to the current instance
		*/
		MovingPoint& operator=(const  MovingPoint<ElementType>& class) = delete;
	
		/**
			Operator =
		
			@param[in] class An instance of class  MovingPoint
			@return Returns reference to the current instance
		*/
		MovingPoint& operator=(const  MovingPoint<ElementType>&& class) = delete;
		
		/**
			Set MovingPoint
		*/
		void setMovingPoint()
		{
			
		}

	private:

	};
}

#endif /* POINTCLOUD_MOVINGPOINT_H_ */