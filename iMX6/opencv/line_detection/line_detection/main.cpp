#include <iostream>
#include "opencv2/opencv.hpp"

#include "line_detection.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    //~ string path = argv[1];
    //~ VideoCapture cap;
    //~ cap.open(path);
    //~ if(!cap.isOpened()){  // check if we succeeded
        //~ cout << "Could not open video." << endl;
        //~ return -1;
    //~ }
    //~ cout << "Start Line Detection" << endl;
    //~ OpencvLineDetection lines;
    //~ cout << "Width:" << cap.get(CV_CAP_PROP_FRAME_WIDTH) << "\nHeight:" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    //~ namedWindow("Frame",1);
    //~ Mat frameR;
    //~ //namedWindow("cp",1);
    //~ //namedWindow("contours",1);
    //~ //namedWindow("canny",1);
    //~ u_int64_t cycle = 0;

    //~ //VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),25, Size(480,270),true);

    //~ for(;;)
        //~ {
            //~ ++cycle;
            //~ Mat frame, frameGRAY;
            //~ cap >> frame; // get a new frame from camera
            //~ //cvtColor(frame, frameGRAY, CV_BGR2GRAY);
            //~ resize( frame, frameR, Size(320,240), 1, 1 ) ;
            //~ //cout << "cols" << frameR.cols << "rows" << frameR.rows << endl;
            //~ lines.searchLine(frameR,cycle);
            //~ imshow("Frame", frameR);
            //~ int k = waitKey(30);
            //~ // Hold p for pause
            //~ while(k == 'p') {
                //~ cout << "key "<< k << endl;
                //~ if(k == 'p'){
                    //~ k = waitKey(500);
                //~ }else{
                    //~ break;
                //~ }

            //~ }
            //~ //video.write(lines.video);
            //~ //if(k >= 0) break;

    //~ }
    cout << "Program started." << endl;
    Mat srcImg, resizedImg;
    OpencvLineDetection lines;
    u_int64_t cycle = 0;
	const char* winname1 = "Blurred Image";
    const char* winname2 = "Canny Image";
	const char* winname3 = "Drawed Contours Image";
	const char* winname4 = "Result Image";
	
    
    srcImg = imread(argv[1]);
    if(srcImg.empty()) {
	    cout << "No image loaded!" << endl;
	    return -1;
    }
    resize(srcImg, resizedImg, Size(640, 480), 1, 1);
	imwrite("resizedImg.jpg", resizedImg);
    cout << "Searching lines." << endl;
    lines.searchLine(resizedImg, cycle);
    cout << "Search for lines completed." << endl;
	imwrite("roiImg.jpg", lines.roiImg);
	namedWindow(winname1, WINDOW_AUTOSIZE);
    imshow(winname1, lines.blurImg);
	imwrite("blurImg.jpg", lines.blurImg);
    namedWindow(winname2, WINDOW_AUTOSIZE);
    imshow(winname2, lines.cannyImg);
	imwrite("cannyImg.jpg", lines.cannyImg);
	namedWindow(winname3, WINDOW_AUTOSIZE);
    imshow(winname3, lines.drawing);
	imwrite("contoursImg.jpg", lines.drawing);
	namedWindow(winname4, WINDOW_AUTOSIZE);
    imshow(winname4, lines.resultImg);
	imwrite("resultImg.jpg", lines.resultImg);
    
    //~ //Test notALine
    //~ vector<Point2i> vPoints(4);
    //~ for (size_t i = 0; i < vPoints.size(); ++i)
        //~ vPoints[i] = Point2i((int)(i * 5), (int)(i % 7));
    //~ cout << "A vector of 2D Points = " << vPoints << endl << endl;

    //~ bool result = notALine(vPoints);
    //~ cout << "test vector is not a line: " << result << endl;
    
    waitKey(0);

    return 0;
}

