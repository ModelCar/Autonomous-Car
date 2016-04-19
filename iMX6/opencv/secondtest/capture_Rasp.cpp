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
#include <signal.h>
#include <sys/stat.h>
using namespace cv;
using namespace std;
#include "TrafficLights.hpp"
#include "Signs.hpp"           
#include "line_detection.h"
#include "server.cpp"


using namespace cv;
using namespace std;

Mat sleft;
Mat sright;
Mat top;
int quit=0;
int ready=0;
const bool print_info=true; // wether algorithms should print individual information
const bool plot=true;  // print matches and calibration_stripe_placement on image (blue to black, upper corners)
const bool save_individual=false;
const bool show_windows=true;
const bool server=false;
// const bool 
vector<TrafficLight> lights;
vector<pair<Point,uint> > signs;


Mutex f1;


void capture() {
	float brightness=0.3;
	VideoCapture stream2(1); 
	while(quit==0) { 
		if(stream2.isOpened())
			break;
		cout << "retrying to open video capture 2" << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
		stream2.open(1);
		
	}
	stream2.set(CV_CAP_PROP_FPS,30);
	stream2.set(CV_CAP_PROP_FRAME_WIDTH,320);
	stream2.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	stream2.set(CV_CAP_PROP_BRIGHTNESS,brightness);
	stream2.set(CV_CAP_PROP_CONTRAST,0.13);
	stream2.set(CV_CAP_PROP_SATURATION,0.40);
	Mat frametmp;
	
	while(quit==0) {
		this_thread::sleep_for(chrono::milliseconds(3));
// 		cout << "frame read" <<endl ;
		stream2.read(frametmp);
// 		cout << "frame read end" <<endl ;
		// Adjust brightness in a way so colors remain clear
		uint8_t* pixelPtr = (uint8_t*)frametmp.data;
		uint8_t blueofyellow=pixelPtr[233*320*3 + 100*3];
		uint8_t greenofyellow =pixelPtr[233*320*3 + 100*3+1];
// 		cout << "set brightness" <<endl ;
		if (blueofyellow > 20 && brightness>0.03) {
			brightness = brightness - 0.02;
			stream2.set(CV_CAP_PROP_BRIGHTNESS,brightness); 
		}	else if (greenofyellow < 120 && brightness<0.92) {
			brightness = brightness + 0.02;
			stream2.set(CV_CAP_PROP_BRIGHTNESS,brightness);
		}
// 		cout << "set brightness end" <<endl ;
// 		cout << "frame" <<endl ;
		f1.lock();
		frametmp.copyTo(top);
		f1.unlock();
// 		cout << "frame_end" <<endl ;
	}
	stream2.release();
}

void signThread(Mat image) {			// **** Signs 				***** //
	// 	double t0 = (double) getTickCount();
	signs = signDetection(image);
	// 	double t1 = (double) getTickCount();
	// 	cout << "signs took "<<(t1-t0) / getTickFrequency() << endl;
}

void lightsThread(Mat image) {
	// ****	Trafficlights ***** //
	// 	cout << "Lights Detection Starting\n";
	// 	double t0 = (double) getTickCount();
	lights = DetectLights(image);
	// 	double t1 = (double) getTickCount();
	// 	cout << "lights took "<<(t1-t0) / getTickFrequency() << endl;
	// 	cout << "Lights Detection finished\n";
}

void signal_quit(int num) {
	// we might use this handler for many signals
	switch (num) {
		case SIGINT:
		case SIGTERM:
			// clean up code.
			quit=1;
			cout << "SIGTERM/SIGINT, cleaning up\n";
			this_thread::sleep_for(chrono::milliseconds(500));
			exit(0);
			break;
	}
}


