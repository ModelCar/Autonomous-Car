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

	int numDisparities = numberOfDisparities < 16 ? 16 :
						 numberOfDisparities % 16 == 0 ?
						 numberOfDisparities : numberOfDisparities &= -16;
	int bmBlockSize = blockSize < 5 ? blockSize = 5
								   : blockSize % 2 == 0 ? ++blockSize : blockSize;
    Ptr <StereoBM> sbm = StereoBM::create(numDisparities,bmBlockSize);

    sbm->setPreFilterCap(preFilterCap< 1 ? 1 : preFilterCap);
    sbm->setMinDisparity(minDisparity);
    sbm->setTextureThreshold(textureThreshold);
    sbm->setUniquenessRatio(uniquenessRatio);
    sbm->setSpeckleWindowSize(speckleWindowSize);
    sbm->setSpeckleRange(speckleRange);
    sbm->setDisp12MaxDiff(disp12MaxDiff);

    sbm->compute(left,right,disp);

	disp.convertTo(disp, CV_8U, 255 / (numberOfDisparities * 16.));

	return disp;
}

void BMMethod::createParameterWindow(string name,Size position) {
	if (cvGetWindowHandle(name.c_str())!=NULL)
		destroyWindow(name);
	namedWindow(name,CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	moveWindow(name,position.width,position.height);
	createTrackbar("preFilterCap", name, &preFilterCap, 63);
    createTrackbar("blockSize", name, &blockSize, 254);
	createTrackbar("minDisparity", name, &minDisparity, 30);
	createTrackbar("numberOfDisparities", name, &numberOfDisparities, 200);
	createTrackbar("textureThreshold", name, &textureThreshold, 50);
	createTrackbar("uniquenessRatio", name, &uniquenessRatio, 50);
	createTrackbar("speckleWindowSize", name, &speckleWindowSize, 200);
	createTrackbar("speckleRange", name, &speckleRange, 50);
	createTrackbar("disp12MaxDiff", name, &disp12MaxDiff, 50);
}
