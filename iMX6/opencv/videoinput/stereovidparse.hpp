#ifndef __PARSE_H_INCLUDED__
#define __PARSE_H_INCLUDED__

#include <opencv2/opencv.hpp>

void capture1();
void signThread(cv::Mat image);
void lightsThread(cv::Mat image);
int stereoprocess();

#endif
