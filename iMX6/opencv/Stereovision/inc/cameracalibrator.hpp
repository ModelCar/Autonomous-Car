#ifndef CAMERACALIBRATOR_HPP_
#define CAMERACALIBRATOR_HPP_

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

/**
 * Class implementing method for stereo camera calibration
 */
class CameraCalibrator{
public:
	/**
	 * Class constructor
	 */
    CameraCalibrator(const bool showDebugWindows = false) : show(showDebugWindows) {}
	/**
	 * Gets calibration images from specified directory and executes calibration
	 * @param string - path to calibration images directory
	 * @param string - specify name of file used to store intrinsics parameters
	 * @param string - specify name of file used to store extrinsics parameters
	 * @param cv::Size - specify size of calibration chessboard
	 */
	void calibrate(std::string, std::string,std::string,cv::Size boardS = cv::Size(9,6));
	/**
	 * Calibrates stereo camera using OpenCV, extract and saves intrinsics and extrinsics camera calibration parameters to files
	 * @param vector<string> -vector of string containing path to calibration images
	 */
    void StereoCalib(const std::vector<std::string>& imagelist);
	/**
	 * Grab pathes to images from specified directory
	 * @param string - path to directory with calibration images
	 */
	std::vector<std::string> getImagesFromDir(std::string filepath);

	/**
	 *
	 */
	void createNewCalibrationImages(std::string filepath, int camIndex1, int camIndex2);
	/**
	 * Class destructor
	 */
    virtual ~CameraCalibrator(){}
private:
	bool show;
	cv::Size boardSize;
	std::string intrinsics;
	std::string extrinsics;
	cv::Mat R, T, E, F, R1, R2, P1, P2, Q;
};


#endif /* CAMERACALIBRATOR_HPP_ */
