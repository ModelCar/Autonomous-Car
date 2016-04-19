#ifndef __SIGNS_H_INCLUDED__
#define __SIGNS_H_INCLUDED__

#include <opencv2/opencv.hpp>

std::vector<std::pair<cv::Point, uint> > signDetection(cv::Mat img);
std::vector<std::pair<cv::Point, uint> > red(cv::Mat img, cv::Vec3b color);
std::vector<std::pair<cv::Point, uint> > yellow(cv::Mat img, cv::Vec3b color);
std::vector<std::pair<cv::Point, uint> > blue(cv::Mat img, cv::Vec3b color);
std::vector<std::pair<cv::Point, uint> > findCircles(cv::Mat imgFiltered);
std::vector<std::pair<cv::Point, uint> > findRedCircles(cv::Mat img, cv::Mat imgFiltered);
int detectNumber(cv::Mat img);
int detectShape(cv::Mat img);
cv::Mat extractContours(cv::Mat img);
int sqLength(cv::Point a, cv::Point b);
bool equalLength(std::vector<cv::Point> sign, double factor);
bool orientation(std::vector<cv::Point> sign, double cy);
bool equalSign(std::pair<cv::Point, uint> a, std::pair<cv::Point, uint> b);
void draw(cv::Mat img, std::vector<std::pair<cv::Point, uint> > signs, cv::Scalar color);
cv::Mat copyContour(cv::Mat img, cv::Vec3f circle);
cv::Mat copyContour(cv::Mat img, std::vector<cv::Point> contour);
void loadReferences();

#endif
