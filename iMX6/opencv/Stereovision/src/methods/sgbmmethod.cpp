#include "../../inc/methods/sgbmmethod.hpp"

using namespace std;
using namespace cv;

SGBMMethod::SGBMMethod(bool show):MatchingMethod(show),fullDP(0){
	if (show)
		createParameterWindow("parameters",Size(640,0));
};

Mat SGBMMethod::getDisparity(Mat left, Mat right) {
	if (left.channels() > 1 || right.channels() > 1) {
			cvtColor(left, left, CV_BGR2GRAY);
			cvtColor(right, right, CV_BGR2GRAY);
	}

	Mat disp;
	sgbm.P1 = 8 * left.channels() * sgbm.SADWindowSize * sgbm.SADWindowSize;
	sgbm.P2 = 32 * left.channels() * sgbm.SADWindowSize * sgbm.SADWindowSize;
	sgbm.minDisparity = minDisparity;
	sgbm.SADWindowSize =
			SADWindowSize < 5 ? SADWindowSize = 5 :
			SADWindowSize % 2 == 0 ? ++SADWindowSize : SADWindowSize;
	sgbm.numberOfDisparities =
			numberOfDisparities < 16 ? 16 :
			numberOfDisparities % 16 == 0 ?
					numberOfDisparities : numberOfDisparities &= -16;
	sgbm.uniquenessRatio = uniquenessRatio;
	sgbm.speckleWindowSize = speckleWindowSize;
	sgbm.speckleRange = speckleRange;
	sgbm.disp12MaxDiff = disp12MaxDiff;
	sgbm.fullDP = fullDP == 1;
	sgbm(left, right, disp);

	disp.convertTo(disp, CV_8U, 255 / (numberOfDisparities * 16.));

	return disp;
}

void SGBMMethod::createParameterWindow(string name,Size position) {
	if (cvGetWindowHandle(name.c_str()) != NULL)
		destroyWindow (name);
	namedWindow(name, CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	moveWindow(name, position.width, position.height);

	createTrackbar("SADWindowSize", name, &SADWindowSize, 254);
	createTrackbar("minDisparity", name, &minDisparity, 30);
	createTrackbar("numberOfDisparities", name, &numberOfDisparities,
			200);
	createTrackbar("textureThreshold", name, &textureThreshold, 50);
	createTrackbar("uniquenessRatio", name, &uniquenessRatio, 50);
	createTrackbar("speckleWindowSize", name, &speckleWindowSize, 200);
	createTrackbar("speckleRange", name, &speckleRange, 50);
	createTrackbar("disp12MaxDiff", name, &disp12MaxDiff, 50);
	createTrackbar("fullDP", name, &fullDP, 1);
}
