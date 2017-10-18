/*
* epivis.cpp
*
*  Created on: 27.07.2016
*      Author: andreask
*/

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <eigen3/Eigen/Dense>

#include "io/filereader.h"

cv::Mat img1;
cv::Mat img2;
Eigen::Matrix3d cameraA1, cameraA2;
Eigen::Matrix3d cameraR1, cameraR2;
Eigen::Vector3d cameraT1, cameraT2;
Filereader filereader;

std::vector<Eigen::Vector3d> pointsImg1;
std::vector<Eigen::Vector3d> pointsImg2;

void mouseHandlerImg1(int event, int x, int y, int flags, void* param)
{
	switch (event) {
	case CV_EVENT_LBUTTONUP:

		std::cout << x << " " << y << std::endl;
		cv::circle(img1, cv::Point(x, y), 5, cv::Scalar(0, 255, 0), 1, 8);
		img1.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
		cv::imshow("IMG1", img1);
		
		Eigen::Vector3d point(x,y,1);
		pointsImg1.push_back(point);
	}
}

void mouseHandlerImg2(int event, int x, int y, int flags, void* param)
{
	switch (event) {
	case CV_EVENT_LBUTTONUP:

		std::cout << x << " " << y << std::endl;
		cv::circle(img2, cv::Point(x, y), 5, cv::Scalar(0, 255, 0), 1, 8);
		img1.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
		cv::imshow("IMG2", img2);

		Eigen::Vector3d point(x, y, 1);
		pointsImg2.push_back(point);
	}
}

int main(int argc, char** argv) {

	std::string img1N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Bilder/52800.jpg";
	std::string conf1N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/OrientierungIII/DLR/52800_param.txt";
	std::string img2N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Bilder/52801.jpg";
	std::string conf2N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/OrientierungIII/DLR/52801_param.txt";
	
	std::cout << "Going to read: " << img1N << std::endl;
	img1 = cv::imread(img1N);
	filereader.readParametersDLR(conf1N, cameraA1, cameraR1, cameraT1);

	std::cout << "Going to read: " << img2N << std::endl;
	img2 = cv::imread(img2N);
	filereader.readParametersDLR(conf2N, cameraA2, cameraR2, cameraT2);

	cv::namedWindow("IMG1", cv::WINDOW_NORMAL);
	cv::namedWindow("IMG2", cv::WINDOW_NORMAL);

	int mouseParam = CV_EVENT_FLAG_LBUTTON;
	cvSetMouseCallback("IMG1", mouseHandlerImg1, &mouseParam);
	cvSetMouseCallback("IMG2", mouseHandlerImg2, &mouseParam);

	cv::imshow("IMG1", img1);
	cv::imshow("IMG2", img2);

	cv::waitKey(0);

	for (size_t i = 0; i < pointsImg1.size(); i++) {
		Eigen::Vector3d pointImg1 = cameraR1.inverse()*cameraA1.inverse()*pointsImg1[i];
		Eigen::Vector3d pointImg2 = cameraR2.inverse()*cameraA2.inverse()*pointsImg2[i];

		Eigen::Vector3d nLines = pointImg1.cross(pointImg2);
		Eigen::Vector3d nTempPlane = pointImg1.cross(nLines);
		
		double t = nTempPlane.dot(-cameraR1.inverse()*cameraT1 - -cameraR2.inverse()*cameraT2) / (nTempPlane.dot(pointImg2));

		std::cout.precision(20);
		std::cout << "Point: " << std::endl;
		std::cout << -cameraR2.inverse()*cameraT2 + pointImg2*t << std::endl ;

	}

	cv::waitKey(0);
}