#include <opencv2/opencv.hpp>

std::vector<std::pair<cv::Point, uint> > signDetection(cv::Mat img);
std::vector<std::pair<cv::Point, uint> > red(cv::Mat img);
std::vector<std::pair<cv::Point, uint> > yellow(cv::Mat img);
std::vector<std::pair<cv::Point, uint> > blue(cv::Mat img);
std::vector<std::pair<cv::Point, uint> > findCircles(cv::Mat imgFiltered);
std::vector<std::pair<cv::Point, uint> > findRedCircles(cv::Mat img, cv::Mat imgFiltered);
int detectNumber(cv::Mat img);
int detectShape(cv::Mat img);
cv::Mat extractContours(cv::Mat img);
double sqLength(cv::Point a, cv::Point b);
bool equalLength(std::vector<cv::Point> sign, int factor);
bool orientation(std::vector<cv::Point> sign, double cy);
bool equalSign(std::pair<cv::Point, uint> a, std::pair<cv::Point, uint> b);
void draw(cv::Mat img, std::vector<std::pair<cv::Point, uint> > signs, cv::Scalar color);
cv::Mat copyContour(cv::Mat img, cv::Vec3f contour);
cv::Mat copyContour(cv::Mat img, std::vector<cv::Point> contour);
std::vector<std::vector<cv::Point> > getReferences();
