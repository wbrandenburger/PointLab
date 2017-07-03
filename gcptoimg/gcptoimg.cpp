/*
 * GCPtoIMG.cpp
 *
 *  Created on: 27.06.2017
 *      Author: andreask
 */

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <eigen3/Eigen/Dense>

#include <boost/filesystem.hpp>

#include "tools/io/filereader.h"

cv::Mat img;
Eigen::Matrix3d cameraA;
Eigen::Matrix3d cameraR;
Eigen::Vector3d cameraT;
Filereader filereader;

int main(){

	std::string img1N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Bilder/";
	std::string gcpN = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/XXXX_2017_Befliegung_ortho.txt";

	std::cout << "Going to read: " << gcpN << std::endl;
	std::vector<Eigen::Vector3d> gcps;
	filereader.readGCP(gcpN, gcps);

	boost::filesystem::directory_iterator end_iter;
	if (boost::filesystem::exists(img1N) && boost::filesystem::is_directory(img1N)) {
		for(boost::filesystem::directory_iterator dir_iter(img1N) ; dir_iter != end_iter ; ++dir_iter) {
			if (boost::filesystem::is_regular_file(dir_iter->status()) ) {
				//std::cout << dir_iter->path().extension().string() << std::endl;
				if(dir_iter->path().extension()==".jpg"){

					std::cout << "Going to read: " << dir_iter->path().string() << std::endl;
					img = cv::imread(dir_iter->path().string());

					std::string txtname = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/OrientierungIII/DLR/" +  dir_iter->path().stem().string() + "_param.txt";
					std::cout << "Going to read: " << txtname << std::endl;
					filereader.readParametersDLR(txtname, cameraA, cameraR, cameraT);

					int counter = 0;
					for(unsigned int i=0; i<gcps.size(); i++){
						std::cout << gcps[i].transpose() << std::endl;
						Eigen::Vector3d p = cameraR*gcps[i] + cameraT;
						p/=p[2];
						p = cameraA*p;
						//std::cout << p.transpose() << std::endl;

						if(p[0]>0 && p[1]>0 && p[0]<img.cols && p[1]<img.rows){
							cv::circle(img, cv::Point( p[0], p[1] ), 32.0, cv::Scalar( 0, 0, 255 ), 16, 8 );
							counter++;
						}
					}

					cv::namedWindow("IMG", cv::WINDOW_NORMAL);
					cv::imshow("IMG", img);
					std::cout << counter << std::endl;
					cv::waitKey(0);
				}
			}
		}
	}

	return 0;
}


