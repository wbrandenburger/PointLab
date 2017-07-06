//#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <eigen3/Eigen/Dense>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include "tools/io.h"
#include "tools/opencv/utils.h"

boost::mutex mutex;

void gcpimgto3d(boost::filesystem::directory_iterator dir_iter_) {
	if (boost::filesystem::is_regular_file(dir_iter_->status())) {
		if (dir_iter_->path().extension() == ".jpg") {
			
			Eigen::Matrix3d cameraA;
			Eigen::Matrix3d cameraR;
			Eigen::Vector3d cameraT;
			Filereader filereader;

			std::string txtname = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/OrientierungIII/DLR/" + dir_iter_->path().stem().string() + "_param.txt";
			filereader.readParametersDLR(txtname, cameraA, cameraR, cameraT);

			cv::Mat imgColor = cv::imread(dir_iter_->path().string());
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
				utils::find<int>(imgLabel, coordinates, j);

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
				utils::find<uchar>(patchThresh, patchCoordinates, 255);

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

				if (patchStd(1) < 2.0 && patchStd(1) < 2.0) {
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

					mutex.lock();
					std::ofstream outStream("C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/gcps.txt", std::ofstream::app);
					outStream.precision(15);
					outStream << -(coordinate(1) + 676700) << " " << coordinate(0) - 5819700 << " " << coordinate(2) << std::endl;
					outStream.close();
					mutex.unlock();


					coordinate = cameraR*coordinate + cameraT;
					coordinate /= coordinate(2);
					coordinate = cameraA*coordinate;

					if (coordinate(0) > 0 && coordinate(1) > 0 && coordinate(0) < imgColor.cols && coordinate(1) < imgColor.rows) {
						cv::circle(imgColor, cv::Point(coordinate(0), coordinate(1)), 32.0, cv::Scalar(0, 0, 255), 16, 8);
					}

					

				}
			}
		}
	}
}

int main(){

	std::string img1N = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Bilder/";
	std::string gcpN = "C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/XXXX_2017_Befliegung_ortho.txt";
	
	std::ofstream outStream("C:/Users/Wolfgang Brandenburg/Documents/Schnoeggersburg/Daten/gcps.txt", std::ofstream::out);
	outStream.precision(15);
	outStream.close();

	boost::filesystem::directory_iterator end_iter;
	if (boost::filesystem::exists(img1N) && boost::filesystem::is_directory(img1N)) {
		boost::thread_group group;
		int counter = 0;
		for (boost::filesystem::directory_iterator dir_iter(img1N); dir_iter != end_iter; ++dir_iter) {
			//group.add_thread(new boost::thread(gcpimgto3d, dir_iter));

			//if (counter  == boost::thread::hardware_concurrency() - 1 ){
			//	group.join_all();
			//}

			gcpimgto3d(dir_iter);

			counter++;
		}

		std::cout << counter << std::endl;
	}

	

	return 0;
}