int main() {
  

	
	signal (SIGTERM, signal_quit);
	signal (SIGINT, signal_quit);

	
	int type=16;      // type of image matrices
	char numstr[50];  // used for saving images to directory
	char dirstr[30];
	int servertoclientfd;
	driveinfo info;
	int stopcountdown=0, attentioncountdown=0, rightofwayctr=0;
	
	// Matrices for stitching
	Mat maptop;
	
	// init code for signs
	loadReferences();
	// init code for line_detection
	OpencvLineDetection lines;
	
	cout << "Hello\n";
	thread t0(capture);
	if (server)
		servertoclientfd = netCreateSingleBlockingServer(8000);
	
	cout << "waiting for camera to initialize" << endl;
	while (1) {
		f1.lock();
		cout << 1-top.empty() << "\r" << flush;
		if ( top.empty()==false) { //TODO: change ones set to true back to false!!!
			f1.unlock();
			break;
		}
		f1.unlock();
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	cout << "\n";
	
	
	
	
	// get frames in pace of fps
	int64 time_cur,time_base = getTickCount();
	int fps=30;
	int counter=0;  // framecounter
	/* ##### include if you want single images in dir ##### */
	if (save_individual) {
		struct stat st ={0};
		chrono::time_point<chrono::system_clock> start;
		start = chrono::system_clock::now();
		time_t start_time = chrono::system_clock::to_time_t(start);
		tm utc_tm = *gmtime(&start_time);
		sprintf(dirstr, "out_%i_%i_%i", utc_tm.tm_hour,utc_tm.tm_min,utc_tm.tm_sec);
		// Create output dir
		if (stat(dirstr, &st) == -1) {
			mkdir(dirstr, 0700);
		}
	}
	/* ######## include end ####### */
	
	
	while (true) {
		time_cur= getTickCount();
		if (fps*(time_cur-time_base)/getTickFrequency()>1) {
			time_base=time_cur;
			
			// ## aquire all three images
			// first top to start detection on it, while others are aquired
			f1.lock();
			top.copyTo(maptop);
			f1.unlock();
// 			cout << "before " << fps*(getTickCount()-time_base)/getTickFrequency() << endl;
			// ## start detection threads
			// no need for mutexes, they use independent resources
			thread t_signs(signThread,maptop);
 			thread t_lights(lightsThread,maptop);
			
			// ## join detection threads
			t_signs.join();
 			t_lights.join();
// 			cout << "after " << fps*(getTickCount()-time_base)/getTickFrequency() << endl;
			//  			rectangle(plottarget,Point(0,0),Point(319,239),Scalar(0, 0, 0),CV_FILLED);
			
			if (print_info) {
				if (lights.size()>0) {
					cout <<"Lights detected :" << lights.size() ;
					for(uint i = 0; i < lights.size() && i < 5; i++) //print out max 5 lights
						cout << (float)lights[i].distance/100<< "m,("<< lights[i].xPos << ", " << lights[i].yPos << "),r=" << lights[i].radius << ",t=" << lights[i].color << "  /" ;
					cout << endl;
				}
				if (signs.size()>0) {
					cout <<"Signs detected: " << signs.size() << " = ";
					for(uint i = 0; i < signs.size() && i < 5; i++) //print out max 5 signs
						if (signs[i].second != 14)
							cout << signs[i].first<< "," << signs[i].second << "  /" ;   
						cout << endl;
				}
			}
			

			Mat plottarget=maptop;
			
			if (plot){
				// rectangle(plottarget,Point(0,0),Point(319,239),Scalar(0, 0, 0),CV_FILLED); // clear plottarget?
				if (lights.size()>0)
					for(uint i = 0; i < lights.size() && i < 20; i++) {
						//print out max 20 lights
						circle( plottarget, Point(lights[i].xPos, lights[i].yPos), 5, Scalar(255, 100, 100), -1, 8, 0 );
						string trafficlightcolor="";
						trafficlightcolor.push_back(lights[i].color);
						putText(plottarget, trafficlightcolor, Point(lights[i].xPos, lights[i].yPos), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 150, 150), 4);
					}
					
					if (signs.size()>0) 
						for(uint i = 0; i < signs.size() && i < 20; i++) {
							//plot out max 20 signs
							if (signs[i].second != 14) {
								circle( plottarget, signs[i].first, 5, Scalar(153, 153, 255), -1, 8, 0 );
								putText(plottarget, to_string(signs[i].second), signs[i].first, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 4);
							}
						}
						
						circle( plottarget, Point(84,229), 2, Scalar(255, 255, 255), -1, 8, 0 );
					circle( plottarget, Point(143,229), 2, Scalar(255, 255, 255), -1, 8, 0 );	
			}
			
			if (server){
				info.stop=false;
				info.speed=20;
				if (stopcountdown>0)
					stopcountdown--;
				if(attentioncountdown>0)
				  attentioncountdown--;
				if(rightofwayctr>0)
				  rightofwayctr--;
				for(uint i = 0; i < signs.size() && i < 20; i++){
					if (signs[i].second==1)
						stopcountdown=20;
					if (signs[i].second==2) // Yield till speed signs work better
						info.speed=10;
					if(signs[i].second==3)
					  attentioncountdown=20;
					if(signs[i].second==4||signs[i].second==13)
					  rightofwayctr=10;
				}
				if (stopcountdown>0)
					info.stop=true;
				if(attentioncountdown>0)
				  info.speed=15;
				netSendInfo(servertoclientfd,&info);
			}
				
			
			// **** lane detection ***** //
			/*cout << "before Line" << fps*(getTickCount()-time_base)/getTickFrequency() << endl;
			Mat im_gray;
			cvtColor(maptop,im_gray,CV_RGB2GRAY);
			cout << "." <<lines.searchLine(im_gray) << flush;*/
// 			cout << "after Line" << fps*(getTickCount()-time_base)/getTickFrequency() << endl;
			
			/* ##### include if you want single images in dir ##### */
			if (save_individual) {
				counter++;
				sprintf(numstr, "%s/frame%05d__lights_%02lu__signs_%02lu.jpg", dirstr,counter,lights.size(),signs.size());
				imwrite(numstr,maptop);
			}
			/* ######## include end ####### */
			
			if (show_windows) {
				namedWindow( "cartop", WINDOW_NORMAL);
				imshow("cartop",maptop);
				if (waitKey(1) >= 0)
					break;
			}
			
		} else {
			this_thread::sleep_for(chrono::milliseconds(1));
		}
		
					
	}
	// 	video.release();
	quit=1;
	t0.join();
	return 0;
}
