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
#include "../inc/Tentacles.hpp"


//TODO: generate tentacles depending on current steering
vector<S_Tentacle> Tentacles::generateTentacles(int image_width, int image_height, double speed) {
    vector<S_Tentacle> tentacles;
    speed=speed*10.0/tentacleResolution;

    for (int steering=-tentaclesPerSide; steering<=tentaclesPerSide;steering++){
        double targetsteering=steering*maxSteeringAngle/tentaclesPerSide;
        cout << targetsteering << endl;
        double heading=0;
        double cursteering=0;
        double x=image_width/2;
        double y=image_height-1;
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

bool Tentacles::checkTentacles(Mat obstacles, Point target) {
    //TODO: check plain input image (Mat obstacles) with a point from checkpoints in struct S_checkpoints
    //If input image at Point coordinate is white, then collision.
    //Also check all checkpoints around the tentacle, because of vehicle thickness.
    uchar value = obstacles.at<uchar>(target);

    if(value != 255) {
        return false;
    } else {
        return true;
    }
}

Mat Tentacles::renderTentacles(Mat inputImg, vector<S_Tentacle> tentacles) {

    Mat gray_img = inputImg;
    Mat outputImg;

    cvtColor(gray_img, outputImg, CV_GRAY2RGB);

    Scalar red(0, 0, 255, 255);
    Scalar green(0, 255, 0, 255);

    for(S_Tentacle ten : tentacles) {
        Scalar tentacleColor = green;
        for (auto it = ten.coordinates.begin(); it != ten.coordinates.end(); it++ ) {
            for(Point checkpoint : it->checkpoints) {
                if(checkTentacles(gray_img, checkpoint)) {
                    tentacleColor = red;
                    //TODO: mark tentacle as unsafe path
                }
            }
            auto itnext = it++;
            if(itnext != ten.coordinates.end()) {

                Point p1(it->x_coord,it->y_coord);
                Point p2(itnext->x_coord, itnext->y_coord);

                /*if(tentacles.checkTentacles(gray_image,p1) || tentacles.checkTentacles(gray_image, p2)) {
                    tentacleColor = red;
                }*/

                line(outputImg, p1, p2, tentacleColor);
            }
        }
    }
    return outputImg;
}
