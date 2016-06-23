#include "../inc/depthsubstraction.hpp"
using namespace std;
using namespace cv;

double QMatrix[] = { 1., 0., 0., -9.8299119567871094e+02, 0., 1., 0.,
		-5.8960412216186523e+02, 0., 0., 0., 2.2548571731734496e+03, 0., 0.,
		4.3534442158719244e-01, 0. };

//variable for getTopView()
cv::Mat top(480, 640, CV_8UC1, cv::Scalar(0) );

DepthSubstraction::DepthSubstraction(int m, bool t) {
	undist = false;
	showDebugWindows = t;
	methodNr = -1;
	setMethod(m);
	methodNr = m;
    pixel_width = 640; //320 * 2 pixels
    pixel_height = 480; //240 * 2 pixels
    view_top = 160; //where the pixel analysis of the depth map begins
    view_bottom = 360; //where the pixel analysis of the depth map ends
    white = 255; //in grayscale CV_8UC1
    black = 0; //in grayscale CV_8UC1
    windowSize = 20;
    sum = 0;
}

DepthSubstraction::~DepthSubstraction() {
    delete method;
}
void DepthSubstraction::extractCalibrationParams(string in, string ex) {
	//Load calibration files
	//reading intrinsic parameters
	FileStorage fs(in, CV_STORAGE_READ);
	if (!fs.isOpened()) {
		printf("Failed to open file %s\n", in.c_str());
		createQMatrix();
		return;
	}

	fs["M1"] >> M1;
	fs["D1"] >> D1;
	fs["M2"] >> M2;
	fs["D2"] >> D2;

	//reading extrinsic parameters
	fs.open(ex, CV_STORAGE_READ);
	if (!fs.isOpened()) {
		printf("Failed to open file %s\n", ex.c_str());
		createQMatrix();
		return;
	}

	fs["R"] >> R;
	fs["T"] >> T;
	fs["R1"] >> R1;
	fs["R2"] >> R2;
	fs["P1"] >> P1;
	fs["P2"] >> P2;
	fs["Q"] >> Q;
	undist = true;
}

void DepthSubstraction::undistort(Mat& left, Mat &right) {
	if (undist) {
		Mat map11, map12, map21, map22;
		initUndistortRectifyMap(M1, D1, R1, P1, left.size(), CV_16SC2, map11,
				map12);
		initUndistortRectifyMap(M2, D2, R2, P2, left.size(), CV_16SC2, map21,
				map22);

		Mat img1r, img2r;
		remap(left, img1r, map11, map12, INTER_LINEAR);
		remap(right, img2r, map21, map22, INTER_LINEAR);

		img1r.copyTo(left);
		img1r.copyTo(undistorted);
		img2r.copyTo(right);
	} else {
		left.copyTo(undistorted);
	}
}

Mat DepthSubstraction::getDepthMap(Mat left, Mat right, int newMethod) {

	setMethod(newMethod);

	//rectify the images
	undistort(left, right);

	//get depth map with the choosen method
	depth_map = method->getDisparity(left, right);

	return depth_map;
}

//TODO: TopView should present depth with always the same scale. If an object is too far away, it should not be in the top view
Mat DepthSubstraction::getTopView(Mat depth_map) {

	//size of depth map pixels is 640*480
	//only 80 to 160 pixels, this avoid the ground and the roof being look at like an object
	for (int j = 0; j < pixel_width; j++) {
		saved_value = 0;

		for (int i = view_top; i < view_bottom; i++) {
			Scalar intensity = depth_map.at<uchar>(i, j);
			saved_value += intensity.val[0];
		}
		//scaled so the intensity value from 0 to 255 fits into the 0 to 480 pixels of the frame
		intensity_average[j] = (saved_value / view_top) * pixel_height / white;
	}

	//Noise data reduction with a moving average
	for (int i = 0; i <= (pixel_width - windowSize); i++) {
		//reinitialize sum at zero
		sum = 0;
		for (int j = i; j < i + windowSize; j++) {
			sum += intensity_average[j];
		}
		intensity_smooth[i] = sum / windowSize;
	}

	//fill in the top view image with black and white values that represent the distance
	for (int y = 0; y < pixel_height; y++) {
		for (int x = 0; x < pixel_width; x++) {
			top.at<uchar>(y,x) = (y <= intensity_smooth[x]) ? white : black;
		}
	}

	return top;
}



void DepthSubstraction::setMethod(int newMethod) {
	if (methodNr == newMethod || methodNr >= methodCount) {
		return;
	}

	if (method != NULL) {
		method = nullptr;
	}

	//choose the method
	methodNr = newMethod;
	switch (methodNr) {
		case 0:
			method = new BMMethod(showDebugWindows);
			break;
		case 1:
			method = new SGBMMethod(showDebugWindows);
			break;
		}
	if (undist && methodNr == 0) {
			method->setRoi(roi1, roi2);
		}
}

void DepthSubstraction::createQMatrix() {
	Mat Qmatrix(Size(4, 4), DataType<float>::type);
	Qmatrix.copySize(Q);
	Q = Scalar(0);
	for (int y = 0; y < Q.cols; y++) {
		for (int x = 0; x < Q.rows; x++) {
			Q.at<float>(y, x) = QMatrix[y * 4 + x];
		}
	}
}
