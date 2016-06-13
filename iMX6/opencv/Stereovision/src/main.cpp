#include "../inc/StereoVision.hpp"

using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {

	StereoVision* stereoVision;

	if  (argc == 3) {
		stereoVision = new StereoVision(atoi(argv[1]), argv[2]);
	} else if (argc == 2) {
		stereoVision = new StereoVision(atoi(argv[1]));
	} else {
		stereoVision = new StereoVision();
	}
	stereoVision->run();

	return 0;
}

