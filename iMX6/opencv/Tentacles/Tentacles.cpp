/*
 * File:   Tentacles.cpp
 * Author: Roger
 *
 * 
 */


#include <thread>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#include "Tentacles.hpp"

struct S_checkpoint{
	vector<Point> checkpoints;
	int x_coord;
	int y_coord;
};

struct S_Tentacle{
  vector<S_checkpoint> coordinates;
	int crashsegment;
	int crashdistance;
};

vector<S_Tentacle> Tentacles::generateTentacles(double speed) {
	vector<S_Tentacle> tentacles;
	speed=speed*10.0/tentacleResolution;
	
	for (int steering=-tentaclesPerSide; steering<=tentaclesPerSide;steering++){
		double targetsteering=steering*maxSteeringAngle/tentaclesPerSide;
		double heading=0;
		double cursteering=0;
		double x=inputImageWidth/2;
		double y=inputImageHeight-1;
		S_Tentacle tentacle;
        S_checkpoint firstCoord;
        firstCoord.x_coord = x;
        firstCoord.y_coord = y;
		tentacle.coordinates.push_back(firstCoord);
		
		for (int t=1; t<tentacleResolution;t++){
			double diff=targetsteering-cursteering;
			if (diff > maxSteeringChange/double(tentacleResolution))
				diff = maxSteeringChange/double(tentacleResolution);
			if (diff < -maxSteeringChange/double(tentacleResolution))
				diff = -maxSteeringChange/double(tentacleResolution);
			cursteering=cursteering+diff;
			heading=heading+cursteering/tentacleResolution*steeringAmplifier;
			const double heading_rad=heading* CV_PI/180;
			
			double part_x=sin(heading_rad);
			double part_y=cos(heading_rad);
			y=y-speed*cos(heading_rad);
			x=x+speed*sin(heading_rad);

            S_checkpoint coordinate;
            coordinate.x_coord = x;
            coordinate.y_coord = y;
			 
			 int checkhalf=checkpointsPerTentacleStep/2;
			 double swp=part_x;
			 part_x=part_y;
			 part_y=swp;
			 for (int checkpoint=0; checkpoint<checkpointsPerTentacleStep; checkpoint++){
				double factor=(checkpoint-checkhalf)/checkhalf*vehicleWidth/2;
                 Point p(x-part_x*factor, y-part_y*factor);
                 coordinate.checkpoints.push_back(p);
			 }
            tentacle.coordinates.push_back(coordinate);
		}
		tentacles.push_back(tentacle);
	}
	return tentacles;
}

void Tentacles::checkTentacles(Mat obstacles, Point target){
	//TODO: check plain input image (Mat obstacles) with a point from checkpoints in struct S_checkpoints
    //If input image at Point coordinate is white, then collision
}


int main() {
	
	Mat input;
	int key;
	cout << "Hello\n";
	int time_base=getTickCount();
	Tentacles tentacles;
	vector<S_Tentacle> tens = tentacles.generateTentacles(20);
	int time_cur=getTickCount();
	double diff=(time_cur-time_base)/getTickFrequency();
	cout << "generating took " << diff << " ms." << endl;
	input = imread("Stereovision.png");

	for(S_Tentacle ten : tens) {
        //TODO: replace with vector iteration e.g. "blabla for blas.begin; bla != blas.end; bla++ ... if(bla != blas.end)
		for (int i = 0; i < tentacleResolution; i++) {
			if(i+1 < tentacleResolution) {
				Point p1(ten.coordinates.at(i).x_coord,ten.coordinates.at(i).y_coord);
				Point p2(ten.coordinates.at(i+1).x_coord, ten.coordinates.at(i+1).y_coord);
				line(input, p1, p2, Scalar(0, 255, 0, 255));
			}
		}
	}
	imshow("Input Image",input);
	while (key!='q')
	{
		key=waitKey(30);
	}
	cout << key << endl;
}
