#include "TrafficLights.cpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat image;

	String imgFolder = "../Images/out_19_9_54_distance_measured/";
	//String imgFolder = "../Images/TrafficLights/lights";
	String imagePath;
	
	int i = 1;					// i determines which image in the folder we are looking for
								// available images include lights1.jpb .... lights 10.jpg
								// and those will be read by this experimental client

	
	CalibrateDistance();		// to be used once upon startup to calculate a linear function
								// that provides the distance in cm given the radius of the traffic light detected
								// and 2 known pairs of traffic lights - radiuses

	CreateControllers();		// to be used if controllers for color filtering and image processing are to be used
	
	
	while (true)
	{
		imagePath.clear();
		char filename[4];
		sprintf (filename, "%04d.jpg", i);
		//sprintf (filename, "%d.jpg", i);

		imagePath = imgFolder + filename;

		//cout << imagePath <<endl;

		//read the image data in the file "lights#.jpg"
		image = imread(imagePath, CV_LOAD_IMAGE_UNCHANGED); 
		
		if (image.empty()) //check whether the image is loaded or not
		{
			cout << "Error : Image cannot be loaded..!!" << endl;
		}

		/**********************************************************************/
		// main code -> detect traffic lights in image and return them in a vector of objects
		vector<TrafficLight> results = DetectLights(image);

		if (results.size() == 0)
		{
			cout << "No traffic lights found" << endl;
		}
		else
		{
			cout << "Traffic Lights detected: " << results.size() << endl;
			for (int i=0; i < results.size(); i++)
			{
				TrafficLight tr = results[i];
				cout << tr.color << " " << (float)tr.distance/100 << "m away at ("<< tr.xPos << ", " << 
					tr.yPos << ") with radius: " << tr.radius << endl;
			}	
		}
		/**************************************************************************/

		//how long to wait until image is reprocessed (in milliseconds)
		int key = waitKey(0);

		// key pressed -> proceed to next image for processing
		if (key >= 0) {
			i++;
			if (i >= 100) {
				i = 1;
			}
		}
	}
	return 0;
}
