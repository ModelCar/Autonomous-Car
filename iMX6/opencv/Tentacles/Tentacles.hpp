
#include <opencv2/opencv.hpp>

struct S_checkpoint;

struct S_Tentacle;

static const int tentacleResolution = 200;
static const int checkpointsPerTentacleStep = 9;

class Tentacles {

public:
    std::vector<S_Tentacle> generateTentacles(double speed);
    bool checkTentacles(cv::Mat obstacles, cv::Point target);

private:

    int tentaclesPerSide = 20;

    double vehicleWidth = 10.0;
    double maxSteeringAngle = 17.0;
    double maxSteeringChange = 30.0;
    double steeringAmplifier = 15.0;
    int inputImageWidth = 320;
    int inputImageHeight = 240;
};

//#define Tentacle_Resolution 200
//#define Tentacles_Per_Side 20
//#define Vehicle_Width 10.0
//#define Checkpoints_Per_Tentacle_Step 9 // has to be odd
#define Checkpoints_For_Crash 2
//#define Steering_Max_Angle 17.0

// only for current inaccurate tentacle generation code
//#define Max_Steering_Change 30.0// limits maximum change of steering angle per time step
//#define Steering_Amplifier 15.0 //#Steering_Amplifier*#Steering_Max_Angle
// <<<<

//#define Tentacle_Input_Image_Width 320.0
//#define Tentacle_Input_Image_Height 240.0

