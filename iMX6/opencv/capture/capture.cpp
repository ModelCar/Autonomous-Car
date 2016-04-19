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

Mat sleft;
Mat sright;
Mat top;
int quit=0;
int ready=0;



Mutex f1,f2,f3;



void capture0(){
	VideoCapture stream0(0); 
	while(quit==0){
		if(stream0.isOpened())
			break;
		cout << "retrying to open video capture 0" << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
		stream0.open(0);
	}
	/* set camera properties */
	stream0.set(CV_CAP_PROP_FPS,30);
	stream0.set(CV_CAP_PROP_FRAME_WIDTH,320);
	stream0.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	Mat frametmp;
	
	while(quit==0){
		this_thread::sleep_for(chrono::milliseconds(3));
		stream0.read(frametmp);
		f1.lock();
		frametmp.copyTo(sleft);
		f1.unlock();
	}
	stream0.release();
}

void capture1(){
	VideoCapture stream1(1);
	while(quit==0){
		if(stream1.isOpened())
			break;
		cout << "retrying to open video capture 1" << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
		stream1.open(1);
	}
	stream1.set(CV_CAP_PROP_FPS,30);
	stream1.set(CV_CAP_PROP_FRAME_WIDTH,320);
	stream1.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	Mat frametmp;
	
	while(quit==0){
		this_thread::sleep_for(chrono::milliseconds(3));
		stream1.read(frametmp);
		f2.lock();
		frametmp.copyTo(sright);
		f2.unlock();
	}
	stream1.release();
}

void capture2(){
	VideoCapture stream2(2);
	while(quit==0){ 
		if(stream2.isOpened())
			break;
		cout << "retrying to open video capture 2" << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
		stream2.open(1);
		
	}
	stream2.set(CV_CAP_PROP_FPS,30);
	stream2.set(CV_CAP_PROP_FRAME_WIDTH,320);
	stream2.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	Mat frametmp;
	
	while(quit==0){
		this_thread::sleep_for(chrono::milliseconds(3));
		stream2.read(frametmp);
		f3.lock();
		frametmp.copyTo(top);
		f3.unlock();
	}
	stream2.release();
}

void threaded_imsave(char destination[], Mat image){
	// Mat is just a reference, we need to save our own copy before the main thread aquires a new frame.
	Mat internal;
	image.copyTo(internal);
	imwrite(destination,internal);
}
	


void signal_quit(int num) 
{
	// we might use this handler for many signals
	switch (num)
	{
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
	cout << "Hello\n";
	thread t1(capture0);
	thread t2(capture1);
	thread t3(capture2);
	  
	int type=16;
	Mat carview(240, 960, type);
	Mat mapleft(carview, cv::Rect(0,0, 320, 240));
	Mat mapright(carview, cv::Rect(320,0, 320, 240));
	Mat maptop(carview, cv::Rect(640,0, 320, 240));
	// video writer fourCCs:  DIVX mpg2 h264 h265  3IVD XVID MPEG
	// VideoWriter video("out.avi",CV_FOURCC('X','V','I','D'),30, Size(960,240),true);
	
	cout << "waiting for cameras to initialize" << endl;
	while (1){
		f1.lock();
		f2.lock();
		f3.lock();
		cout << 1-sleft.empty() << "/" << 1-sright.empty() << "/" << 1-top.empty() << "\r" << flush;
		if (sleft.empty()==false && sright.empty()==false && top.empty()==false){
			f3.unlock();
			f2.unlock();
			f1.unlock();
			break;
		}
		f3.unlock();
		f2.unlock();
		f1.unlock();
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	cout << "\n";
	
	
	
	
	// write frames to videowriter in pace of fps
	int64 time_cur,time_base = getTickCount();
	int fps=30;
	/* ##### include if you want single images in dir ##### */
	int counter=0;
	char numstr[50];
	struct stat st ={0};
	char dirstr[30];
	chrono::time_point<chrono::system_clock> start;
	start = chrono::system_clock::now();
	time_t start_time = chrono::system_clock::to_time_t(start);
	tm utc_tm = *gmtime(&start_time);
	sprintf(dirstr, "out_%i_%i_%i", utc_tm.tm_hour,utc_tm.tm_min,utc_tm.tm_sec);
	// Create output dir
	if (stat(dirstr, &st) == -1) {
		mkdir(dirstr, 0700);
	}
	/* ######## include end ####### */
	
	
	while (true) {
		time_cur= getTickCount();
		if (fps*(time_cur-time_base)/getTickFrequency()>1) {
			time_base=time_cur;
			
			// aquire all three images
			f1.lock();
			sleft.copyTo(mapleft);
			f1.unlock();
			f2.lock();
			sright.copyTo(mapright);
			f2.unlock();
			f3.lock();
			top.copyTo(maptop);
			f3.unlock();
			
			/* ##### include if you want single images in dir ##### */
			counter++;
			sprintf(numstr, "%s/frame%d.jpg", dirstr,counter);
			imwrite(numstr,carview);
			/* ######## include end ####### */
			// Each image will be saved in an own thread to utilize all cores
			thread(threaded_imsave,numstr,carview).detach();
			//video.write(carview);
		} else {
			this_thread::sleep_for(chrono::milliseconds(1));
		}
		// 		imshow("car",carview);
		// 		if (waitKey(30) >= 0)
		// 			break;
	}
	// 	video.release();
	quit=1;
	t1.join();
	t2.join();
	t3.join();
	return 0;
}