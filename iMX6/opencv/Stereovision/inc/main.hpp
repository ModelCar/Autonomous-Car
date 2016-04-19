#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "depthsubstraction.hpp"
#include "cameracalibrator.hpp"

std::string calibImagesPath = "../calibration";
std::string INTRINSICS = "../calibration/intrinsics.yml";
std::string EXTRINSICS = "../calibration/extrinsics.yml";

/**
 * Main class of application
 */
class StereoVision {
public:
	/**
	 * Class constructor
	 * @param int - defines mode of application
	 * @param string - path to intrinsics camera calibration parameters
	 * @param string - path to extrinsics camera calibration parameters
	 */
	StereoVision(const int mode = 0, const std::string in = INTRINSICS, const std::string ex = EXTRINSICS);
	/**
	 * runs application
	 */
	void run();
	/**
	 * Class destructor
	 */
	~StereoVision(){};

private:
	void showImages();

	int leftcamera;
	int rightcamera;
	int methodNr;
	int mode; /** 0 - normal, 1-calibration **/

	cv::Mat left_frame;
	cv::Mat right_frame;
	cv::Mat depth_map;
	cv::Size image_size;

	CameraCalibrator cl;
	DepthSubstraction ds;
};

#endif /* MAIN_HPP_ */
