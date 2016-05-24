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
#include <stdexcept>

#include "opencv_utils.h"

using namespace std;
using namespace cv;

class OpencvLineDetection {
public:
    bool searchLine(Mat imagen, u_int64_t cycle);
    bool matchContours(vector<Point> c1, vector<Point> c2);
    Point2d convertPoint(Mat homography, Point2d p);
    Mat readHomography(string path);
    void showBirdView(Mat srcImage, Mat H, double Z, Size dstImageSize);
    vector<vector<Point> > joinContours(vector<vector<Point> > contours);
    vector<Point> isJoinable(vector<Point> c1, vector<Point> c2);
    Mat edge, cedge;
    Mat video;
    int edgeThresh = 40;
    // Image diameter
    int imgWidth;
    int imgHeight;
    // width and height of the ROI
    int roiX = 320;
    int roiY = 60;
    // Coordinates to start the ROI
    int startX = 0;
    int startY = 180;
    // minimal contour Length
    int thrMinContour = 5;
    // result as detected lanes
    struct Lane {
        vector<Point> line;
        u_int64_t frame;
        int priority;
    };
    vector<Lane> lanes;
    // min priority to show green line
    int thrPriority = 15;
    // frame count to buffer last lines
    int bufferNewLines = 5;
    // percentage for Douglas Peucker alg
    int thrDP = 10;
    // Match contours parameters
    int thrX = 20;
    int thrY = 10;

};

#endif // OPENCV_LINE_DETECTION_H
