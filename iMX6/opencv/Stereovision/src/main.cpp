#include "../inc/StereoVision.hpp"
#include "../inc/DriveLogicMock.hpp"
#include "pthread.h"
#include "thread"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

	DriveLogicMock * driveLogicMock = new DriveLogicMock();

	StereoVision* stereoVision;

	if  (argc == 3) {
		stereoVision = new StereoVision(atoi(argv[1]), argv[2]);
	} else if (argc == 2) {
		stereoVision = new StereoVision(atoi(argv[1]));
	} else {
		stereoVision = new StereoVision();
	}

    stereoVision->setCollisionAvoidanceDelegate(driveLogicMock);
    stereoVision->setStereovisionDelegate(driveLogicMock);

    stereoVision->run();

	return 0;
}

