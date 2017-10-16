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
#include "tools/utils/mouseposition.h"

namespace utils
{
	/**
		Handle for mouse events

		@param[in] event_ Mouse event
		@param[in] x_ Current position in x direction
		@param[in] y_ Current position in y direction
		@param[in] flags_ Current parameter
		@param[in] param_ Pointer to data
	*/
	static void mouseHandler(int event_, int x_, int y_, int flags_, void* param_);

	class Plot
	{

	public:

		/**
			Constructor
		*/
		Plot(char* window_name_ = "Plot 1", bool interaction_ = 0) : number_of_functions(0), size_plot(1024), 
			number_of_elements(0), starting_index(0), x_zero(NULL), mouse_position(utils::MousePosition()),
			window_name(window_name_), interaction(interaction_) {}

		/**
			Destructor
		*/
		~Plot() {}

		/**
			Clear
		*/
		virtual void clear() = 0;

		/**
			Set size of image
		*/
		virtual void setSize(size_t size_plot_)
		{
			size_plot = size_plot_;
		}

		/**
			Zoom in

			@param[in] mouse_position_ Current mouse position
		*/
		virtual void setZoomIn(const utils::MousePosition mouse_position_) = 0;

		/**
			Zoom out
		*/
		virtual void setZoomOut() = 0;

		/**
			Set mouse position

			@param[in] mouse_position_ Current mouse position
		*/
		virtual void setMousePosition(utils::MousePosition mouse_position_)
		{
			mouse_position = mouse_position_;
		}

		/**
			Get mouse position
		*/
		virtual utils::MousePosition getMousePosition() const
		{
			return mouse_position;
		}

		/**
			Print x- and y-values for current mouse position

			@param[in] mouse_position_ Current mouse position
		*/
		virtual void getValues(utils::MousePosition mouse_position_) const = 0;

 		/**
			Draw image
		*/
		virtual void drawImage() = 0;


		/**
			Plot
		*/
		virtual void plot()
		{
			drawImage();

			if (interaction) {
				cvSetMouseCallback(window_name, utils::mouseHandler, this);
			}
		}

	protected:

		/**
			Number of functions
		*/
		size_t number_of_functions;

		/**
			Size of the image
		*/
		size_t size_plot;

		/**
			Number of elements
		*/
		size_t number_of_elements;

		/**
			Starting index
		*/
		size_t starting_index;

		/**
			Zero
		*/
		double x_zero;

		/**
			Image
		*/
		cv::Mat image;

		/**
			Mouse position
		*/
		utils::MousePosition mouse_position;

		/**
			Window
		*/
		char* window_name;

		/**
			Interaction
		*/
		bool interaction;
	};

	/**
		Handle for mouse events

		@param[in] event_ Mouse event
		@param[in] x_ Current position in x direction
		@param[in] y_ Current position in y direction
		@param[in] flags_ Current parameter
		@param[in] param_ Pointer to data
	*/
	static void mouseHandler(int event_, int x_, int y_, int flags_, void* param_)
	{
		if (event_ == CV_EVENT_LBUTTONDOWN)
		{
			utils::MousePosition mouse_position(x_, y_);
			Plot* plot = (Plot*)param_;

			(*plot).setMousePosition(mouse_position);

		}
		if (event_ == CV_EVENT_LBUTTONUP)
		{
			utils::MousePosition mouse_position(x_, y_);
			Plot* plot = (Plot*) param_;

			if ((*plot).getMousePosition() != mouse_position) {
				if ((*plot).getMousePosition() > mouse_position) {
					utils::MousePosition temp = (*plot).getMousePosition();
					(*plot).setMousePosition(mouse_position);
					mouse_position = temp;
				}
				(*plot).setZoomIn(mouse_position);
					
				(*plot).drawImage();
			}
			else {
				(*plot).getValues(mouse_position);
			}
		}
		if (event_ == CV_EVENT_RBUTTONUP)
		{
			Plot* plot = (Plot*) param_;

			(*plot).setZoomOut();
			(*plot).drawImage();
		}

	}

