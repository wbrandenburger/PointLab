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

#ifndef IMAGE_UTILS_H_
#define IMAGE_UTILS_H_

#include <opencv2/highgui.hpp>

namespace image
{

	/**
		Compute the plot for a array

		@param y_ the data array which will be used for computation
		@param plotImage_ the resulting image with the plot
	*/
	template<typename ElementType>void plot(cv::Mat y_, cv::Mat& plotImage_, char* color = "red", size_t lineWidth = 1)
	{
		int size = 1024;

		ElementType min = y_.ptr<ElementType>(0)[0];
		ElementType max = y_.ptr<ElementType>(0)[0];
		for (int i = 0; i < y_.cols; i++) {
			if (y_.ptr< ElementType>(0)[i] > max) {
				max = y_.ptr< ElementType>(0)[i];
			}

			if (y_.ptr< ElementType>(0)[i] < min) {
				min = y_.ptr< ElementType>(0)[i];
			}
		}

		ElementType height = max-min;
		for (int i = 1; i < y_.cols; i++) {
			y_.ptr<ElementType>(0)[i] =(y_.ptr<ElementType>(0)[i] - min)*( (ElementType) size / height);
		}

		if (plotImage_.cols == 0 || plotImage_.rows == 0) {
			plotImage_ = cv::Mat(size, size, CV_8UC3, cv::Scalar(0, 0, 0));
		}

		cv::Scalar lineColor;
		if (!strcmp(color, "red")) { lineColor = cv::Scalar(0, 0, 255); }
		if (!strcmp(color, "green")) { lineColor = cv::Scalar(0, 255, 0); }
		if (!strcmp(color, "blue")) { lineColor = cv::Scalar(255, 0, 0); }
		if (!strcmp(color, "cyan")) { lineColor = cv::Scalar(255, 255, 0); }
		if (!strcmp(color, "magenta")) { lineColor = cv::Scalar(255, 0, 255); }
		if (!strcmp(color, "yellow")) { lineColor = cv::Scalar(0, 255, 255); }

		for (int i = 1; i < size; i++) {
			cv::line(plotImage_, cv::Point(i-1, size - std::round(y_.ptr< ElementType>(0)[(int)std::round((i - 1)*((double)y_.cols / size))])),
				cv::Point( i, size - std::round( y_.ptr<ElementType>(0)[(int)std::round((i)*((double)y_.cols / size))])),
				lineColor, lineWidth, 8, 0);
		}
	}

	template<typename ElementType>void bar(cv::Mat y_, cv::Mat& plotImage_, char* color_ = "red")
	{
		int size = 1024;

		ElementType min = y_.ptr<ElementType>(0)[0];
		ElementType max = y_.ptr<ElementType>(0)[0];
		for (int i = 0; i < y_.cols; i++) {
			if (y_.ptr< ElementType>(0)[i] > max) {
				max = y_.ptr< ElementType>(0)[i];
			}

			if (y_.ptr< ElementType>(0)[i] < min) {
				std::cout << y_.ptr< ElementType>(0)[i] << std::endl;
				min = y_.ptr< ElementType>(0)[i];
			}
		}

		ElementType height = max - min;
		for (int i = 1; i < y_.cols; i++) {
			y_.ptr<ElementType>(0)[i] = (y_.ptr<ElementType>(0)[i] - min)*((ElementType)size / height);
		}

		if (plotImage_.cols == 0 || plotImage_.rows == 0) {
			plotImage_ = cv::Mat(size, size, CV_8UC3, cv::Scalar(0, 0, 0));
		}

		cv::Scalar lineColor;
		if (!strcmp(color_, "red")) { lineColor = cv::Scalar(0, 0, 255); }
		if (!strcmp(color_, "green")) { lineColor = cv::Scalar(0, 255, 0); }
		if (!strcmp(color_, "blue")) { lineColor = cv::Scalar(255, 0, 0); }
		if (!strcmp(color_, "cyan")) { lineColor = cv::Scalar(255, 255, 0); }
		if (!strcmp(color_, "magenta")) { lineColor = cv::Scalar(255, 0, 255); }
		if (!strcmp(color_, "yellow")) { lineColor = cv::Scalar(0, 255, 255); }

		for (int i = 1; i < size; i++) {
			rectangle(plotImage_,cv::Point(i-1, size), 
				cv::Point(i, size- std::round(y_.ptr<ElementType>(0)[(int)std::round((i)*((double)y_.cols / size))])), lineColor, CV_FILLED);

		}
	}

