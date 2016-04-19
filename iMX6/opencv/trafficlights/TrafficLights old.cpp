#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class TrafficLight;

Mat filterRed(Mat);
Mat filterGreen(Mat);
Mat filterYellow(Mat);
void CreateControllers();
vector<TrafficLight> DetectLights(Mat imgOriginal);
void CalibrateDistance();

bool showImages = true;
bool printInfo = false;

//calibrate distance estimator
int calRadius1 = 16;	//pixels of 1st circle
int calDist1 = 120;		//distance in centimeters of circle in real image
int calRadius2 = 8;		//pixels of 2nd circle
int calDist2 = 300;		//distance in centimeters of circle in real image
float slope;
float offset;

//Global variables for controllers
int GaussiaKernelSize = 4;
int Hough_dp = 1;
int Hough_minDist = 25;
int Hough_param1 = 2;
int Hough_param2 = 10;
int Hough_minRadius = 1;
int Hough_maxRadius = 100;
int decBrightness = 0;
int changeContrast = 10;
int erodeDilate = 2;
int blurSize = 2;
int yLimit = 10;
int colorPercentage = 5;

int iLowH_Red1 = 0;
int iHighH_Red1 = 10;
int iLowS_Red1 = 85;
int iHighS_Red1 = 255;
int iLowV_Red1 = 100;
int iHighV_Red1 = 255;
int iLowH_Red2 = 160;
int iHighH_Red2 = 179;
int iLowS_Red2 = 85;
int iHighS_Red2 = 255;
int iLowV_Red2 = 80;
int iHighV_Red2 = 255;

int iLowH_Yellow = 22;
int iHighH_Yellow = 38;
int iLowS_Yellow = 120;
int iHighS_Yellow = 255;
int iLowV_Yellow = 80;
int iHighV_Yellow = 255;

int iLowH_Green = 38;
int iHighH_Green = 85;
int iLowS_Green = 120;
int iHighS_Green = 255;
int iLowV_Green = 80;
int iHighV_Green = 255;


class TrafficLight
{
	public:
		int xPos;   		// x position of center of traffic light
		int yPos;			// y position of center of traffic light
		int radius;			// radius of circle detected
		string color;		// "R", "Y", "G"
		int distance;		// Estimation how far away the traffic lights is
		float colPerc;

		/* getDistance uses radius as parameter (measures in pixels) and return distance of traffic light
		measured in centimeters
		*/
		void calcDistance()
		{
			this->distance = slope * radius + offset;
		}
};

