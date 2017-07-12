//#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <eigen3/Eigen/Dense>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include "tools/io.h"
#include "tools/utils.h"
#include "tools/image/utils.h"

boost::mutex mutex;

void gcpimgto3d(std::string txtnameImg_, std::string txtnameParam_, std::vector<Eigen::Vector3d>& gcps_, std::string additional_) {	
	Eigen::Matrix3d cameraA;
	Eigen::Matrix3d cameraR;
	Eigen::Vector3d cameraT;
	Filereader filereader;
			
	filereader.readParametersDLR(txtnameParam_, cameraA, cameraR, cameraT);

	cv::Mat imgColor = cv::imread(txtnameImg_, CV_LOAD_IMAGE_COLOR || cv::IMREAD_IGNORE_ORIENTATION);
	cv::Size imgSize = imgColor.size();

	// Convert to a grayscale image
	cv::Mat imgGray;
	cv::cvtColor(imgColor, imgGray, CV_RGB2GRAY);

	// Smooth image with boxfilter
	cv::Mat imgGrayBox;
	cv::boxFilter(imgGray, imgGrayBox, imgGray.depth(), cv::Size(5, 5));

	// Binarize image
	cv::Mat imgThresh;
	cv::threshold(imgGrayBox, imgThresh, 250, 1, cv::THRESH_BINARY);

	cv::Mat imgLabel;
	int labels = cv::connectedComponents(imgThresh, imgLabel, 8, CV_32S);

	for (size_t j = 1; j < labels; j++) {
		std::vector<std::vector<size_t>> coordinates;
		image::find<int>(imgLabel, coordinates, j);

		Eigen::Vector3d mean(0.0, 0.0, 1.0);
		float meanx, meany = 0;
		for (size_t i = 0; i < coordinates.size(); i++) {
			mean(0) += coordinates[i][0];
			mean(1) += coordinates[i][1];
		}
		mean(0) /= coordinates.size();
		mean(1) /= coordinates.size();
				
		size_t patchSize = 7;
		int height1 = mean(0) - patchSize;
		if (height1 < 0) {
			height1 = 0;
		}
		int height2 = mean(0) + patchSize;
		if (height2 > imgSize.height) {
			height2 = imgSize.height;
		}
		int width1 = mean(1) - patchSize;
		if (width1 < 0) {
			width1 = 0;
		}
		int width2 = mean(1) + patchSize;
		if (width2 > imgSize.width) {
			width2 = imgSize.width;
		}

		cv::Rect rect(width1, height1, width2 - width1, height2 - height1);
		cv::Mat patch = imgGray(rect);

		cv::Mat patchThresh;
		cv::threshold(patch, patchThresh, 245, 255, cv::THRESH_BINARY);

		std::vector<std::vector<size_t>> patchCoordinates;
		image::find<uchar>(patchThresh, patchCoordinates, 255);

		Eigen::Vector3d patchMean(0.0, 0.0, 1.0);
		for (size_t i = 0; i < patchCoordinates.size(); i++) {
			patchMean(0) += patchCoordinates[i][0];
			patchMean(1) += patchCoordinates[i][1];
		}
		patchMean(0) /= patchCoordinates.size();
		patchMean(1) /= patchCoordinates.size();

		Eigen::Vector2d patchStd(0.0, 0.0);
		for (size_t i = 0; i < patchCoordinates.size(); i++) {
			patchStd(0) += std::pow(patchCoordinates[i][0] - patchMean(0), 2);
			patchStd(1) += std::pow(patchCoordinates[i][1] - patchMean(1), 2);
		}
		patchStd /= patchCoordinates.size();
		patchStd(0) = std::sqrt(patchStd(0)); patchStd(1) = std::sqrt(patchStd(1));

		if (patchStd(1) < 2.0 && patchStd(1) < 2.0 & patchCoordinates.size() < 60) {
			patchMean(0) += width1;
			patchMean(1) += height1;
			Eigen::Vector3d patchMeanA = cameraA.inverse()*patchMean;

			int height = 90;
			Eigen::Vector3d b;
			b(0) = -cameraR(0, 2) * height - cameraT(0);
			b(1) = -cameraR(1, 2) * height - cameraT(1);
			b(2) = -cameraR(2, 2) * height - cameraT(2);

			Eigen::Matrix3d A = cameraR;
			A(0, 2) = -patchMeanA(0);
			A(1, 2) = -patchMeanA(1);
			A(2, 2) = -1;

			Eigen::Vector3d coordinate = A.inverse()*b;
			coordinate(2) = height;

			size_t index = 0;
			double minDist = std::numeric_limits<double>::infinity();
			for (size_t i = 0; i < gcps_.size(); i++) {
				Eigen::Vector3d temp = cameraR*gcps_[i] + cameraT;
				temp /= temp(2);
				temp = cameraA*temp;

				double dist = std::sqrt(std::pow(patchMean(0) - temp(0), 2) + std::pow(patchMean(1) - temp(1), 2));

				if (dist < minDist) {
					minDist = dist;
					index = i+1;
				}

			}

			//if (minDist < 250) {
			//	mutex.lock();
			//	std::ofstream outStream("C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/gcps.txt", std::ofstream::app);
			//	outStream.precision(15);
			//	outStream << coordinate(0) - 676700 << " " << coordinate(1) - 5819700 << " " << coordinate(2) << std::endl;
			//	outStream.close();
			//	mutex.unlock();
			//}

			if (minDist < 250) {
				mutex.lock();
				std::ofstream outStream("C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/gcps.txt", std::ofstream::app);
				outStream.precision(15);
				outStream << additional_ << ";" << index << ";" << patchMean(0) << ";" << patchMean(1) << std::endl;
				outStream.close();
				mutex.unlock();
			}
		}

	}
}


int main(){

	std::string img1N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Bilder/";
	std::string gcpN = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/XXXX_2017_Befliegung_ortho.txt";

	Filereader filereader;
	std::vector<Eigen::Vector3d> gcps;
	filereader.readGCP(gcpN, gcps);

	std::ofstream outStream("C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/gcps.txt", std::ofstream::out);
	outStream << "image gcp row column" << std::endl;
	outStream.close();
	
	utils::Timer timer;
	timer.start();

	boost::filesystem::directory_iterator end_iter;
	
	utils::threadpool pool(24);

	if (boost::filesystem::exists(img1N) && boost::filesystem::is_directory(img1N)) {
		for (boost::filesystem::directory_iterator dir_iter(img1N); dir_iter != end_iter; ++dir_iter) {
			if (boost::filesystem::is_regular_file(dir_iter->status())) {
				if (dir_iter->path().extension() == ".jpg") {
					std::string txtnameImg = dir_iter->path().string();
					std::string txtnameParam = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/OrientierungIII/DLR/" + dir_iter->path().stem().string() + "_param.txt";

					while (!pool.runTask(boost::bind(gcpimgto3d, txtnameImg, txtnameParam, gcps, dir_iter->path().stem().string())));
				}
			}
		}
	}

	pool.shutdown();

	std::cout << timer.stop() << std::endl;

	return 0;
}


