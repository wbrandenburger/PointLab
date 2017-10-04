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

#ifndef UTILS_PLOT_H_
#define UTILS_PLOT_H_

#include <opencv2/opencv.hpp>

#include "eigen3/Eigen/Dense"

#include "tools/utils/matrix.h"

namespace utils
{
	/**
		Compute the plot for a array

		@param[in] y_ The data array which will be used for computation
		@param[in,out] plot_image_ The resulting image with the plot
		@param[in] color_ String which determines the color
		@param[in] line_width_ Determines the width of the line
	*/
	template<typename ElementType>void plot(cv::Mat y_, cv::Mat& plot_image_, char* color = "red", size_t line_width = 1)
	{
		size_t size = 1024;

		ElementType min = y_.ptr<ElementType>(0)[0];
		ElementType max = y_.ptr<ElementType>(0)[0];
		for (size_t i = 0; i < y_.cols; i++) {
			if (y_.ptr< ElementType>(0)[i] > max) {
				max = y_.ptr< ElementType>(0)[i];
			}

			if (y_.ptr< ElementType>(0)[i] < min) {
				min = y_.ptr< ElementType>(0)[i];
			}
		}

		ElementType height = max - min;

		if (plot_image_.cols == 0 || plot_image_.rows == 0) {
			plot_image_ = cv::Mat(size, size, CV_8UC3, cv::Scalar(0, 0, 0));
		}

		cv::Scalar lineColor;
		if (!strcmp(color, "red")) { lineColor = cv::Scalar(0, 0, 255); }
		if (!strcmp(color, "green")) { lineColor = cv::Scalar(0, 255, 0); }
		if (!strcmp(color, "blue")) { lineColor = cv::Scalar(255, 0, 0); }
		if (!strcmp(color, "cyan")) { lineColor = cv::Scalar(255, 255, 0); }
		if (!strcmp(color, "magenta")) { lineColor = cv::Scalar(255, 0, 255); }
		if (!strcmp(color, "yellow")) { lineColor = cv::Scalar(0, 255, 255); }

		for (int i = 1; i < size - 1; i++) {
			cv::line(plot_image_, cv::Point(i - 1, size - (y_.ptr< ElementType>(0)[std::round((i - 1)*((double)y_.size() / (double)size))] - min) * size / height),
				cv::Point(i, size - (y_.ptr< ElementType>(0)[std::round((i)*((double)y_.size() / (double)size))] - min) * size / height),
				lineColor, line_width, 8, 0);
		}
	}

	/**
		Compute the plot for a array

		@param[in] y_ The data array which will be used for computation
		@param[in,out] plot_image_ The resulting image with the plot
		@param[in] color_ String which determines the color
		@param[in] line_width_ Determines the width of the line
	*/
	template<typename ElementType>void plot(std::vector<ElementType>& y_, cv::Mat& plot_image_, char* color = "red", size_t line_width = 1)
	{
		size_t size = 1024;

		ElementType min = y_[0];
		ElementType max = y_[0];
		for (size_t i = 0; i < y_.size(); i++) {
			if (y_[i] > max) {
				max = y_[i];
			}

			if (y_[i] < min) {
				min = y_[i];
			}
		}
		
		ElementType height = max - min;

		if (plot_image_.cols == 0 || plot_image_.rows == 0) {
			plot_image_ = cv::Mat(size, size, CV_8UC3, cv::Scalar(0, 0, 0));
		}

		cv::Scalar lineColor;
		if (!strcmp(color, "red")) { lineColor = cv::Scalar(0, 0, 255); }
		if (!strcmp(color, "green")) { lineColor = cv::Scalar(0, 255, 0); }
		if (!strcmp(color, "blue")) { lineColor = cv::Scalar(255, 0, 0); }
		if (!strcmp(color, "cyan")) { lineColor = cv::Scalar(255, 255, 0); }
		if (!strcmp(color, "magenta")) { lineColor = cv::Scalar(255, 0, 255); }
		if (!strcmp(color, "yellow")) { lineColor = cv::Scalar(0, 255, 255); }

		for (int i = 1; i < size - 1; i++) {
			cv::line(plot_image_, cv::Point(i - 1, size - (y_[std::round((i - 1)*((double)y_.size() / (double)size))] - min) * size / height),
				cv::Point(i, size - (y_[std::round((i)*((double)y_.size() / (double)size))] - min) * size / height),
				lineColor, line_width, 8, 0); 
		}
	}

