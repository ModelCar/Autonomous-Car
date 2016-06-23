//
// Created by simonso on 13.06.16.
//

#include "../inc/StereoVision.hpp"
using namespace std;
using namespace cv;
//TODO: topview muss noch sauber deklariert werden und nicht hier einfach so rumfliegen
cv::Mat top_view( 480, 640, CV_8UC1,cv::Scalar(0));


string StereoVision::calibImagesPath = "../calibration";
string StereoVision::INTRINSICS = "../calibration/intrinsics.yml";
string StereoVision::EXTRINSICS = "../calibration/extrinsics.yml";

StereoVision::StereoVision(const int m,
                           const std::string in,
                           const std::string ex) : mode(m) {

    currentSpeed = 0.0;
    currentSteering = 0.0;

    serialDevice = initializeSerialDevice();
    //calibration of cameras if there is need
    if (mode == 1) {
        camCalibrator.calibrate(calibImagesPath,in,ex);
    }
    else if (mode == 2) {
        //TODO: integrate new images to calibration like in mode 1
        camCalibrator.createNewCalibrationImages("../test/",1,2);
        //For now just return
        return;
    }

    //get camera calibration parameters
    depthSubstraction.extractCalibrationParams(in,ex);

    //images showing settings
    image_size = Size(320,240);

    //default method
    //0 = Block matching algorithm (BM)
    //1 = Semi-global block matching algorithm (SGBM)
    methodNr = 0;

    //set windows to show
    //cvNamedWindow("disparity",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
    //cvMoveWindow("disparity",0,0);
    //cvNamedWindow("left",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
    //cvMoveWindow("left",0,image_size.height);
    //cvNamedWindow("right",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
    //cvMoveWindow("right",image_size.width,image_size.height);
    //createTrackbar("method", "disparity", &methodNr, depthSubstraction.methodCount-1);
    //cvNamedWindow("top_view",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
    //cvMoveWindow("top_view",image_size.width,0);
}

void StereoVision::run() {

    //Adjust index camera
    leftcamera = 1;
    rightcamera = 2;
    bool driveRight = false;

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

            if (capRight.retrieve(right_frame,1)){
                //get depth map
                digitalWrite (LED, HIGH); //On
                depth_map = depthSubstraction.getDepthMap(left_frame,right_frame, methodNr);

                //get top view
                top_view = depthSubstraction.getTopView(depth_map);

                //TODO: retrieve current steering and speed from car before generating tentacles

                //TODO: decide if it is necessary to generate the tentacles with different initial angles
                vector<S_Tentacle> tens = tentacles.generateTentacles(top_view.size().width,top_view.size().height,30, 0);
                tentacles.checkTentacles(top_view,tens);

                digitalWrite(LED, LOW); // Off

                if(!tentacles.findNewSteeringAngle(currentSteering, tens, driveRight)) {
                    //TODO: no new steering angle found! Crash imminent! Do Emergency break
                    if(serialDevice != -1) {
                        sendsteeringspeed(serialDevice, 0, 0);
                    }

                } else {
                    cout << "Found new steering angle!" << currentSteering << endl;
                    //TODO: maybe it is enough to just see if the 0 angle is a safe path, instead of deciding to drive left or right
                    //driveRight = currentSteering < 0;
                    if(serialDevice != -1) {
                        sendsteeringspeed(serialDevice, currentSpeed, currentSteering);
                    }
                    currentSteering = 0.0;
                }

                //tentacle_map = tentacles.renderTentacles(top_view,tens);

                //currentSteering = 0;
                //show images
                //showImages();
            }
        }

        //if(waitKey(30) >= 0) break; // ID for the key to exit the program
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
    resize(tentacle_map, tentacle_map, image_size);

    imshow("left", left);
    imshow("right", right);
    imshow("disparity", depth_map);
    //imshow("top_view",top_view);
    imshow("tentacles",tentacle_map);

    waitKey(30);
}

int StereoVision::initializeSerialDevice() {
    string cmdresult;
    cmdresult = exec(cmd1);
    cout << "command 1 returned: " << cmdresult << endl;
    cmdresult = exec(cmd2);
    cout << "command 2 returned: " << cmdresult << endl;

    if(cmdresult == "") {
        return -1; //return because no serial device is attached.
    }
    wiringPiSetup();
    pinMode(LED, OUTPUT);

    int SerialDevice = SerialInit(19200);

    if(SerialDevice = -1) {
        cout << "WARNING: SerialInit failed!!" << endl;
    }
}