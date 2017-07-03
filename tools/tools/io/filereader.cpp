/*
 * Filereader.cpp
 *
 *  Created on: 19.08.2016
 *      Author: andreask
 */

#include "filereader.h"
#include <boost/algorithm/string.hpp>
#include <string>     // std::string, std::stod
#include <iostream>   // std::cout

void
Filereader::readGCP(std::string filename, std::vector<Eigen::Vector3d>& gcps) {
	std::cerr << "Reading in file: " << filename << "..." << std::endl;
	std::ifstream inStream(filename.c_str());
	if (!inStream.good()) {
		std::cout << "error in readCamConfig: file " << filename << " not found" << std::endl;
		return;
	}
	std::string buffer;
	getline(inStream, buffer, '\n');
	getline(inStream, buffer, '\n');
	while (inStream.good()) {
		std::cout << buffer << std::endl;
		std::vector<std::string> strs;
		boost::split(strs, buffer, boost::is_any_of("\t"));
		if (strs.size() == 0)
			continue;
		Eigen::Vector3d temp;
		std::cout.precision(20);
		temp[1] = -std::stod(strs[1]);
		temp[0] = std::stod(strs[2]);
		temp[2] = std::stod(strs[3]);
		gcps.push_back(temp);
		std::cout << temp.transpose() << std::endl;
		getline(inStream, buffer, '\n');
	}
}

void
Filereader::readParametersDLR(std::string filename, Eigen::Matrix3d& cam, Eigen::Matrix3d& cameraR, Eigen::Vector3d& cameraT){

	std::cerr << "Reading in file: " << filename << "..." << std::endl;
	std::ifstream inStream(filename.c_str());
	if(!inStream.good()){
		std::cout << "error in readCamConfig: file " << filename << " not found" << std::endl;
		return;
	}
	std::string buffer;
	std::string tester;
	bool setCamR = false;
	bool setCamT = false;
	while(inStream.good()){
		getline(inStream,buffer,'\n');
		std::istringstream raut(buffer.substr(0,1));
		tester = "#";
		if(raut.str().compare(tester)==0){
			continue;
		}
		else{
			int mode = 0;
			int tempNr=buffer.find("=",1);
			std::stringstream stm(buffer.substr(0,tempNr));
			std::string tester1="camera.T";
			std::string tester2="camera.R";
			std::string tester3="camera.A";
			if(stm.str().compare(tester1)==0) mode=1;
			else if(stm.str().compare(tester2)==0) 	mode=2;
			else if(stm.str().compare(tester3)==0) 	mode=3;
			if(mode==1){
				tempNr=buffer.find("[",1)+1;
				for(int i=0; i<3; i++){
					std::string searchChar = " ";
					if(i==2) searchChar = "]";
					int tempNr2=buffer.find(searchChar,tempNr);
					std::stringstream stm1(buffer.substr(tempNr,tempNr2-tempNr));
					stm1 >> cameraT[i];
					tempNr=tempNr2+1;
				}
				setCamT=true;
			}
			if(mode==2){
				int i=0;
				int j=0;
				unsigned int endNr=buffer.find("]",tempNr)+1;
				tempNr=buffer.find("[",tempNr)+1;
				while(buffer.find(" ",tempNr)<endNr&&buffer.find(" ",tempNr)!=std::string::npos){
					while(buffer.find(" ",tempNr)<buffer.find(";",tempNr)){
						int tempNr2=buffer.find(" ",tempNr);
						std::stringstream stm1(buffer.substr(tempNr,tempNr2-tempNr));
						stm1 >> cameraR(i,j++);
						tempNr=tempNr2+1;
					}
					unsigned int tempNr2=buffer.find(" ",tempNr);
					if(tempNr2==std::string::npos||tempNr2>endNr){
						break;
					}
					std::stringstream stm1(buffer.substr(tempNr,tempNr2-tempNr-1));
					stm1 >> cameraR(i++,j);
					tempNr=tempNr2+1;
					j=0;
				}
				std::stringstream stm1(buffer.substr(tempNr,endNr-tempNr-1));
				stm1 >> cameraR(i,j);
				setCamR=true;
			}
			if(mode==3){
				int i=0;
				int j=0;
				unsigned int endNr=buffer.find("]",tempNr)+1;
				tempNr=buffer.find("[",tempNr)+1;
				while(buffer.find(" ",tempNr)<endNr&&buffer.find(" ",tempNr)!=std::string::npos){
					while(buffer.find(" ",tempNr)<buffer.find(";",tempNr)){
						int tempNr2=buffer.find(" ",tempNr);
						std::stringstream stm1(buffer.substr(tempNr,tempNr2-tempNr));
						stm1 >> cam(i,j++);
						tempNr=tempNr2+1;
					}
					unsigned int tempNr2=buffer.find(" ",tempNr);
					if(tempNr2==std::string::npos||tempNr2>endNr){
						break;
					}
					std::stringstream stm1(buffer.substr(tempNr,tempNr2-tempNr-1));
					stm1 >> cam(i++,j);
					tempNr=tempNr2+1;
					j=0;
				}
				std::stringstream stm1(buffer.substr(tempNr,endNr-tempNr-1));
				stm1 >> cam(i,j);
				setCamR=true;
			}
		}
	}
	if(!setCamT){
		std::cout << "error in readCamConfig: could not read 'camera.T' from '" <<
		"'." << std::endl;
	}
	if(!setCamR){
		std::cout << "error in readCamConfig: could not read 'camera.R' from '" <<
		"'." << std::endl;
	}
	Eigen::Matrix3d cameraR_ = cameraR.transpose();
	Eigen::Vector3d cameraT_ = -cameraR_*cameraT;
	cameraT = cameraT_;
	cameraR = cameraR_;
	std::cout << cameraR << std::endl;
	std::cout << cameraT << std::endl;
}


void
Filereader::readParametersPMVS(std::string filename, Eigen::Matrix3d& cam, Eigen::Matrix3d& cameraR, Eigen::Vector3d& cameraT){

	std::ifstream* file = new std::ifstream(&filename[0]);

	 *file >> cam(0,0) >> cam(0,1) >> cam(0,2);
	 *file >> cam(1,0) >> cam(1,1) >> cam(1,2);
	 *file >> cam(2,0) >> cam(2,1) >> cam(2,2);

	 *file >> cameraR(0,0) >> cameraR(0,1) >> cameraR(0,2);
	 *file >> cameraR(1,0) >> cameraR(1,1) >> cameraR(1,2);
	 *file >> cameraR(2,0) >> cameraR(2,1) >> cameraR(2,2);

	 *file >> cameraT[0] >> cameraT[1] >> cameraT[2];
	 std::cout << cameraT.transpose() << std::endl;
}
