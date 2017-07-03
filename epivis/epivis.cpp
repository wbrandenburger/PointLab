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

#include "tools/io/filereader.h"

cv::Mat img1;
cv::Mat img2;
cv::Mat img1_orig;
cv::Mat img2_orig;
Eigen::Matrix3d cameraA1, cameraA2;
Eigen::Matrix3d cameraR1, cameraR2;
Eigen::Vector3d cameraT1, cameraT2;
Eigen::Matrix3d fundamentalmatrix;
Filereader filereader;

bool ComputeLine(Eigen::Vector3d &line, const float iw5, const float ih5, float &xl1, float &xl2,
			 float &yl1, float &yl2){
  int flag;

  //degenerate cases: line in the direction of the x and y axes, respectively
  float l1 = line[0], l2 = line[1], l3 = line[2];
  if(fabs(l2) < 1.e-20f){
	  xl1 = -l3 / l1;
	  xl2 = xl1;
	  yl1 = -ih5;
	  yl2 = ih5;
	  } //if fabs
  if(fabs(l1) < 1.e-20f){
	  yl1 = -l3 / l2;
	  yl2 = yl1;
	  xl1 = -iw5;
	  xl2 = iw5;
	} //if fabs

  xl1 = ((ih5 * l2) - l3) / l1;
  xl2 = ((-ih5 * l2) - l3) / l1;
  yl1 = ((iw5 * l1) - l3) / l2;
  yl2 = ((-iw5 * l1) - l3) / l2;

  //distinguish between all the different cases how the epipolar line might intersect the square
  flag = 0;
  if((xl1 >= -iw5) && (xl1 <= iw5)){
	  if((yl1 >= -ih5) && (yl1 <= ih5)){
		  flag = 1;
		  yl2 = yl1;
		  xl2 = -iw5;
		  yl1 = -ih5;
		  } //if y11
	else{
		flag = -1;
		yl1 = -ih5;
		} //else
	  } //if xl1
  else{
	  if((yl1 >= -ih5) && (yl1 <= ih5)){
		  flag = -1;
		  xl1 = -iw5;
		  } //if yl1
	  } //else

  if(flag < 1){
	  if((xl2 >= -iw5) && (xl2 <= iw5)){
		  if(flag == -1){
			  flag = 1;
			  yl2 = ih5;
			  } //if flag
		  else if((yl2 >= -ih5) && (yl2 <= ih5)){
			  flag = 1;
			  xl1 = iw5;
			  yl1 = yl2;
			  yl2 = ih5;
			} //else if yl2
		  } //if xl2
	else if((yl2 >= -ih5) && (yl2 <= ih5)) if (flag == -1){
		flag = 1;
		xl2 = iw5;
		} //else if
	  } //if flag
  // flag == 0 means: line is outside presentation area

  if (flag == 0)
    return(false);
  else
    return(true);
} // void ComputeLines