vector<TrafficLight> DetectLights(Mat imgOriginal)
{
	Mat imgMask;								//image in b/w after color filter has been applied
	Mat imgCircles = imgOriginal.clone();		//image with the circles detected drawn on it
	Mat imgProcessed = imgOriginal.clone();		//image after processing

	vector<TrafficLight> results;				// vector containing the results of the CV algorithm

	const char *list[] = {"Red", "Yellow", "Green"};


	//IMAGE PROCESSING
	//brightness and contrast adjustment
    imgProcessed.convertTo(imgProcessed, -1, (float)changeContrast/10, -decBrightness);

    if (erodeDilate > 0)
    {
	    // Morphological opening (removes small objects from the foreground)
		erode(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilate, erodeDilate)));
		dilate(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilate, erodeDilate)));

		// Morphological closing (removes small holes from the foreground)
		dilate(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilate, erodeDilate)));
		erode(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilate, erodeDilate)));
	}

	// Blur the image
	// check if blurSize is odd
	//if (blurSize % 2 == 0) blurSize++;
	//GaussianBlur(imgProcessed, imgProcessed, Size(blurSize, blurSize), 2, 2);


	// Repeat for every color (red, yellow, green)
	for (int i = 0; i < 3 ; i++){
		if (list[i] == "Red"){
			imgMask = filterRed(imgProcessed);
		}
		else if (list[i] == "Yellow"){
			imgMask = filterYellow(imgProcessed);
		}
		else if (list[i] == "Green"){
			imgMask = filterGreen(imgProcessed);
		}


		// Use the Hough transform to detect circles in the mask image
		vector<Vec3f> circles;
		HoughCircles(imgMask, circles, CV_HOUGH_GRADIENT, 	//image, circles, method
				Hough_dp, Hough_minDist,					//dp, minDist
				Hough_param1, Hough_param2,					//param1, param2
				Hough_minRadius, Hough_maxRadius);			//min radius, max radius

	    // Parameters used in python dp=1, minDist=50, param1=2, param2=15, minRadius=10, maxRadius=50)

      	// Loop over all detected circles and outline them on the original image
		if (circles.size() > 0) 
		{
			for (int current_circle = 0; current_circle < circles.size(); ++current_circle) {

				TrafficLight tr_light;
				
				tr_light.xPos = round(circles[current_circle][0]);
		        tr_light.yPos = round(circles[current_circle][1]);
		        tr_light.radius = round(circles[current_circle][2]);
		        tr_light.calcDistance();

				//check how thick the circle is
				int x_radius_min = tr_light.xPos - tr_light.radius;
		        int x_radius_max = tr_light.xPos + tr_light.radius;
		        int y_radius_min = tr_light.yPos - tr_light.radius;
		        int y_radius_max = tr_light.yPos + tr_light.radius;

		        int x_white_counter = 0;
		        int x_pixel_counter = 0;
		        int y_white_counter = 0;
		        int y_pixel_counter = 0;

		        Scalar colour;

		        //x axis
				for (int j = x_radius_min; j < x_radius_max; j++)
				{
					colour = imgMask.at<uchar>(Point(j, tr_light.yPos));
					
					//cout << "Pixel( " << j << ", " << tr_light.yPos << ") has color: " << colour.val[0] << endl; 
					if(colour.val[0]>=220)
						x_white_counter++;
				}

				// y axis
				for (int j = y_radius_min; j < y_radius_max; j++)
				{
					colour = imgMask.at<uchar>(Point(tr_light.xPos, j));
					
					//cout << "Pixel( " << j << ", " << tr_light.yPos << ") has color: " << colour.val[0] << endl; 
					if(colour.val[0]>=220)
						y_white_counter++;
				}

				x_pixel_counter = x_radius_max - x_radius_min;
				//cout << "Percentage: " <<  << endl;
				y_pixel_counter = y_radius_max - y_radius_min;
				//cout << "Percentage: " <<  << endl;

				tr_light.colPerc = ((float)x_white_counter/x_pixel_counter + (float)y_white_counter/y_pixel_counter) / 2;
				
				if (tr_light.colPerc < (float)colorPercentage/10)
				{
					circles.erase(circles.begin()+current_circle);
					//cout << "Circle erased" << endl;
					continue;
				}


				// check if center of circle is in the uppper yLimit part of the image
				// if not so continue with next circle	
				if (tr_light.yPos > imgProcessed.size().height * (float)yLimit/10) continue;

				//draw circles on image
				circle(imgCircles, Point(tr_light.xPos, tr_light.yPos), tr_light.radius, Scalar(0, 255, 0), 2);
				if (tr_light.colPerc < (float)colorPercentage/10)
				{
					circle( imgCircles, Point(tr_light.xPos, tr_light.yPos), 5, Scalar(255, 255, 255), -1, 8, 0 );
					tr_light.color = 'F';
				}
				else
				{
					if (list[i] == "Red"){
						circle( imgCircles, Point(tr_light.xPos, tr_light.yPos), 5, Scalar(153, 153, 255), -1, 8, 0 );
						tr_light.color = 'R';
					}
					else if (list[i] == "Yellow"){
						circle( imgCircles, Point(tr_light.xPos, tr_light.yPos), 5, Scalar(153, 255, 255), -1, 8, 0 );
						tr_light.color = 'Y';
					}
					else if (list[i] == "Green"){
						circle( imgCircles, Point(tr_light.xPos, tr_light.yPos), 5, Scalar(153, 255, 153), -1, 8, 0 );
						tr_light.color = 'G';
					}
				}

				results.push_back(tr_light);
			}
		}
		if (showImages) imshow(string("Filter ") + list[i], imgMask);	//show the thresholded image of each color
	}

	/*if (showImages)
	{
		Size sz1 = imgCircles.size();
	    Size sz2 = imgProcessed.size();
	    Mat im3(sz1.height, sz1.width+sz2.width, CV_8UC3);
	    Mat left(im3, Rect(0, 0, sz1.width, sz1.height));
	    imgCircles.copyTo(left);
	    Mat right(im3, Rect(sz1.width, 0, sz2.width, sz2.height));
	    imgProcessed.copyTo(right);
	    imshow("Original and Processed", im3);
	}    */
	
	if (showImages) imshow("Image", imgCircles);	//show the thresholded image of each color

	return results;
}

