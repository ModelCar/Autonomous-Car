#ifndef __PARSE_H_INCLUDED__
#define __PARSE_H_INCLUDED__

#include <opencv2/opencv.hpp>

#define Tentacle_Resolution 200
#define Tentacles_Per_Side 20
#define Vehicle_Width 10
#define Checkpoints_Per_Tentacle_Step 9 // has to be odd
#define Checkpoints_For_Crash 2
#define Steering_Max_Angle 17

// only for current inaccurate tentacle generation code
#define Max_Steering_Change 30// limits maximum change of steering angle per time step
#define Steering_Amplifier 15 //#Steering_Amplifier*#Steering_Max_Angle
// <<<<

#define Tentacle_Input_Image_Width 320
#define Tentacle_Input_Image_Height 240

void generateTentacles();

void checkTentacles(cv::Mat obstacles, cv::Point target);

#endif
