#include "../../inc/methods/bmmethod.hpp"
using namespace std;
using namespace cv;

BMMethod::BMMethod(bool show):MatchingMethod(show) {
	if (show)
		createParameterWindow("parameters",cv::Size(640,0));
}

cv::Mat BMMethod::getDisparity(cv::Mat left, cv::Mat right) {
	if (left.channels() > 1 || right.channels() > 1) {
			cvtColor(left, left, CV_BGR2GRAY);
			cvtColor(right, right, CV_BGR2GRAY);
	}

	Mat disp;
	sbm.state->preFilterCap = preFilterCap;
	sbm.state->SADWindowSize = SADWindowSize < 5 ? SADWindowSize = 5
			: SADWindowSize % 2 == 0 ? ++SADWindowSize : SADWindowSize;
	sbm.state->minDisparity = minDisparity;
	sbm.state->numberOfDisparities = numberOfDisparities < 16 ? 16
			: numberOfDisparities % 16 == 0 ? numberOfDisparities : numberOfDisparities
					&= -16;
	sbm.state->textureThreshold = textureThreshold;
	sbm.state->uniquenessRatio = uniquenessRatio;
	sbm.state->speckleWindowSize = speckleWindowSize;
	sbm.state->speckleRange = speckleRange;
	sbm.state->disp12MaxDiff = disp12MaxDiff;
	sbm(left, right, disp);
	disp.convertTo(disp, CV_8U, 255 / (numberOfDisparities * 16.));

	return disp;
}

void BMMethod::createParameterWindow(string name,Size position) {
	if (cvGetWindowHandle(name.c_str())!=NULL)
		destroyWindow(name);
	namedWindow(name,CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	moveWindow(name,position.width,position.height);
	createTrackbar("preFilterCap", name, &preFilterCap, 63);
	createTrackbar("SADWindowSize", name, &SADWindowSize, 254);
	createTrackbar("minDisparity", name, &minDisparity, 30);
	createTrackbar("numberOfDisparities", name, &numberOfDisparities, 200);
	createTrackbar("textureThreshold", name, &textureThreshold, 50);
	createTrackbar("uniquenessRatio", name, &uniquenessRatio, 50);
	createTrackbar("speckleWindowSize", name, &speckleWindowSize, 200);
	createTrackbar("speckleRange", name, &speckleRange, 50);
	createTrackbar("disp12MaxDiff", name, &disp12MaxDiff, 50);
}
