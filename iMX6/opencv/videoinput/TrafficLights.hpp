#ifndef __TRAFFICLIGHTS_H_INCLUDED__
#define __TRAFFICLIGHTS_H_INCLUDED__

#include <opencv2/opencv.hpp>

class TrafficLight{
  public:
  int xPos;   		// x position of center of traffic light
  int yPos;			// y position of center of traffic light
  int radius;			// radius of circle detected
  char color;		// "R", "Y", "G"
  int distance;		// Estimation how far away the traffic lights is
  double contorArea;
  double calcArea;
	
  /* getDistance uses radius as parameter (measures in pixels) and return distance of traffic light
   *	measured in centimeters
   */
  void calcDistance();
};


cv::Mat filterRed(cv::Mat img);
cv::Mat filterGreen(cv::Mat img);
cv::Mat filterYellow(cv::Mat img);
void CreateControllers();
std::vector<TrafficLight> DetectLights(cv::Mat imgOriginal);
void CalibrateDistance();
cv::Mat processFilterImage(cv::Mat inputImage, char color);

#endif
