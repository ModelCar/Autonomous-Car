/*
 * File:   capture.cpp
 * Author: Roger
 *
 * 
 */


#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
#include "TrafficLights.hpp"
#include "Signs.hpp"           
#include "line_detection.h"

Mat frame_left;
Mat frame_right;
Mat frame_top;
int quit=0;
int pause;

bool printindividual=true; // wether algorithms should print individual information
vector<TrafficLight> lights;
vector<pair<Point,uint> > signs;

Mutex mleft,mright,mtop,mpause;


// Function to input our video like a real time video feed
void capture1(){
	VideoCapture stream1("../input.avi");
	
	
	int type=16;
	double fps=30.0;
	Mat carview(240, 960, type);
	Mat mapleft(carview, cv::Rect(0,0, 320, 240));
	Mat mapright(carview, cv::Rect(320,0, 320, 240));
	Mat maptop(carview, cv::Rect(640,0, 320, 240));
	Mat frame1tmp;
	
	int64 t1,t0 = getTickCount();
	while(quit==0){
		mpause.lock();
		mpause.unlock();
		t1= getTickCount();
		if (fps*(t1-t0)/getTickFrequency()>1) {
			t0=getTickCount();
			stream1.read(carview);
			mleft.lock();
			mapleft.copyTo(frame_left);
			mleft.unlock();
			mright.lock();
			mapright.copyTo(frame_right);
			mright.unlock();
			mtop.lock();
			maptop.copyTo(frame_top);
			mtop.unlock();
		} else {
			//cout << "sleeping " << endl;
			this_thread::sleep_for(chrono::milliseconds(1));
		}

	}
	stream1.release();
}

void signThread(Mat image){			// **** Signs 				***** //
	signs = signDetection(image);
}

void lightsThread(Mat image){
				// ****	Trafficlights ***** //
// 	cout << "Lights Detection Starting\n";
//	lights = DetectLights(image);
// 	cout << "Lights Detection finished\n";
}

/** !!! This is an example function to process the video
 * 
 * Look under aquire images how to get the images for processing them 
 * within your function. This is to provide current images to all functions 
 * without them interfering wth each other or the capture thread
 * 
 */
int stereoprocess(){
	
	// Definitions for images you will process
	Mat myleftimage;
	Mat myrightimage;
	Mat mytopimage;
	// aquire images
	mleft.lock();
	frame_left.copyTo(myleftimage);
	mleft.unlock();
	mright.lock();
	frame_right.copyTo(myrightimage);
	mright.unlock();
	

	
	mtop.lock();
	frame_top.copyTo(mytopimage);
	mtop.unlock();
	
	thread t_signs(signThread,mytopimage);
	thread t_lights(lightsThread,mytopimage);
	t_signs.join();
	t_lights.join();
	
	Mat plottarget=mytopimage;
	
	if (lights.size()>0 && printindividual) 
	{
// 		cout <<"Lights detected :" << lights.size() ;
		for(uint i = 0; i < lights.size() && i < 5; i++)//print out max 5 lights
		{  
// 			cout << (float)lights[i].distance/100<< "m,("<< lights[i].xPos << ", " << lights[i].yPos << "),r=" << lights[i].radius << "  /" ;
			circle( plottarget, Point(lights[i].xPos, lights[i].yPos), 5, Scalar(255, 100, 100), -1, 8, 0 );
			putText(plottarget, lights[i].color, Point(lights[i].xPos, lights[i].yPos), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 150, 150), 4);
		}
		cout << endl;
	}
	if (signs.size()>0 && printindividual) 
	{
// 		cout <<"Signs detected: " << signs.size() << " = ";
		for(uint i = 0; i < signs.size() && i < 5; i++)//print out max 5 signs
		{  
// 			cout << signs[i].first<< "," << signs[i].second << "  /" ; 
			if (signs[i].second != 14){
				circle( plottarget, signs[i].first, 5, Scalar(153, 153, 255), -1, 8, 0 );
				putText(plottarget, to_string(signs[i].second), signs[i].first, FONT_HERSHEY_SIMPLEX, 1, Scalar(200, 200, 255), 4);
			}
		}
		cout << endl;
	}
	
	// do some processing, here we just show the images
	imshow("stereo_left",myleftimage);
	imshow("stereo_right",myrightimage);
	imshow("topcam",mytopimage);
	int key=waitKey(30); 
	if ( key == 112) { // P for pause
		if (pause==0) {
			cout << "Pause" << flush;
 			mpause.trylock();
			cout << "_l" << flush;
			pause=1;
		}	else {
			cout << "\r       \r" << flush;
			pause=0;
 			mpause.unlock();
		}
	} else if (key == 27 || key==113) // ESC & q for quit
		quit=1;
		
}


int main() {
	cout << "Hello\n";
	// in real 1 thread for each camera
	thread t1(capture1);
	
		// init code for signs
	loadReferences();
	// init code for line_detection
	OpencvLineDetection lines;
	
	cout << "Waiting for stream to initialize" << endl;
	while (1){
		mleft.lock();
		mright.lock();
		cout << 1-frame_left.empty() << "/" << 1-frame_right.empty() << "\r" << flush;
		if (frame_left.empty()==false && frame_right.empty()==false){
			mleft.unlock();
			mright.unlock();
			break;
		}
		mleft.unlock();
		mright.unlock();
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	cout << "Stream initialized\n";
	
	
	while (quit==0) {
		
		/** Calling your function
		 * 
		 * Later on, functions will be called as threads and
		 * use mutexes to store their return values in variables
		 * This way one blocking function does not interfere with te other ones
		 * and fast functions can be executed more often than slow ones.
		 * Also this will take full advantage of multi core architectures.
		 * (suggestion, problem: functions also need to set some kind of flag when they
		 * are finished, or I need to use a more advanted threading library than the standard one)
		 */
		stereoprocess();
		
	}
	// here the car will be braked and left in a safe state
	t1.join();
	return 0;
}
