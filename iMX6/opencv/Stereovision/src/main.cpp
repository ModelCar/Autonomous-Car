#include "../inc/StereoVision.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

    bool showImages = true;
	StereoVision* stereoVision;

	if  (argc == 3) {
		stereoVision = new StereoVision(showImages, atoi(argv[1]), argv[2]);
	} else if (argc == 2) {
		stereoVision = new StereoVision(showImages, atoi(argv[1]));
	} else {
		stereoVision = new StereoVision(showImages);
	}

	string speedStr;
	double speed;
	cout << "Type in desired start speed: ";
	getline (cin, speedStr);
	speed = stod(speedStr);
	stereoVision->setCurrentSpeed(speed);

	//TODO: Maybe read current steering from car
	cout << "Assuming initial steering angle: 0.0" << endl;
	stereoVision->setCurrentSteering(0.0);

    stereoVision->run();

	return 0;
}