	void hist(cv::Mat& image_, cv::Mat& hist_, int dim_ = 1, int channel_ = 1)
	{
		int histSize = 256;
		hist_ = cv::Mat(1, histSize, CV_32F);
		for (int i=  0; i < histSize; i++) {
			hist_.ptr<float>(0)[i] = 0;
		}
		for (int rows = 0; rows < image_.rows; rows++) {
			for (int cols = 0; cols < image_.cols; cols++) {
				hist_.ptr<float>(0)[(int) image_.ptr<uchar>(rows)[cols*dim_ + channel_ - 1]]++;
			}
		}
	}

	void sumhist(cv::Mat& image_, cv::Mat& sumhist_, int dim_ = 1, int channel_ = 1)
	{
		int histSize = 256;
		cv::Mat hist = cv::Mat(1, histSize, CV_32F);
		sumhist_ = cv::Mat(1, histSize, CV_32F);
		for (int i = 0; i < histSize; i++) {
			hist.ptr<float>(0)[i] = 0;
		}
		for (int rows = 0; rows < image_.rows; rows++) {
			for (int cols = 0; cols < image_.cols; cols++) {
				hist.ptr<float>(0)[(int)image_.ptr<uchar>(rows)[cols*dim_ + channel_ - 1]]++;
			}
		}
		sumhist_ = hist;
		sumhist_.ptr<float>(0)[0] = 0;
		for (int i = 1; i < histSize; i++) {
			sumhist_.ptr<float>(0)[i] = sumhist_.ptr<float>(0)[i-1] + hist.ptr<float>(0)[i];
		}
	}

	void binomialKernelRecursivRow(cv::Mat& kernel_, int level_)
	{
		if (kernel_.rows == 0) {
			kernel_ = cv::Mat(2, 1, CV_32F);
			kernel_.ptr<float>(0)[0] = 1;
			kernel_.ptr<float>(1)[0] = 1;
		}

		cv::Mat newKernel = cv::Mat(kernel_.rows + 1, 1, CV_32F);
		for (int rows = 0; rows < newKernel.rows; rows++) {
			float s1 = 0, s2 = 0;
			if (rows > 0) {
				s1 = kernel_.ptr<float>(rows - 1)[0];
			}
			if (rows < kernel_.rows) {
				s2 = kernel_.ptr<float>(rows)[0];
			}
			newKernel.ptr<float>(rows)[0] = s1 + s2;
		}

		kernel_ = newKernel;

		if (kernel_.rows < level_) {
			binomialKernelRecursivRow(kernel_, level_);
		}
	}

	void binomialKernelRecursivCol(cv::Mat& kernel_, int level_)
	{
		if (kernel_.cols == 0) {
			kernel_ = cv::Mat(1, 2, CV_32F);
			kernel_.ptr<float>(0)[0] = 1;
			kernel_.ptr<float>(0)[1] = 1;
		}

		cv::Mat newKernel = cv::Mat(1, kernel_.cols + 1, CV_32F);
		for (int cols = 0; cols < newKernel.cols; cols++) {
			float s1 = 0, s2 = 0;
			if (cols > 0) {
				s1 = kernel_.ptr<float>(0)[cols - 1];
			}
			if (cols < kernel_.cols) {
				s2 = kernel_.ptr<float>(0)[cols];
			}
			newKernel.ptr<float>(0)[cols] = s1 + s2;
		}

		kernel_ = newKernel;

		if (kernel_.cols < level_) {
			binomialKernelRecursivCol(kernel_, level_);
		}
	}

