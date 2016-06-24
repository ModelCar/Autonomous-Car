
#include <opencv2/opencv.hpp>

struct S_checkpoint{
    std::vector<cv::Point> checkpoints;
    int x_coord;
    int y_coord;
};;

struct S_Tentacle{
    std::vector<S_checkpoint> coordinates;
    int crashsegment;
    int crashdistance;
    bool isSafePath;
    double steeringAngle;
};

static const int tentacleResolution = 200;
static const int checkpointsPerTentacleStep = 9;

class Tentacles {

public:

    std::vector<S_Tentacle> generateTentacles(int image_width, int image_height, double speed, double currentSteering);
    void checkTentacles(cv::Mat obstacles, std::vector<S_Tentacle> &tentacles);
    cv::Mat renderTentacles(cv::Mat inputImg, std::vector<S_Tentacle> tentacles);
    bool findNewSteeringAngle(double &steeringAngle, std::vector<S_Tentacle> tentacles);

private:

    bool isCollisionPoint(cv::Mat obstacles, cv::Point target);

    int tentaclesPerSide = 20;
    double vehicleWidth = 10.0;
    double maxSteeringAngle = 14.0;
    double maxSteeringChange = 30.0;
    double steeringAmplifier = 15.0;

    int leftUnsafeTentacles;
    int rightUnsafeTentacles;
};

