#ifndef OPENCV_LINE_DETECTION_H
#define OPENCV_LINE_DETECTION_H

#include "opencv2/opencv.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "math.h"

#include <stdio.h>
#include <stdlib.h>

#include "opencv_utils.h"

class OpencvLineDetection {
public:
    bool searchLine(cv::Mat imagen);
    bool matchContours(std::vector<cv::Point> c1, std::vector<cv::Point> c2);
    cv::Point2d convertPoint(cv::Mat homography, cv::Point2d p);
    cv::Mat readHomography(std::string path);
    void showBirdView(cv::Mat srcImage, cv::Mat H, double Z, cv::Size dstImageSize);
    cv::Mat edge, cedge;
    int edgeThresh = 50;
    // Image diameter
    int imgWidth = 320;
    int imgHeight = 240;
    // Start Coordinates of the ROI
    int roiX = 320;
    int roiY = 60;
    // Coordinates to start of
    int startX = 0;
    int startY = 180;
    // minimal contour Length
    int thrMinContour = 20;
    // result as detected lanes
    std::vector<std::vector<cv::Point> > lanes;
};

#endif // OPENCV_LINE_DETECTION_H