Mat filterRed(Mat img){
	Mat imgHSV;
	Mat imgThresholded;

	cvtColor(img, imgHSV, COLOR_BGR2HSV);		//Convert the captured frame from BGR to HSV

	Mat lowerRedRange;
	Mat upperRedRange;

	inRange(imgHSV, Scalar(iLowH_Red1, iLowS_Red1, iLowV_Red1), Scalar(iHighH_Red1, iHighS_Red1, iHighS_Red1), lowerRedRange);
	inRange(imgHSV, Scalar(iLowH_Red2, iLowS_Red2, iLowV_Red2), Scalar(iHighH_Red2, iHighS_Red2, iHighS_Red2), upperRedRange);

	//Combine lower and upper range of red filter
	addWeighted(lowerRedRange, 1.0, upperRedRange, 1.0, 0.0, imgThresholded);

	//blur image for better results
	//if (blurSize % 2 == 0) blurSize++;
	//GaussianBlur(imgThresholded, imgThresholded, Size(blurSize, blurSize), 2, 2);

	return imgThresholded;
}

Mat filterYellow(Mat img) {

	Mat imgHSV;
	Mat imgThresholded;

	//Convert the captured frame from BGR to HSV
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	inRange(imgHSV, Scalar(iLowH_Yellow, iLowS_Yellow, iLowV_Yellow), Scalar(iHighH_Yellow, iHighS_Yellow, iHighS_Yellow), imgThresholded);

	//blur image for better results
	//if (blurSize % 2 == 0) blurSize++;
	//GaussianBlur(imgThresholded, imgThresholded, Size(blurSize, blurSize), 2, 2);

	return imgThresholded;
}

Mat filterGreen(Mat img) {

	Mat imgHSV;
	Mat imgThresholded;

	//Convert the captured frame from BGR to HSV
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	inRange(imgHSV, Scalar(iLowH_Green, iLowS_Green, iLowV_Green), Scalar(iHighH_Green, iHighS_Green, iHighS_Green), imgThresholded);

	//blur image for better results
	//if (blurSize % 2 == 0) blurSize++;
	//GaussianBlur(imgThresholded, imgThresholded, Size(blurSize, blurSize), 2, 2);

	return imgThresholded;
}

