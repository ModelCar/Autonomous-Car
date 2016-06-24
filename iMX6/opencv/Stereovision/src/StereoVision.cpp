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

void* backgroundThread() {

}

StereoVision::StereoVision(const bool showDebugWindows,
                           const int m,
                           const std::string in,
                           const std::string ex) : mode(m), show(showDebugWindows) {

    currentSpeed = 0.0;
    currentSteering = 0.0;
    imagewritecount = 0;
    saveImages = false;

    camCalibrator = new CameraCalibrator(show);
    depthSubstraction = new DepthSubstraction(0, show);

    serialDevice = -1;
    //serialDevice = initializeSerialDevice();

    //calibration of cameras if there is need
    if (mode == 1) {
        camCalibrator->calibrate(calibImagesPath,in,ex);
    }
    else if (mode == 2) {
        //TODO: integrate new images to calibration like in mode 1
        camCalibrator->createNewCalibrationImages("../test/",1,2);
        //For now just return
        return;
    }

    //get camera calibration parameters
    depthSubstraction->extractCalibrationParams(in,ex);

    //images showing settings
    image_size = Size(320,240);

    //default method
    //0 = Block matching algorithm (BM)
    //1 = Semi-global block matching algorithm (SGBM)
    methodNr = 0;

    if(show) {
        //set windows to show
        cvNamedWindow("disparity",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
        cvMoveWindow("disparity",0,0);
        cvNamedWindow("left",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
        cvMoveWindow("left",0,image_size.height);
        cvNamedWindow("right",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
        cvMoveWindow("right",image_size.width,image_size.height);
        createTrackbar("method", "disparity", &methodNr, depthSubstraction->methodCount-1);
        cvNamedWindow("top_view",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
        cvMoveWindow("top_view",image_size.width,0);
    }
}

StereoVision::~StereoVision() {
    delete camCalibrator;
    delete depthSubstraction;
}

void StereoVision::run() {

    //Adjust index camera
    leftcamera = 1;
    rightcamera = 2;

    cout << "Measuring Tentacle generation: " << endl;
    clock_t begin = clock();
    vector<S_Tentacle> initialTentacles = tentacles.generateTentacles(top_view.size().width,top_view.size().height,30, 0);
    clock_t end = clock();
    measureAndPrintTime(begin, end);

    VideoCapture capLeft = VideoCapture(leftcamera);
    VideoCapture capRight = VideoCapture(rightcamera);

    if(!capLeft.open(leftcamera) || !capRight.open(rightcamera))  // check if we succeeded
        exit(1);

    //capLeft.set(CV_CAP_PROP_FRAME_WIDTH,320);
    //capLeft.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    //capRight.set(CV_CAP_PROP_FRAME_WIDTH,320);
    //capRight.set(CV_CAP_PROP_FRAME_HEIGHT,240);

    for(;;)
    {
        vector<S_Tentacle> tens = initialTentacles;
        if( !capLeft.grab() || !capRight.grab())
        {
            cout << "Can not grab images." << endl;
            exit(1);
        }

        if (capLeft.retrieve(left_frame,0)){

            if (capRight.retrieve(right_frame,1)) {
                //get depth map
                //digitalWrite(LED, HIGH); //On

                cout << "Measuring depth map creation: " << endl;
                begin = clock();
                depth_map = depthSubstraction->getDepthMap(left_frame, right_frame, methodNr);
                end = clock();
                measureAndPrintTime(begin,end);
                //get top view

                cout << "Measuring top view generation: " << endl;
                begin = clock();
                top_view = depthSubstraction->getTopView(depth_map);
                end = clock();
                measureAndPrintTime(begin,end);
                //TODO: retrieve current steering and speed from car before generating tentacles

                //TODO: decide if it is necessary to generate the tentacles with different initial angles

                cout << "Measuring tentacle checking: " << endl;
                begin = clock();
                tentacles.checkTentacles(top_view, tens);
                end = clock();
                measureAndPrintTime(begin,end);

                //digitalWrite(LED, LOW); // Off

                cout << "Measuring steering angle finding: " << endl;
                begin = clock();
                if (!tentacles.findNewSteeringAngle(currentSteering, tens)) {
                    //TODO: no new steering angle found! Crash imminent! Do Emergency break
                    if (serialDevice != -1) {
                        sendsteeringspeed(serialDevice, 0, 0);
                    }

                } else {
                    cout << "Found new steering angle!" << currentSteering << endl;
                    //TODO: maybe it is enough to just see if the 0 angle is a safe path, instead of deciding to drive left or right
                    //driveRight = currentSteering < 0;
                    if (serialDevice != -1) {

                        sendsteeringspeed(serialDevice, (float) currentSpeed, (float) currentSteering);
                    }
                    currentSteering = 0.0;
                }
                end = clock();
                measureAndPrintTime(begin,end);

                //show images
                if (show) {
                    tentacle_map = tentacles.renderTentacles(top_view,tens);
                    showImages();
                }

                if(saveImages) {
                    writeImages();
                }
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
    imshow("top_view",top_view);
    imshow("tentacles",tentacle_map);
    waitKey(30);
}

void StereoVision::writeImages() {

    string imageID = to_string(imagewritecount);
    imwrite("../images/left_rectified"+ imageID + ".bmp", left_frame);
    imwrite("../images/depth_map" + imageID + ".bmp", depth_map);
    imwrite("../images/tentacles" + imageID + ".bmp", tentacle_map);
    imagewritecount++;
}

int StereoVision::initializeSerialDevice() {
    exec(cmd1);
    exec(cmd2);
    wiringPiSetup();
    pinMode(LED, OUTPUT);
    return SerialInit(19200);
}

double StereoVision::measureAndPrintTime(clock_t begin, clock_t end) {
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Time: " << elapsed_secs << " elapsed" << endl;
}