#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <signal.h>
#include <sys/stat.h>
using namespace cv;
using namespace std;       
#include "line_detection.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h> // for checking if file exists with access()
#include "client.h"
#include "communication.h" // serial communication


Mat sleft;
Mat sright;
Mat top;
int quit=0;
int ready=0;
const bool print_info=true; // wether algorithms should print individual information
const bool plot=true;  // print matches and calibration_stripe_placement on image (blue to black, upper corners)
const bool save_individual=false;
const bool show_windows=false;
const bool client=true;
const bool serial_communication=true;
// const bool 


Mutex f1;


void capture() {
	float brightness=0.3;
	VideoCapture stream2(0); 
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


int main(int argc, char** argv) {
	
	
	
	signal (SIGTERM, signal_quit);
	signal (SIGINT, signal_quit);
	
	
	int type=16;      // type of image matrices
	char numstr[50];  // used for saving images to directory
	char dirstr[30];
	char *hostname;
	char *serialPort;
	int serialport_switch_counter;
	int ClientToServerfd;
	driveinfo info;
	int stopcountdown=0;
	
	// Matrices for stitching
	Mat maptop;
	
	// init code for signs
	// init code for line_detection
	OpencvLineDetection lines;
	
	cout << "Hello\n";
	// 	thread t0(capture);
	
	if (client) {
		if (access("/dev/ttymxc1",F_OK) != -1){
			hostname=(char*)"10.42.0.254";
			serialPort=(char*)"/dev/ttymxc1";
		}
		else{
			hostname=(char*)"localhost";
			serialPort=(char*)"/dev/tnt1";
		}
		
		if (argc < 2) {
			fprintf(stderr,"usage %s hostname for specific server, trying %s\n", argv[0],hostname);
		} else {
			hostname=argv[1];
		}
		
		ClientToServerfd = netConnect(hostname,8000);	
	}
	
	if (serial_communication) {
		uart0_setup(serialPort);
	}
	
	// 	cout << "waiting for camera to initialize" << endl;
	// 	while (1) {
	// 		f1.lock();
	// 		cout << 1-top.empty() << "\r" << flush;
	// 		if ( top.empty()==false) { //TODO: change ones set to true back to false!!!
	// 			f1.unlock();
	// 			break;
	// 		}
	// 		f1.unlock();
	// 		this_thread::sleep_for(chrono::milliseconds(100));
	// 	}
	// 	cout << "\n";
	
	
	
	
	
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
	
	// get frames in pace of fps
	int64 time_cur,time_base = getTickCount();
	int fps=30;
	int counter=0;  // framecounter
	
	while (true) {
		time_cur= getTickCount();
		if (fps*(time_cur-time_base)/getTickFrequency()>1) {
			time_base=time_cur;
			
			// ## aquire all three images
			// first top to start detection on it, while others are aquired
			// 			f1.lock();
			// 			top.copyTo(maptop);
			// 			f1.unlock();
			// 			cout << "before " << fps*(getTickCount()-time_base)/getTickFrequency() << endl;
			// ## start detection threads
			// no need for mutexes, they use independent resources
			
			// 			cout << "after " << fps*(getTickCount()-time_base)/getTickFrequency() << endl;
			//  			rectangle(plottarget,Point(0,0),Point(319,239),Scalar(0, 0, 0),CV_FILLED);
			
			if (print_info) {
				
			}
			
			
			Mat plottarget=maptop;
			
			if (plot){
				// rectangle(plottarget,Point(0,0),Point(319,239),Scalar(0, 0, 0),CV_FILLED); // clear plottarget?
				
			}
			
			if (client){
				netReceiveInfo(ClientToServerfd,&info);
				if (print_info)
					cout << "Here is the message: speed="<<info.speed << ", stop="<< info.stop << ", attention="<< info.attention << ", rightway="<< info.rightofway << ", trafficLight="<< info.trafficlight << endl;
				if (serial_communication){
					serialport_switch_counter++;
					if (serialport_switch_counter%2==0){
						if (info.stop || info.trafficlight>1)
							sendSpeed(0);
						else 
							sendSpeed(info.speed);
					}
					if (serialport_switch_counter%2==1){
						if (info.rightofway)
							sendSteering(100);
						else 
							sendSteering(0);
					}
				}
			}
			
			
			
			// **** lane detection ***** //
			/*cout << "before Line" << fps*(getTickCount()-time_base)/getTickFrequency() << endl;
			 *		Mat im_gray;
			 *		cvtColor(maptop,im_gray,CV_RGB2GRAY);
			 *		cout << "." <<lines.searchLine(im_gray) << flush;*/
			// 			cout << "after Line" << fps*(getTickCount()-time_base)/getTickFrequency() << endl;
			
			/* ##### include if you want single images in dir ##### */
			if (save_individual) {
				counter++;
				sprintf(numstr, "%s/frame%05d.jpg", dirstr,counter);
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
	// 	t0.join();
	return 0;
}