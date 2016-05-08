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

    cv::Ptr<cv::StereoSGBM> sgbm /*= StereoSGBM::create()*/;

    sgbm->setP1(8 * left.channels() * sgbm->getBlockSize() * sgbm->getBlockSize());
    sgbm->setP2(32 * left.channels() * sgbm->getBlockSize() * sgbm->getBlockSize());

    sgbm->setMinDisparity(minDisparity);

    sgbm->setBlockSize(blockSize < 5 ? blockSize = 5 :
            blockSize % 2 == 0 ? ++blockSize : blockSize);

    sgbm->setNumDisparities(numberOfDisparities < 16 ? 16 :
                            numberOfDisparities % 16 == 0 ?
                            numberOfDisparities : numberOfDisparities &= -16);

    sgbm->setUniquenessRatio(uniquenessRatio);
    sgbm->setSpeckleWindowSize(speckleWindowSize);
    sgbm->setSpeckleRange(speckleRange);
    sgbm->setDisp12MaxDiff(disp12MaxDiff);

    //TODO: mode is different from fullDP parameter, need to adjust it accordingly
    //MODE_HH seems to be the fullDP pendant
    sgbm->setMode(StereoSGBM::MODE_HH);

    sgbm->compute(left,right,disp);

	disp.convertTo(disp, CV_8U, 255 / (numberOfDisparities * 16.));

	return disp;
}

void SGBMMethod::createParameterWindow(string name,Size position) {
	if (cvGetWindowHandle(name.c_str()) != NULL)
		destroyWindow (name);
	namedWindow(name, CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	moveWindow(name, position.width, position.height);

    createTrackbar("blockSize", name, &blockSize, 254);
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
