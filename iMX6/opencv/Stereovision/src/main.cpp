#include "../inc/main.hpp"

using namespace std;
using namespace cv;

cv::Mat top_view( 480, 640, CV_8UC1,cv::Scalar(0));

StereoVision::StereoVision(const int m,const string in,const string ex) {

	mode = m;

	//calibration of cameras if there is need
	if (mode == 1) {
		cl.calibrate(calibImagesPath,INTRINSICS,EXTRINSICS);
	}

	//get camera calibration parameters
	ds.extractCalibrationParams(in,ex);

	//images showing settings
	image_size = Size(320,240);

	//default method
	//0 = Block matching algorithm (BM)
	//1 = Semi-global block matching algorithm (SGBM)
	methodNr = 0;

	//set windows to show
	cvNamedWindow("disparity",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	cvMoveWindow("disparity",0,0);
	cvNamedWindow("left",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	cvMoveWindow("left",0,image_size.height);
	cvNamedWindow("right",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	cvMoveWindow("right",image_size.width,image_size.height);
	createTrackbar("method", "disparity", &methodNr, ds.methodCount-1);
	cvNamedWindow("top_view",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	cvMoveWindow("top_view",image_size.width,0);
}

void StereoVision::run() {

	//Adjust index camera
	leftcamera = 1;
	rightcamera = 2;

	VideoCapture capLeft = VideoCapture(leftcamera);
	VideoCapture capRight = VideoCapture(rightcamera);

	if(!capLeft.open(leftcamera) || !capRight.open(rightcamera))  // check if we succeeded
		exit(1);

	for(;;)
	{
		if( !capLeft.grab() || !capRight.grab())
		{
			cout << "Can not grab images." << endl;
			exit(1);
		}

		if (capLeft.retrieve(left_frame,0)){
			;
			if (capRight.retrieve(right_frame,1)){
				//get depth map
				depth_map = ds.getDepthMap(left_frame,right_frame, methodNr);

				//get top view
				top_view = ds.getTopView(depth_map);

				//show images
				showImages();
			}
		}

		if(waitKey(30) >= 0) break; // ID for the key to exit the program
	}

}

void StereoVision::showImages(){
	Mat left;
	Mat right;

	//show images and trackbars
	resize(left_frame, left, image_size);
	resize(right_frame, right, image_size);
	resize(depth_map, depth_map, image_size);
	resize(top_view, top_view, image_size);

	imshow("left", left);
	imshow("right", right);
	imshow("disparity", depth_map);
	imshow("top_view",top_view);

	waitKey(30);
}

int main(int argc, char *argv[]) {

	StereoVision* sv;

	if  (argc == 3) {
		sv = new StereoVision(atoi(argv[1]), argv[2]);
	} else if (argc == 2) {
		sv = new StereoVision(atoi(argv[1]));
	} else {
		sv = new StereoVision();
	}

	sv->run();

	return 0;
}

