#include <iostream>
#include "opencv2/opencv.hpp"

#include "line_detection.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    string path = argv[1];
    VideoCapture cap;
    cap.open(path);
    if(!cap.isOpened()){  // check if we succeeded
        cout << "Could not open video." << endl;
        return -1;
    }
    cout << "Start Line Detection" << endl;
    OpencvLineDetection lines;
    cout << "Width:" << cap.get(CV_CAP_PROP_FRAME_WIDTH) << "\nHeight:" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    namedWindow("Frame",1);
    Mat frameR;
    //namedWindow("cp",1);
    //namedWindow("contours",1);
    //namedWindow("canny",1);
    u_int64_t cycle = 0;

    //VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),25, Size(480,270),true);

    for(;;)
        {
            ++cycle;
            Mat frame, frameGRAY;
            cap >> frame; // get a new frame from camera
            //cvtColor(frame, frameGRAY, CV_BGR2GRAY);
            resize( frame, frameR, Size(320,240), 1, 1 ) ;
            //cout << "cols" << frameR.cols << "rows" << frameR.rows << endl;
            lines.searchLine(frameR,cycle);
            imshow("Frame", frameR);
            int k = waitKey(30);
            // Hold p for pause
            while(k == 'p') {
                cout << "key "<< k << endl;
                if(k == 'p'){
                    k = waitKey(500);
                }else{
                    break;
                }

            }
            //video.write(lines.video);
            //if(k >= 0) break;

    }

    return 0;
}

