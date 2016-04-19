/*
 * File:   Tentacles.cpp
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
#include "Tentacles.hpp"

struct S_checkpoint{
	Point checkpoints[Checkpoints_Per_Tentacle_Step];
	int x;
	int y;
};

struct S_Tentacle{
  S_checkpoint coordinate[Tentacle_Resolution];
	int crashsegment;
	int crashdistance;
};

S_Tentacle tentacle[Tentacles_Per_Side*2+1];

void generateTentacles(double speed){
	speed=speed*10/Tentacle_Resolution;
	
	for (int steering=-Tentacles_Per_Side; steering<=Tentacles_Per_Side;steering++){
		const int tentacleid=steering+Tentacles_Per_Side;
		double targetsteering=steering*Steering_Max_Angle/Tentacles_Per_Side;
		double heading=0;
		double cursteering=0;
		double x=Tentacle_Input_Image_Width/2;
		double y=Tentacle_Input_Image_Height-1;
	
		tentacle[tentacleid].coordinate[0].x=x;
		tentacle[tentacleid].coordinate[0].y=y;
		
		for (int t=1; t<Tentacle_Resolution;t++){
			double diff=targetsteering-cursteering;
			if (diff>Max_Steering_Change/Tentacle_Resolution)
				diff=Max_Steering_Change/Tentacle_Resolution;
			if (diff<-Max_Steering_Change/Tentacle_Resolution)
				diff=-Max_Steering_Change/Tentacle_Resolution;
			cursteering=cursteering+diff;
			heading=heading+cursteering/Tentacle_Resolution*Steering_Amplifier;
			const double heading_rad=heading* CV_PI/180;
			
			double part_x=sin(heading_rad);
			double part_y=cos(heading_rad);
			y=y-speed*cos(heading_rad);
			x=x+speed*sin(heading_rad);
			
			 tentacle[tentacleid].coordinate[t].x=x;
			 tentacle[tentacleid].coordinate[t].y=y;
			 
			 int checkhalf=Checkpoints_Per_Tentacle_Step/2;
			 double swp=part_x;
			 part_x=part_y;
			 part_y=swp;
			 for (int checkpoint=0; checkpoint<Checkpoints_Per_Tentacle_Step; checkpoint++){
				double factor=(checkpoint-checkhalf)/checkhalf*Vehicle_Width/2;
				tentacle[tentacleid].coordinate[t].checkpoints[checkpoint].x = x-part_x*factor;
				tentacle[tentacleid].coordinate[t].checkpoints[checkpoint].y = y-part_y*factor;
			 }
		}
	}
}

void checkTentacles(Mat obstacles, Point target){
	
}


int main() {
	
	Mat input;
	int key;
	cout << "Hello\n";
	int time_base=getTickCount();
	generateTentacles(20);
	int time_cur=getTickCount();
	double diff=(time_cur-time_base)/getTickFrequency();
	cout << "generating took " << diff << " ms." << endl;
	input = imread("../StereoVision.png");
	
	imshow("Input Image",input);
	while (key!='q')
	{
		key=waitKey(30);
	}
	cout << key << endl;;
}
