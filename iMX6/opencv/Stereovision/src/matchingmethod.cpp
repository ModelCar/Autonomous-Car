#include "../inc/matchingmethod.hpp"

using namespace cv;

void MatchingMethod::setRoi(cv::Rect r1, cv::Rect r2) {
	roi1 = r1;
	roi2 = r2;
}
