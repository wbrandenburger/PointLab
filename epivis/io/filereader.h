/*
 * Filereader.h
 *
 *  Created on: 19.08.2016
 *      Author: andreask
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <iostream>
#include <fstream>

#include <vector>

#include <eigen3/Eigen/Dense>

class Filereader{
public:
	void readGCP(std::string filename, std::vector<Eigen::Vector3d>& gcps);
	void readParametersDLR(std::string filename, Eigen::Matrix3d& cam, Eigen::Matrix3d& cameraR, Eigen::Vector3d& cameraT);
	void readParametersPMVS(std::string filename, Eigen::Matrix3d& cam, Eigen::Matrix3d& cameraR, Eigen::Vector3d& cameraT);
};

#endif /* FILEREADER_H_ */
