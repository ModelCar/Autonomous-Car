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
	//bool notALine(vector<Point> contour);
    bool matchContours(vector<Point> c1, vector<Point> c2);
    Point2d convertPoint(Mat homography, Point2d p);
    Mat readHomography(string path);
    void showBirdView(Mat srcImage, Mat H, double Z, Size dstImageSize);
    vector<vector<Point> > joinContours(vector<vector<Point> > contours);
    vector<Point> isJoinable(vector<Point> c1, vector<Point> c2);
	Mat blurImg;
    Mat detected_edges;
    Mat roiImg;
    Mat cannyImg;
	Mat drawing;
    Mat resultImg;
    Mat video;
    static const int edgeThresh = 70;
    // Image diameter
    int imgWidth;
    int imgHeight;
    // width and height of the ROI
    int roiX;
    int roiY;
    // Coordinates to start the ROI
    int startX;
    int startY;
	// Coordinates of Camera
	int camX;
	int camY;
	// Left and right Line
	//vector< vector<Point> > Lines;
    // minimal contour Length
    static const int thrMinContour = 50;
    // result as detected lanes
    struct Lane {
        vector<Point> line;
        u_int64_t frame;
        int priority;
    };
    vector<Lane> lanes;
    // min priority to show green line
    static const int thrPriority = 15;
    // frame count to buffer last lines
    static const int bufferNewLines = 5;
    // percentage for Douglas Peucker alg
    static const int thrDP = 1;
    // Match contours parameters
    static const int thrX = 20;
    static const int thrY = 10;

};

#endif // OPENCV_LINE_DETECTION_H