	template<typename ElementType> class PlotVector : public Plot 
	{

	public:

		/**
			Constructor
		*/
		PlotVector(char* window_name_ = "Plot 1", bool interaction_ = 0) : 
			Plot(window_name_, interaction_) {}

		/**
			Destructor
		*/
		~PlotVector() {}

		/**
			Clear
		*/
		void clear()
		{
			number_of_functions = 0;
			number_of_elements = 0;
			starting_index = 0;
			x_zero = NULL;
			
			mouse_position.clear();

			functions.clear();
			intervall.clear();
		}

		/**
			Set function

			@param[in] function_ Function which shall be plotted
		*/
		void setFunction(const std::vector<ElementType>& function_)
		{
			functions.push_back(function_);
			number_of_functions++;
		}

		/**
			Set intervall
		*/
		void setIntervall(const std::vector<ElementType>& intervall_)
		{
			intervall = intervall_;
			number_of_elements = intervall.size();

			if (intervall[0] < 0 && intervall[number_of_elements - 1] > 0) {
				size_t index = 0;
				while (intervall[index] < 0) {
					index++;
				}

				x_zero = (double)index / (double)number_of_elements*(double)size_plot;
			}
		}

		/**
			Set intervall
		*/
		void setIntervall()
		{
			if (!number_of_functions) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			if (intervall[starting_index] < 0 && intervall[starting_index + number_of_elements - 1] > 0) {
				size_t index = starting_index;
				while (intervall[index] < 0) {
					index++;
				}

				x_zero = ((double)index - starting_index) / (double)number_of_elements*(double)size_plot;
			}
			else {
				x_zero = NULL;
			}
		}

		/**
			Zoom in

			@param[in] mouse_position_ Current mouse position
		*/
		void setZoomIn(const utils::MousePosition mouse_position_)
		{
			starting_index = (size_t)((double)mouse_position.getX() / (double)size_plot *  (double)number_of_elements) + starting_index;
			number_of_elements = (size_t) ((double) (mouse_position_.getX() - mouse_position.getX()) / (double) size_plot * (double) number_of_elements);

			setIntervall();
		}

		/**
			Zoom out
		*/
		void setZoomOut()
		{
			starting_index = 0;
			number_of_elements = functions[0].size();

			setIntervall();
		}

		/**
			Print x- and y-values for current mouse position

			@param[in] mouse_position_ Current mouse position
		*/
		void getValues(utils::MousePosition mouse_position_) const {

			size_t index = std::round((double)mouse_position_.getX() / (double)size_plot *(double)number_of_elements + (double)starting_index);
			std::cout << "x-value: " << intervall[index];
			for (size_t i = 0; i < number_of_functions; i++) {
				std::cout << " y-value: " << functions[i][index];
			}
			std::cout << std::endl;
		}

 		/**
			Draw image
		*/
		void drawImage() 
		{
			if (!number_of_functions) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			ElementType min = functions[0][starting_index];
			ElementType max = functions[0][starting_index];
	
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = starting_index; j < starting_index + number_of_elements; j++) {
					if (functions[i][j] > max) {
						max = functions[i][j];
					}

					if (functions[i][j] < min) {
						min = functions[i][j];
					}
				}
			}

			ElementType height = max - min;

			image = cv::Mat(size_plot, size_plot, CV_8UC3, cv::Scalar(0, 0, 0));

			for (size_t i = 0; i < number_of_functions; i++) {

				cv::Scalar lineColor;
				switch (i % 6) {
				case 0: lineColor = cv::Scalar(0, 0, 255); break;
				case 1: lineColor = cv::Scalar(0, 255, 0); break;
				case 2: lineColor = cv::Scalar(255, 0, 0); break;
				case 3: lineColor = cv::Scalar(255, 255, 0); break;
				case 4: lineColor = cv::Scalar(255, 0, 255); break;
				case 5: lineColor = cv::Scalar(0, 255, 255); break;
				}

				double shift = size_plot / number_of_elements < 1 ? 1 : size_plot / number_of_elements;
				double j = shift;
				while (j < size_plot){
					cv::line(image,
						cv::Point(j - shift, size_plot - (functions[i][std::round((j - shift)*((double)number_of_elements / (double)size_plot)) + starting_index] - min) * size_plot / height),
						cv::Point(j, size_plot - (functions[i][std::round(j*((double)number_of_elements / (double)size_plot)) + starting_index] - min) * size_plot / height),
						lineColor, 1, 8, 0);
					j += shift;
				}

			}