void CreateControllers()
{
	//create a window called "Control" to adjust Hue, Saturation and Value (for calibration purposes only)
	// namedWindow("Control_Red", CV_WINDOW_NORMAL);
	// namedWindow("Control_Yellow", CV_WINDOW_NORMAL);
	// namedWindow("Control_Green", CV_WINDOW_NORMAL);
	namedWindow("Effects", CV_WINDOW_NORMAL);

	// //Create trackbars in "Control" window
	// //Red Controls
	// //Hue (0 - 179)
	// createTrackbar("Red1_LowH", "Control_Red", &iLowH_Red1, 179);
	// createTrackbar("Red1_HighH", "Control_Red", &iHighH_Red1, 179);
	// //Saturation (0 - 255)
	// createTrackbar("Red1_LowS", "Control_Red", &iLowS_Red1, 255);
	// createTrackbar("Red1_HighS", "Control_Red", &iHighS_Red1, 255);
	// //Value (0 - 255)
	// createTrackbar("Red1_LowV", "Control_Red", &iLowV_Red1, 255);
	// createTrackbar("Red1_HighV", "Control_Red", &iHighV_Red1, 255);
	// //Hue (0 - 179)
	// createTrackbar("Red2_LowH", "Control_Red", &iLowH_Red2, 179);
	// createTrackbar("Red2_HighH", "Control_Red", &iHighH_Red2, 179);
	// //Saturation (0 - 255)
	// createTrackbar("Red2_LowS", "Control_Red", &iLowS_Red2, 255);
	// createTrackbar("Red2_HighS", "Control_Red", &iHighS_Red2, 255);
	// //Value (0 - 255)
	// createTrackbar("Red2_LowV", "Control_Red", &iLowV_Red2, 255);
	// createTrackbar("Red2_HighV", "Control_Red", &iHighV_Red2, 255);

	// //Yellow Controls
	// //Hue (0 - 179)
	// createTrackbar("Green_LowH", "Control_Yellow", &iLowH_Yellow, 179);
	// createTrackbar("Green_HighH", "Control_Yellow", &iHighH_Yellow, 179);
	// //Saturation (0 - 255)
	// createTrackbar("Green_LowS", "Control_Yellow", &iLowS_Yellow, 255);
	// createTrackbar("Green_HighS", "Control_Yellow", &iHighS_Yellow, 255);
	// //Value (0 - 255)
	// createTrackbar("Green_LowV", "Control_Yellow", &iLowV_Yellow, 255);
	// createTrackbar("Green_HighV", "Control_Yellow", &iHighV_Yellow, 255);

	// //Green Controls
	// //Hue (0 - 179)
	// createTrackbar("Green_LowH", "Control_Green", &iLowH_Green, 179);
	// createTrackbar("Green_HighH", "Control_Green", &iHighH_Green, 179);
	// //Saturation (0 - 255)
	// createTrackbar("Green_LowS", "Control_Green", &iLowS_Green, 255);
	// createTrackbar("Green_HighS", "Control_Green", &iHighS_Green, 255);
	// //Value (0 - 255)
	// createTrackbar("Green_LowV", "Control_Green", &iLowV_Green, 255);
	// createTrackbar("Green_HighV", "Control_Green", &iHighV_Green, 255);

	createTrackbar("Gaussian Kernel Size", "Effects", &GaussiaKernelSize, 20);
	createTrackbar("Hough dp", "Effects", &Hough_dp, 20);
	createTrackbar("Hough minDist", "Effects", &Hough_minDist, 100);
	createTrackbar("Hough param1", "Effects", &Hough_param1, 200);
	createTrackbar("Hough param2", "Effects", &Hough_param2, 200);
	createTrackbar("Hough minRadius", "Effects", &Hough_minRadius, 20);
	createTrackbar("Hough maxRadius", "Effects", &Hough_maxRadius, 200);
	createTrackbar("Decrease Brightness", "Effects", &decBrightness, 100);
	createTrackbar("Change Contrast", "Effects", &changeContrast, 20);
	createTrackbar("Erode/Dialate", "Effects", &erodeDilate, 10);
	createTrackbar("Blur Kernel Size", "Effects", &blurSize, 10);
	createTrackbar("Upper part of image", "Effects", &yLimit, 10);
	createTrackbar("Color Percentage", "Effects", &colorPercentage, 10);
}


/* CalibrateDistance uses 2 calibration points declared by 
	(calRadius1, calDist1) and (calRadius2, calDist2)
	and estimates slope and offset of linear function

	It does not return any results as the slope and offset variables are global and calculated once
*/
void CalibrateDistance()
{
	if (printInfo) cout << "Point 1: (" << calRadius1 << ", " << calDist1 << ")" << endl;
	if (printInfo) cout << "Point 2: (" << calRadius2 << ", " << calDist2 << ")" << endl;

	slope = (float)(calDist2 - calDist1) / (float)(calRadius2 - calRadius1);

	offset = calDist1 - slope*calRadius1;

	if (printInfo) cout << "Slope: " << slope << ", Offset: " << offset << endl;
}