void mouseHandler(int event, int x, int y, int flags, void* param)
{
    switch(event){
    case CV_EVENT_LBUTTONUP:

    	img1_orig.copyTo(img1);
    	img2_orig.copyTo(img2);

    	std::cout << x << " " <<  y << std::endl;
    	cv::circle(img1, cv::Point(x,y), 5, cv::Scalar(0, 255, 0), 1, 8);
    	img1.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
    	cv::imshow("IMG1", img1);

    	Eigen::Vector3d p1(x, y, 1);
    	//Eigen::Vector3d epiline = p1.transpose()*fundamentalmatrix;
    	Eigen::Vector3d epiline = fundamentalmatrix*p1;

    	/*
    	p1 = cameraA1.inverse()*p1;
    	double d1 = 0.3;
    	p1*=d1;
		p1 = cameraR1.inverse()*(p1 - cameraT1);
		p1 = cameraR2*p1 + cameraT2;
    	p1 = p1/p1[2];
    	p1 = cameraA2*p1;

    	Eigen::Vector3d p2(x,y,1);
    	p2 = cameraA1.inverse()*p2;
    	double d2 = 1.4;
    	p2*=d2;
   		p2 = cameraR1.inverse()*(p2 - cameraT1);
    	p2 = cameraR2*p2 + cameraT2;
    	p2 = p2/p2[2];
    	p2 = cameraA2*p2;

    	cv::circle(img2, cv::Point(p2[0],p2[1]), 5, cv::Scalar(0, 255, 0), 1, 8);
    	cv::circle(img2, cv::Point(p1[0],p1[1]), 5, cv::Scalar(0, 255, 0), 1, 8);

    	std::cout << "dist: " << sqrt((double)((p2[0]-p1[0])*(p2[0]-p1[0]) + (p2[1]-p1[1])*(p2[1]-p1[1]))) << std::endl;

    	Eigen::Matrix2d A;
    	A(0,0)=p1[0]; A(0,1)=1; A(1,0)=p2[0]; A(1,1)=1;
    	Eigen::Vector2d b;
    	b(0)=p1[1]; b(1)=p2[1];
    	Eigen::Vector2d x = A.colPivHouseholderQr().solve(b);
    	cv::line(img2, cv::Point(0,x[1]), cv::Point(img2.cols-1, x[0]*img2.cols-1+x[1]), cv::Scalar(0, 0, 255), 4);

    	//std::cout << "check: " << epiline[0]*ep[0] + epiline[1]*ep[1]  + epiline[2] << std::endl;
*/

    	float xl1, xl2, yl1, yl2;
    	ComputeLine(epiline, img2.cols, img2.rows, xl1, xl2, yl1, yl2);
    	cv::line(img2, cv::Point((int) xl1,(int) yl1), cv::Point((int) xl2,(int) yl2), CV_RGB(255,0,0), 1, CV_AA);

    	/*double x1 = -epiline[2]/epiline[1];
    	double y1 = -epiline[2]/epiline[0];
    	//std::cout << epiline << std::endl;
    	std::cout << "x-achse:" << y1 << "  , y-achse " << x1 << std::endl;
    	//if(x1>0 && x1<img2.cols){
    	if(false){
			Eigen::Matrix2d A2;
			A2(0,0)=0; A2(0,1)=1; A2(1,0)=y1; A2(1,1)=1;
			Eigen::Vector2d b2;
			b2(0)=x1; b2(1)=0;
			Eigen::Vector2d x2 = A2.colPivHouseholderQr().solve(b2);
			cv::line(img2, cv::Point(0,x2[1]), cv::Point(img2.cols-1, x2[0]*img2.cols-1+x2[1]), cv::Scalar(0, 255, 0), 1);
    	}
    	//else if(y1>0 && y1<img2.rows){
    	else if(true){
    		std::cout << "not supported" << std::endl;

			Eigen::Matrix2d A2;
			A2(0,0)=1; A2(0,1)=0; A2(1,0)=1; A2(1,1)=x1;
			Eigen::Vector2d b2;
			b2(0)=0; b2(1)=y1;
			Eigen::Vector2d y2 = A2.colPivHouseholderQr().solve(b2);
			cv::line(img2, cv::Point(y2[1],0), cv::Point(y2[0]*img2.rows-1+y2[1], img2.rows-1), cv::Scalar(0, 255, 0), 1);

    	}
    	else{
    		std::cout << "no intersection" << std::endl;
    	}*/

    	cv::imshow("IMG2", img2);
    }
}

int main( int argc, char** argv ){

	//std::string img1N = argv[1];
	//std::string conf1N = argv[2];
	//std::string img2N = argv[3];
	//std::string conf2N = argv[4];

	std::string img1N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Bilder/52700.jpg";
	std::string conf1N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/OrientierungIII/DLR/52700_param.txt";
	std::string img2N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Bilder/52701.jpg";
	std::string conf2N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/OrientierungIII/DLR/52701_param.txt";

	std::cout << "Going to read: " << img1N << std::endl;
	img1 = cv::imread(img1N);
	img1_orig = cv::imread(img1N);
	filereader.readParametersDLR(conf1N, cameraA1, cameraR1, cameraT1);

	std::cout << "Going to read: " << img2N << std::endl;
	img2 = cv::imread(img2N);
	img2_orig = cv::imread(img2N);
	filereader.readParametersDLR(conf2N, cameraA2, cameraR2, cameraT2);

	//Eigen::Matrix3d R = (cameraR1*cameraR2.transpose()).inverse();
	//Eigen::Vector3d transl = cameraR2.transpose()*cameraT2 - cameraR1.transpose()*cameraT1;

	Eigen::Matrix3d R = cameraR2*cameraR1.transpose();
	Eigen::Vector3d transl = cameraT2 - R*cameraT1;

	Eigen::Matrix3d S;
	S(0,0)=0.0; S(0,1)=-transl[2]; S(0,2)=transl[1];
	S(1,0)=transl[2]; S(1,1)=0.0; S(1,2)=-transl[0];
	S(2,0)=-transl[1]; S(2,1)=transl[0]; S(2,2)=0;
	//std::cout << S << std::endl;

	fundamentalmatrix = cameraA1.inverse().transpose() * S * R * cameraA2.inverse();
	//fundamentalmatrix = cameraA1.inverse().transpose() * cameraR1
	//		* S * cameraR2.transpose() * cameraA2.inverse();
	std::cout << "Transl: " << transl.transpose() << std::endl;
	std::cout << fundamentalmatrix << std::endl;

	cv::namedWindow("IMG1", cv::WINDOW_NORMAL);
	int mouseParam= CV_EVENT_FLAG_LBUTTON;
	cvSetMouseCallback("IMG1", mouseHandler, &mouseParam);
	cv::imshow("IMG1", img1);

	cv::namedWindow("IMG2", cv::WINDOW_NORMAL);
	cv::imshow("IMG2", img2);

	cv::waitKey(0);
}
