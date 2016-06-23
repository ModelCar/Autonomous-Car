#ifndef DEPTHSUBSTRACTION_HPP_
#define DEPTHSUBSTRACTION_HPP_

#include <iostream>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"

#include "matchingmethod.hpp"
#include "methods/bmmethod.hpp"
#include "methods/sgbmmethod.hpp"

/**
 * Class implementing usage of stereo matching methods in order to obtain depth map
 */
class DepthSubstraction {
public:
	/** Number of implemented stereo matching methods */
	static const int methodCount = 2 ;

	/**
	 * Class constructor
	 * @param int - number of initial stereo matching method
	 * @param bool - specify if window parameter is showed
	 */
	DepthSubstraction(int m = 1,bool t = true);
	/**
	 * Return depth map obtained using left and right frame from camera stream, with the choosen method
	 * @param Mat - left frame
	 * @param Mat - right frame
	 * @param int - newMethod
	 * @return Mat - depth_map
	 */
	cv::Mat getDepthMap(cv::Mat,cv::Mat,int);
	/**
	 * Return top view of the depth map using the actual depth map so the car knows where to go
	 * @param Mat - depth_map
	 * @return Mat - top_view
	 */
	cv::Mat getTopView(cv::Mat);
	/**
	 * extracts intrinsics and extrinsics camera calibration parameters from files
	 * @param string - specify name of file used to store intrinsics parameters
	 * @param string - specify name of file used to store extrinsics parameters
	 */
	void extractCalibrationParams(std::string, std::string);
	/**
	 * undistorts left and right image from camera stream using camera stereo calibration parameters
	 * @param Mat - left frame
	 * @param Mat - right frame
	 */
	void undistort(cv::Mat&, cv::Mat&);
	/**
	 * Class destructor
	 */
	~DepthSubstraction();

	/** number of current disparity method, controlled by trackbar in StereoVision class **/
	int methodNr;

	/** depth map **/
	cv::Mat depth_map;

	/** top view **/
	double saved_value;
	double intensity_average[640];
	double intensity_smooth[640];
    int pixel_width; //320 * 2 pixels
    int pixel_height; //240 * 2 pixels
    int view_top; //where the pixel analysis of the depth map begins
    int view_bottom; //where the pixel analysis of the depth map ends
    int white; //in grayscale CV_8UC1
    int black; //in grayscale CV_8UC1
    int windowSize;
    double sum;

	/** RECTIFY AND CALIBRATION PARAMETERS**/
	cv::Rect roi1, roi2;
	cv::Mat Q;
	cv::Mat R, T, R1, P1, R2, P2;
	cv::Mat M1, D1, M2, D2;
	cv::Mat undistorted;

private:
	void createQMatrix();
	void setMethod(int);

	bool undist;
	bool showDebugWindows;

	MatchingMethod* method;
};

#endif /* DEPTHSUBSTRACTION_HPP_ */