	void binomialKernel(cv::Mat& kernel_, int level_)
	{
		cv::Mat kernel_row;
		binomialKernelRecursivRow(kernel_row, level_);
		cv::Mat kernel_col;
		binomialKernelRecursivCol(kernel_col, level_);

		kernel_ = kernel_row * kernel_col;
	
		int sum = 0;
		for (int rows = 0; rows < kernel_.rows; rows++) {
			for (int cols = 0; cols < kernel_.cols; cols++) {
				sum += kernel_.ptr<float>(rows)[cols];
			}
		}
		kernel_ = kernel_ / sum;
	}

	template<typename ElementType, typename ResultType>
	void filter2DC1 (cv::Mat src_, cv::Mat& dst, cv::Mat kernel_)
	{
		int kernel_x = kernel_.rows;
		int kernel_y = kernel_.cols;

		for (int rows = (kernel_x - 1) / 2; rows < src_.rows - (kernel_x - 1) / 2; rows++) {
			for (int cols = (kernel_y - 1) / 2; cols < src_.cols - (kernel_y - 1) / 2; cols++) {
				
				for (int rows_kernel = 0; rows_kernel < kernel_x; rows_kernel++) {
					for (int cols_kernel = 0; cols_kernel < kernel_y; cols_kernel++) {
						
						int diff_x = (-1)*(kernel_x - 1) / 2 + rows_kernel;
						int diff_y = (-1)*(kernel_y - 1) / 2 + cols_kernel;

						dst.ptr<ResultType>(rows)[cols] += ((ResultType) src_.ptr<ElementType>(rows+diff_x)[cols+diff_y]) *
															kernel_.ptr<ResultType>(rows_kernel)[cols_kernel];

					}
				}
				dst.ptr<ResultType>(rows)[cols] = dst.ptr<ResultType>(rows)[cols] / 8;
			}
		}
	}
	
	template<typename ElementType, typename ResultType>
	void normalize(cv::Mat src_, cv::Mat& dst_) {

		ElementType max = ElementType();
		for (int rows = 1; rows < src_.rows - 1; rows++) {
			for (int cols = 1; cols < src_.cols - 1; cols++) {
				if (max < src_.ptr<ElementType>(rows)[cols]) { max = src_.ptr<ElementType>(rows)[cols]; }
			}
		}

		for (int rows = 1; rows < src_.rows - 1; rows++) {
			for (int cols = 1; cols < src_.cols - 1; cols++) {
				dst_.ptr<ResultType>(rows)[cols] = (ResultType)(src_.ptr<ElementType>(rows)[cols] * (256 / max));
			}
		}
	}

	template<typename ElementType, typename ResultType>
	void normalizePeak(cv::Mat src_, cv::Mat& dst_) 
	{

		ElementType avg = ElementType();
		for (int rows = 1; rows < src_.rows - 1; rows++) {
			for (int cols = 1; cols < src_.cols - 1; cols++) {
				avg += src_.ptr<ElementType>(rows)[cols]; 
			}
		}
		avg /= (src_.cols*src_.rows);
		
		ElementType std = ElementType();
		for (int rows = 1; rows < src_.rows - 1; rows++) {
			for (int cols = 1; cols < src_.cols - 1; cols++) {
				std += (src_.ptr<ElementType>(rows)[cols]-avg)*(src_.ptr<ElementType>(rows)[cols] - avg);
			}
		}
		std = std::sqrt(std / (src_.cols*src_.rows));

		ElementType value = avg + 5 * std;

		for (int rows = 1; rows < src_.rows - 1; rows++) {
			for (int cols = 1; cols < src_.cols - 1; cols++) {
				dst_.ptr<ResultType>(rows)[cols] = (ResultType)(src_.ptr<ElementType>(rows)[cols] * (256 / value));
			}
		}
	}

	template<typename ElementType>
	void find(cv::Mat& src_, std::vector<std::vector<size_t>>& coordinates, size_t value)
	{
		for (size_t rows = 0; rows < src_.rows; rows++) {
			for (size_t cols = 0; cols < src_.cols; cols++) {
				if (src_.ptr<ElementType>(rows)[cols] == value) {
					std::vector<size_t> coordinate{ rows, cols };
					coordinates.push_back(coordinate);
				}
			}
		}
	}

}

#endif /* IMAGE_UTILS_H_*/