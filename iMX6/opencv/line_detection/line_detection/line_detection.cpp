#include <iostream>
#include "line_detection.h"
#include <algorithm>    // std::sort

using namespace std;
using namespace cv;

// Temporary for sorting vectors of points
struct myclass {
    bool operator() (cv::Point pt1, cv::Point pt2) { return (pt1.y > pt2.y);}
} myobject;

bool notALine(vector<Point> contour){
	int distanceThreshold = 5;
	int numIterations = 0;
	/*
	for(vector<Point>::iterator p = contour.begin(); p != contour.end(); p++){
		for(vector<Point>::iterator q = p; q != contour.end(); q++){
			numIterations++;
			if(p->y == q->y && (p->x)-(q->x) > distanceThreshold) {
				return true;
			}
		}
	}*/
	for(int i = 0; i < contour.size(); i++){
		for(int j = i; j < contour.size(); j++){
			numIterations++;
			if(contour[i].y == contour[j].y && abs((contour[i].x)-(contour[j].x)) > distanceThreshold) {
				return true;
			}
		}
	}
	cout << "Number of Iterations: " << numIterations << endl;
	return false;
}

double distToCam(vector<Point> p, int camX, int camY){
	int maxY = -1;
	int index = -1;
	for(int i=0; i<p.size(); i++){
		if(maxY<p[i].y) {
			maxY = p[i].y;
			index = i;
		}
	}
	cout << "Point with max Y-value: " << p[index] << endl; 
	int x = abs(camX - p[index].x);
	int y = abs(camY - p[index].y);
	return sqrt(x^2+y^2);
}
/*int maxXValue(vector<Point> contour){
	int maxX = 0;
	for(Point::iterator p = contour.begin(); p!=contour.end(); p++){
		if(maxX<p->x) {
			maxX = p->x;
		}
	}
	return maxX;		
}
int minXValue(vector<Point> contour){
	int minX = 0;
	for(Point::iterator p = contour.begin(); p!=contour.end(); p++){
		if(minX>p->x) {
			minX = p->x;
		}
	}
	return minX;		
}*/
int meanXValue(vector<Point> contour){
	int sumX = 0;
	for(int i = 0; i < contour.size(); i++) {
		sumX += contour[i].x;
	}
	return sumX/contour.size();		
}
/*
vector< vector<Point> > lines(vector< vector<Point> > contours, Point cam){
	vector<Point> left;
	vector<Point> right;
	vector< vector<Point> > resultLines;
	int closestLeft = 1000;
	int closestRigth = 1000;
	int dist;
	for(int i = 0; i< contours.size(); i++ ){
		dist = distToCam(contours[i]);
		if(meanXValue(contours[i] < cam.x && closestLeft > dist){
			closestLeft = dist;
			left = contours[i];
		}
		else if(closestRight > dist){
			closestRight = dist;
			right = contours[i];
		}	
	}
	return resultLines;
}*/

/**
 * @brief OpencvLineDetection::searchLine
 * @param imagen
 * @return
 */
bool OpencvLineDetection::searchLine(Mat image, u_int64_t cycle) {


    // Load a new Image from Path
    // Load image as greyscaled
    // Check if image has data and return -1 if not
    if ( !image.data )
        {
            printf("No image data \n");
            return 0;
        } else {
            //printf("Got image\n");
    }

    if (image.rows <= 1 || image.cols <= 1){
        throw std::invalid_argument( "image to small exception" );
    }
    
    resultImg = image;
    
    imgHeight = image.rows; // y
    imgWidth = image.cols; // x
    roiX = imgWidth;
    roiY = imgHeight/2;
    startX = 0;
    startY = imgHeight/2;
	camX = roiX/2;
	camY = roiY;
	double closestLeft = 1000;
	double closestRight = 1000;
	vector< vector<Point> > Lines(2);
    
    Rect roi(startX, startY, roiX, roiY);
    image(roi).copyTo(roiImg);
    
    // Convert to gray scale
    cvtColor(roiImg, roiImg, COLOR_BGR2GRAY);
    
    // Prepare with blurring
    blur(roiImg, blurImg, Size(4,4));

    // Run the edge detector on grayscale
    int kernel_size = 3;
    Canny(blurImg, detected_edges, edgeThresh, edgeThresh*3, kernel_size);
    cannyImg = Scalar::all(0);
    roiImg.copyTo(cannyImg, detected_edges);
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);

    findContours(cannyImg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    drawing = Mat::zeros(cannyImg.size(), CV_8UC3 );
    
    // Filter small contours by thrMinContour
    int i = 0;
    for(vector< vector<Point> >::iterator it = contours.begin(); it!=contours.end();){
        if(it->size()<thrMinContour){
            it=contours.erase(it);
        }
		else if(notALine(contours[i])){
		it=contours.erase(it);
		}
        else{
			//std::sort(contours[i].begin(), contours[i].end(), myobject);
			//cout << "Iteration i: " << i << endl;
			//cout << "Size of contour: " << contours[i].size() << endl;
			//cout << "Contour: " << contours[i] << endl;
            ++it;
	    	i++;
        }
    }
    
    for(int i = 0; i < contours.size(); i++ ){
        //set colors
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

		approxPolyDP(Mat(contours[i]), contours[i], thrDP, true);

		// Distance to line
		double currDist;

		
		currDist = distToCam(contours[i], camX, camY);
		cout << "Mean X: " << meanXValue(contours[i]) << endl;
		cout << "Current Distance: " << currDist << endl;
		if(meanXValue(contours[i]) < camX) {
			if(currDist < closestLeft) {
				closestLeft = currDist;
				Lines[0] = contours[i];
			}
		}
		else {
			if(currDist < closestRight) {
				closestRight = currDist;
				Lines[1] = contours[i];
			}
		}
		
		cout << "Closest Right: " << closestRight << endl;
		cout << "Closest Left: " << closestLeft << endl;
		cout << "Contour " << i << ": " << contours[i] << endl;
        //draw different contours
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
	resultImg = Mat::zeros(cannyImg.size(), CV_8UC3 );
	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	drawContours(resultImg, Lines, 0, color, 2, 8, hierarchy, 0, Point() );
	drawContours(resultImg, Lines, 1, color, 2, 8, hierarchy, 0, Point() );

    return true;
}

