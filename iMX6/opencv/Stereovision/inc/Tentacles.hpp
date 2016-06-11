
#include <opencv2/opencv.hpp>

struct S_checkpoint{
    std::vector<cv::Point> checkpoints;
    int x_coord;
    int y_coord;
};;

struct S_Tentacle{
    //TODO: add steering angle for tentacle
    std::vector<S_checkpoint> coordinates;
    int crashsegment;
    int crashdistance;
};

static const int tentacleResolution = 200;
static const int checkpointsPerTentacleStep = 9;

class Tentacles {

public:
    std::vector<S_Tentacle> generateTentacles(double speed);
    bool checkTentacles(cv::Mat obstacles, cv::Point target);
    cv::Mat renderTentacles(cv::Mat inputImg, std::vector<S_Tentacle> tentacles);

private:

    int tentaclesPerSide = 20;

    double vehicleWidth = 10.0;
    double maxSteeringAngle = 17.0;
    double maxSteeringChange = 30.0;
    double steeringAmplifier = 15.0;
    int inputImageWidth = 320;
    int inputImageHeight = 240;
};

