/*
 * File:   Tentacles.cpp
 * Author: Roger
 *
 * 
 */


#include <thread>
#include <opencv2/opencv.hpp>
#include "../inc/Tentacles.hpp"

using namespace cv;
using namespace std;

vector<S_Tentacle> Tentacles::generateTentacles(int image_width, int image_height, double speed, double currentSteering) {
    vector<S_Tentacle> tentacles;
    speed=speed*10.0/tentacleResolution;

    for (int steering=-tentaclesPerSide; steering<=tentaclesPerSide;steering++){
        double targetsteering=steering*maxSteeringAngle/tentaclesPerSide;
        double heading=0;
        double cursteering = currentSteering;
        double x=image_width/2;
        double y=image_height-1;
        S_Tentacle tentacle;
        tentacle.steeringAngle = targetsteering;
        tentacle.isSafePath = true;
        S_checkpoint firstCoord;
        firstCoord.x_coord = (int)x;
        firstCoord.y_coord = (int)y;
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
            coordinate.x_coord = (int)x;
            coordinate.y_coord = (int)y;

            int checkhalf=checkpointsPerTentacleStep/2;
            double swp=part_x;
            part_x=part_y;
            part_y=swp;
            for (int checkpoint=0; checkpoint<checkpointsPerTentacleStep; checkpoint++){
                double factor=(checkpoint-checkhalf)/checkhalf*vehicleWidth/2;
                Point p((int)(x-part_x*factor), (int)(y-part_y*factor));
                coordinate.checkpoints.push_back(p);
            }
            tentacle.coordinates.push_back(coordinate);
        }
        tentacles.push_back(tentacle);
    }
    return tentacles;
}

void Tentacles::checkTentacles(cv::Mat obstacles, std::vector<S_Tentacle> &tentacles) {

    leftUnsafeTentacles = 0;
    rightUnsafeTentacles = 0;
    for (auto it = tentacles.begin(); it != tentacles.end(); it++) {
        for (auto at = it->coordinates.begin(); at != it->coordinates.end(); at++) {
            //for(Point checkpoint : at->checkpoints) {
                Point point(at->x_coord,at->y_coord);
                if(isCollisionPoint(obstacles,point)) {
                    if(it - tentacles.begin() <= tentacles.size()/2) {
                        leftUnsafeTentacles++;
                    } else {
                        rightUnsafeTentacles++;
                    }
                    it->isSafePath = false;
                    break;
                }
            //}
        }
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
        if(!ten.isSafePath) {
            tentacleColor = red;
        }
        for (auto it = ten.coordinates.begin(); it != ten.coordinates.end(); it++ ) {
            auto itnext = it++;
            if(itnext != ten.coordinates.end()) {

                Point p1(it->x_coord,it->y_coord);
                Point p2(itnext->x_coord, itnext->y_coord);
                line(outputImg, p1, p2, tentacleColor);
            }
        }
    }
    return outputImg;
}

bool Tentacles::isCollisionPoint(Mat obstacles, Point target) {
    //If input image at Point coordinate is white, then collision.
    //Also check all checkpoints around the tentacle, because of vehicle thickness.
    if(obstacles.size().width < target.x || obstacles.size().height < target.y) {
        return false;
    }
    return obstacles.at<uchar>(target) == 255;

}

bool Tentacles::findNewSteeringAngle(double &steeringAngle, std::vector<S_Tentacle> tentacles) {

    //TODO: maybe better when i just check if angle 0.0 is safe at the moment, and then just use 0.0 as angle

    S_Tentacle middleTentacle = tentacles.at(tentacles.size()/2);

    if(middleTentacle.isSafePath) {
        steeringAngle = middleTentacle.steeringAngle;
        return true;
    }

    if(leftUnsafeTentacles > rightUnsafeTentacles) {
        //Choose right side first

        for(int i = (int)tentacles.size()-1; i >= 0; i--) {
            S_Tentacle tentacle = tentacles.at(i);
            if(tentacle.isSafePath) {
                steeringAngle = tentacle.steeringAngle;
                return true;
            }
        }
    } else {
        //Chose left side first
        for(int i = 0; i < tentacles.size(); i++) {
            S_Tentacle tentacle = tentacles.at(i);
            if(tentacle.isSafePath) {
                steeringAngle = tentacle.steeringAngle;
                return true;
            }
        }
    }

    /*for(auto it = tentacles.begin(); it != tentacles.end(); it++) {
        if(it->steeringAngle == steeringAngle) {
            if(it->steeringAngle == 0.0 && it->isSafePath) {
                return true;
            }

            if(leftUnsafeTentacles > rightUnsafeTentacles) {
                //Choose right side first

                for(int i = (int)tentacles.size()-1; i >= 0; i--) {
                    S_Tentacle tentacle = tentacles.at(i);
                    if(tentacle.isSafePath) {
                        steeringAngle = tentacle.steeringAngle;
                        return true;
                    }
                }
            } else {
                //Chose left side first
                for(int i = 0; i < tentacles.size(); i++) {
                    S_Tentacle tentacle = tentacles.at(i);
                    if(tentacle.isSafePath) {
                        steeringAngle = tentacle.steeringAngle;
                        return true;
                    }
                }
            }
        }
    }*/

    cout << "No safe path found!" << endl;
    return false;
}