			double y_zero = size_plot - (-min) * size_plot / height;
			cv::line(image, cv::Point(0, y_zero), cv::Point(size_plot, y_zero), cv::Scalar(255, 255, 255), 1, 8, 0);

			if (x_zero != NULL) {
				cv::line(image, cv::Point(x_zero, 0), cv::Point(x_zero, size_plot), cv::Scalar(255, 255, 255), 1, 8, 0);
			}
			cv::imshow(window_name, image);
		}

	private:

		/**
			Functions
		*/
		std::vector<std::vector<ElementType>> functions;

		/**
			Intervall
		*/
		std::vector<ElementType> intervall;
	};

	template<typename ElementType> class PlotEigen : public Plot
	{

	public:

		/**
			Constructor
		*/
		PlotEigen(char* window_name_ = "Plot 1", bool interaction_ = 0) : 
			Plot(window_name_, interaction_){}

		/**
			Destructor
		*/
		~PlotEigen() {}

		/**
			Clear
		*/
		void clear()
		{
			number_of_functions = 0;
			number_of_elements = 0;
			starting_index = 0;
			x_zero = NULL;
			
			mouse_position.clear();

			functions.clear();
			intervall = Eigen::Matrix<ElementType, Eigen::Dynamic, 1>();
		}

		/**
			Set function

			@param[in] function_ Function which shall be plotted
		*/
		void setFunction(const Eigen::Matrix<ElementType, Eigen::Dynamic, 1>& function_)
		{
			functions.push_back(function_);
			number_of_functions++;
		}

		/**
			Set intervall
		*/
		void setIntervall(const Eigen::Matrix<ElementType, Eigen::Dynamic, 1>& intervall_)
		{
			intervall = intervall_;
			number_of_elements = intervall.rows();

			if (intervall(0,0) < 0 && intervall(number_of_elements - 1,0) > 0) {
				size_t index = 0;
				while (intervall(index,0) < 0) {
					index++;
				}

				x_zero = (double)index / (double)number_of_elements*(double)size_plot;
			}
		}

		/**
			Set intervall
		*/
		void setIntervall()
		{
			if (!number_of_functions) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			if (intervall(starting_index,0) < 0 && intervall(starting_index + number_of_elements - 1,0) > 0) {
				size_t index = starting_index;
				while (intervall(index,0) < 0) {
					index++;
				}

				x_zero = ((double)index - starting_index) / (double)number_of_elements*(double)size_plot;
			}
			else {
				x_zero = NULL;
			}
		}

		/**
			Zoom in

			@param[in] mouse_position_ Current mouse position
		*/
		void setZoomIn(const utils::MousePosition mouse_position_)
		{
			starting_index = (size_t)((double)mouse_position.getX() / (double)size_plot *  (double)number_of_elements) + starting_index;
			number_of_elements = (size_t) ((double) (mouse_position_.getX() - mouse_position.getX()) / (double) size_plot * (double) number_of_elements);

			setIntervall();
		}

		/**
			Zoom out
		*/
		void setZoomOut()
		{
			starting_index = 0;
			number_of_elements = functions[0].rows();

			setIntervall();
		}

		/**
			Print x- and y-values for current mouse position

			@param[in] mouse_position_ Current mouse position
		*/
		void getValues(utils::MousePosition mouse_position_) const {

			size_t index = std::round((double)mouse_position_.getX() / (double)size_plot *(double)number_of_elements + (double)starting_index);
			std::cout << "x-value: " << intervall(index,0);
			for (size_t i = 0; i < number_of_functions; i++) {
				std::cout << " y-value: " << functions[i](index,0);
			}
			std::cout << std::endl;
		}

 		/**
			Draw image
		*/
		void drawImage() 
		{
			if (!number_of_functions) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			ElementType min = functions[0](starting_index,0);
			ElementType max = functions[0](starting_index,0);
	
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = starting_index; j < starting_index + number_of_elements; j++) {
					if (functions[i](j,0) > max) {
						max = functions[i](j,0);
					}

					if (functions[i](j,0) < min) {
						min = functions[i](j,0);
					}
				}
			}

			ElementType height = max - min;

			image = cv::Mat(size_plot, size_plot, CV_8UC3, cv::Scalar(0, 0, 0));

			for (size_t i = 0; i < number_of_functions; i++) {

				cv::Scalar lineColor;
				switch (i % 6) {
				case 0: lineColor = cv::Scalar(0, 0, 255); break;
				case 1: lineColor = cv::Scalar(0, 255, 0); break;
				case 2: lineColor = cv::Scalar(255, 0, 0); break;
				case 3: lineColor = cv::Scalar(255, 255, 0); break;
				case 4: lineColor = cv::Scalar(255, 0, 255); break;
				case 5: lineColor = cv::Scalar(0, 255, 255); break;
				}

				double shift = size_plot / number_of_elements < 1 ? 1 : size_plot / number_of_elements;
				double j = shift;
				while (j < size_plot){
					cv::line(image,
						cv::Point(j - shift, size_plot - (functions[i](std::round((j - shift)*((double)number_of_elements / (double)size_plot)) + starting_index, 0) - min) * size_plot / height),
						cv::Point(j, size_plot - (functions[i](std::round(j*((double)number_of_elements / (double)size_plot)) + starting_index, 0) - min) * size_plot / height),
						lineColor, 1, 8, 0);
					j += shift;
				}

			}

			double y_zero = size_plot - (-min) * size_plot / height;
			cv::line(image, cv::Point(0, y_zero), cv::Point(size_plot, y_zero), cv::Scalar(255, 255, 255), 1, 8, 0);

			if (x_zero != NULL) {
				cv::line(image, cv::Point(x_zero, 0), cv::Point(x_zero, size_plot), cv::Scalar(255, 255, 255), 1, 8, 0);
			}
			cv::imshow(window_name, image);
		}

	private:

		/**
			Functions
		*/
		std::vector<Eigen::Matrix<ElementType, Eigen::Dynamic, 1>> functions;

		/**
			Intervall
		*/
		Eigen::Matrix<ElementType, Eigen::Dynamic, 1> intervall;
	};

	template<typename ElementType> class PlotMat : public Plot
	{

	public:

		/**
			Constructor
		*/
		PlotMat(char* window_name_ = "Plot 1", bool interaction_ = 0) :
			Plot(window_name_, interaction_) {}

		/**
			Destructor
		*/
		~PlotMat() {}

		/**
			Clear
		*/
		void clear()
		{
			number_of_functions = 0;
			number_of_elements = 0;
			starting_index = 0;
			x_zero = NULL;
			
			mouse_position.clear();

			functions.clear();
			(*intervall).clear();
		}

		/**
			Set function

			@param[in] function_ Function which shall be plotted
		*/
		void setFunction(utils::Matrix<ElementType>* function_)
		{
			functions.push_back(function_);		
			number_of_functions++;
		}

		/**
			Set intervall
		*/
		void setIntervall(utils::Matrix<ElementType>* intervall_)
		{
			intervall = intervall_;
			number_of_elements = (*intervall).getRows();

			if ((*intervall)[0][0] < 0 && (*intervall)[number_of_elements - 1][0] > 0) {
				size_t index = 0;
				while ((*intervall)[index][0] < 0) {
					index++;
				}

				x_zero = (double)index / (double)number_of_elements*(double)size_plot;
			}
		}

		/**
			Set intervall
		*/
		void setIntervall()
		{
			if (!number_of_functions) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			if ((*intervall)[starting_index][0] < 0 && (*intervall)[starting_index + number_of_elements - 1][0] > 0) {
				size_t index = starting_index;
				while ((*intervall)[index][0] < 0) {
					index++;
				}

				x_zero = ((double)index - starting_index) / (double)number_of_elements*(double)size_plot;
			}
			else {
				x_zero = NULL;
			}
		}

		/**
			Zoom in

			@param[in] mouse_position_ Current mouse position
		*/
		void setZoomIn(const utils::MousePosition mouse_position_)
		{
			starting_index = (size_t)((double)mouse_position.getX() / (double)size_plot *  (double)number_of_elements) + starting_index;
			number_of_elements = (size_t) ((double) (mouse_position_.getX() - mouse_position.getX()) / (double) size_plot * (double) number_of_elements);

			setIntervall();
		}

		/**
			Zoom out
		*/
		void setZoomOut()
		{
			starting_index = 0;
			number_of_elements = (*functions[0]).getRows();

			setIntervall();
		}

		/**
			Print x- and y-values for current mouse position

			@param[in] mouse_position_ Current mouse position
		*/
		void getValues(utils::MousePosition mouse_position_) const {

			size_t index = std::round((double)mouse_position_.getX() / (double)size_plot *(double)number_of_elements + (double)starting_index);
			std::cout << "x-value: " << (*intervall)[index][0];
			for (size_t i = 0; i < number_of_functions; i++) {
				std::cout << " y-value: " << (*functions[i])[index][0];
			}
			std::cout << std::endl;
		}

 		/**
			Draw image
		*/
		void drawImage() 
		{
			if (!number_of_functions) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}

			ElementType min = (*functions[0])[starting_index][0];
			ElementType max = (*functions[0])[starting_index][0];
	
			for (size_t i = 0; i < number_of_functions; i++) {
				for (size_t j = starting_index; j < starting_index + number_of_elements; j++) {
					if ((*functions[i])[j][0] > max) {
						max = (*functions[i])[j][0];
					}

					if ((*functions[i])[j][0] < min) {
						min = (*functions[i])[j][0];
					}
				}
			}

			ElementType height = max - min;

			image = cv::Mat(size_plot, size_plot, CV_8UC3, cv::Scalar(0, 0, 0));

			for (size_t i = 0; i < number_of_functions; i++) {

				cv::Scalar lineColor;
				switch (i % 6) {
				case 0: lineColor = cv::Scalar(0, 0, 255); break;
				case 1: lineColor = cv::Scalar(0, 255, 0); break;
				case 2: lineColor = cv::Scalar(255, 0, 0); break;
				case 3: lineColor = cv::Scalar(255, 255, 0); break;
				case 4: lineColor = cv::Scalar(255, 0, 255); break;
				case 5: lineColor = cv::Scalar(0, 255, 255); break;
				}

				double shift = size_plot / number_of_elements < 1 ? 1 : size_plot / number_of_elements;
				double j = shift;
				while (j < size_plot){
					cv::line(image,
						cv::Point(j - shift, size_plot - ((*functions[i])[std::round((j - shift)*((double)number_of_elements / (double)size_plot)) + starting_index][0] - min) * size_plot / height),
						cv::Point(j, size_plot - ((*functions[i])[std::round(j*((double)number_of_elements / (double)size_plot)) + starting_index][0] - min) * size_plot / height),
						lineColor, 1, 8, 0);
					j += shift;
				}

			}

			double y_zero = size_plot - (-min) * size_plot / height;
			cv::line(image, cv::Point(0, y_zero), cv::Point(size_plot, y_zero), cv::Scalar(255, 255, 255), 1, 8, 0);

			if (x_zero != NULL) {
				cv::line(image, cv::Point(x_zero, 0), cv::Point(x_zero, size_plot), cv::Scalar(255, 255, 255), 1, 8, 0);
			}
			cv::imshow(window_name, image);
		}

	private:

		/**
			Functions
		*/
		std::vector<utils::Matrix<ElementType>*> functions;

		/**
			Intervall
		*/
		utils::Matrix<ElementType>* intervall;
	};
}

#endif /* UTILS_PLOT_H_*/	