	/**
		Compute the plot for a array

		@param[in] y_ The data array which will be used for computation
		@param[in,out] plot_image_ The resulting image with the plot
		@param[in] color_ String which determines the color
		@param[in] line_width_ Determines the width of the line
	*/
	template<typename ElementType>void plot(Eigen::Matrix<ElementType, Eigen::Dynamic, 1> y_, cv::Mat& plot_image_, char* color = "red", size_t line_width = 1)
	{
		size_t size = 1024;

		ElementType min = y_(0,0);
		ElementType max = y_(0,0);
		for (size_t i = 0; i < y_.size(); i++) {
			if (y_(i, 0) > max) {
				max = y_(i, 0);
			}

			if (y_(i, 0) < min) {
				min = y_(i, 0);
			}
		}

		ElementType height = max - min;

		if (plot_image_.cols == 0 || plot_image_.rows == 0) {
			plot_image_ = cv::Mat(size, size, CV_8UC3, cv::Scalar(0, 0, 0));
		}

		cv::Scalar lineColor;
		if (!strcmp(color, "red")) { lineColor = cv::Scalar(0, 0, 255); }
		if (!strcmp(color, "green")) { lineColor = cv::Scalar(0, 255, 0); }
		if (!strcmp(color, "blue")) { lineColor = cv::Scalar(255, 0, 0); }
		if (!strcmp(color, "cyan")) { lineColor = cv::Scalar(255, 255, 0); }
		if (!strcmp(color, "magenta")) { lineColor = cv::Scalar(255, 0, 255); }
		if (!strcmp(color, "yellow")) { lineColor = cv::Scalar(0, 255, 255); }

		for (int i = 1; i < size - 1; i++) {
			cv::line(plot_image_, cv::Point(i - 1, size - (y_(std::round((i - 1)*((double)y_.size() / (double)size)), 0) - min) * size / height),
				cv::Point(i, size - (y_(std::round((i)*((double)y_.size() / (double)size)), 0) - min) * size / height),
				lineColor, line_width, 8, 0);
		}
	}

	/**
		Compute the plot for a array

		@param[in] y_ The data array which will be used for computation
		@param[in,out] plot_image_ The resulting image with the plot
		@param[in] color_ String which determines the color
		@param[in] line_width_ Determines the width of the line
	*/
	template<typename ElementType>void plot(Matrix<ElementType> y_, cv::Mat& plot_image_, char* color = "red", size_t line_width = 1)
	{
		size_t size = 1024;

		ElementType min = y_[0][0];
		ElementType max = y_[0][0];
		for (size_t i = 0; i < y_.size(); i++) {
			if (y_[i][0] > max) {
				max = y_[i][0];
			}

			if (y_[i][0] < min) {
				min = y_[i][0];
			}
		}


		ElementType height = max - min;

		if (plot_image_.cols == 0 || plot_image_.rows == 0) {
			plot_image_ = cv::Mat(size, size, CV_8UC3, cv::Scalar(0, 0, 0));
		}

		cv::Scalar lineColor;
		if (!strcmp(color, "red")) { lineColor = cv::Scalar(0, 0, 255); }
		if (!strcmp(color, "green")) { lineColor = cv::Scalar(0, 255, 0); }
		if (!strcmp(color, "blue")) { lineColor = cv::Scalar(255, 0, 0); }
		if (!strcmp(color, "cyan")) { lineColor = cv::Scalar(255, 255, 0); }
		if (!strcmp(color, "magenta")) { lineColor = cv::Scalar(255, 0, 255); }
		if (!strcmp(color, "yellow")) { lineColor = cv::Scalar(0, 255, 255); }

		for (int i = 1; i < size - 1; i++) {
			cv::line(plot_image_, cv::Point(i - 1, size - (y_[std::round((i - 1)*((double)y_.size() / (double)size))][0] - min) * size / height),
				cv::Point(i, size - (y_[std::round((i)*((double)y_.size() / (double)size))][0] - min) * size / height),
				lineColor, line_width, 8, 0);
		}
	}

	//template<typename ElementType>void bar(cv::Mat y_, cv::Mat& plotImage_, char* color_ = "red")
	//{
	//	int size = 1024;

	//	ElementType min = y_.ptr<ElementType>(0)[0];
	//	ElementType max = y_.ptr<ElementType>(0)[0];
	//	for (int i = 0; i < y_.cols; i++) {
	//		if (y_.ptr< ElementType>(0)[i] > max) {
	//			max = y_.ptr< ElementType>(0)[i];
	//		}

	//		if (y_.ptr< ElementType>(0)[i] < min) {
	//			std::cout << y_.ptr< ElementType>(0)[i] << std::endl;
	//			min = y_.ptr< ElementType>(0)[i];
	//		}
	//	}

	//	ElementType height = max - min;
	//	for (int i = 1; i < y_.cols; i++) {
	//		y_.ptr<ElementType>(0)[i] = (y_.ptr<ElementType>(0)[i] - min)*((ElementType)size / height);
	//	}

	//	if (plotImage_.cols == 0 || plotImage_.rows == 0) {
	//		plotImage_ = cv::Mat(size, size, CV_8UC3, cv::Scalar(0, 0, 0));
	//	}

	//	cv::Scalar lineColor;
	//	if (!strcmp(color_, "red")) { lineColor = cv::Scalar(0, 0, 255); }
	//	if (!strcmp(color_, "green")) { lineColor = cv::Scalar(0, 255, 0); }
	//	if (!strcmp(color_, "blue")) { lineColor = cv::Scalar(255, 0, 0); }
	//	if (!strcmp(color_, "cyan")) { lineColor = cv::Scalar(255, 255, 0); }
	//	if (!strcmp(color_, "magenta")) { lineColor = cv::Scalar(255, 0, 255); }
	//	if (!strcmp(color_, "yellow")) { lineColor = cv::Scalar(0, 255, 255); }

	//	for (int i = 1; i < size; i++) {
	//		rectangle(plotImage_,cv::Point(i-1, size), 
	//			cv::Point(i, size- std::round(y_.ptr<ElementType>(0)[(int)std::round((i)*((double)y_.cols / size))])), lineColor, CV_FILLED);

	//	}
	//}	
}

#endif /* UTILS_PLOT_H_*/	