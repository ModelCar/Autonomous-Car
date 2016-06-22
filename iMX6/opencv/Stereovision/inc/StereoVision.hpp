//
// Created by simonso on 13.06.16.
//

#ifndef SV_STEREOVISION_HPP
#define SV_STEREOVISION_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "opencv2/highgui/highgui.hpp"

#include "depthsubstraction.hpp"
#include "cameracalibrator.hpp"
#include "../inc/Tentacles.hpp"
#include "../inc/Interfaces/ICollisionAvoidanceDelegate.hpp"
#include "../inc/Interfaces/IStereovisionDelegate.hpp"
#include "../inc/com.hpp"

class StereoVision {

public:
    /**
     * Class constructor
     * @param int - defines mode of application
     * @param string - path to intrinsics camera calibration parameters
     * @param string - path to extrinsics camera calibration parameters
     */
    StereoVision(const int m = 0, const std::string in = INTRINSICS, const std::string ex = EXTRINSICS);

    void setCurrentSpeed(double speed) {currentSpeed = speed;}
    void setCurrentSteering(double steering) {currentSteering = steering;}

    /**
     * runs application
     */
    void run();
    /**
     * Class destructor
     */
    ~StereoVision(){}

private:

    static std::string calibImagesPath;
    static std::string INTRINSICS;
    static std::string EXTRINSICS;

    void showImages();
    int initializeSerialDevice();

    double currentSpeed;
    double currentSteering;

    int serialDevice;

    int leftcamera;
    int rightcamera;
    int methodNr;
    int mode; /** 0 - normal, 1 - calibration, 2 - take new calibration images **/

    cv::Mat left_frame;
    cv::Mat right_frame;
    cv::Mat depth_map;
    cv::Mat tentacle_map;
    cv::Size image_size;

    Tentacles tentacles;
    CameraCalibrator camCalibrator;
    DepthSubstraction depthSubstraction;
};


#endif //SV_STEREOVISION_HPP
