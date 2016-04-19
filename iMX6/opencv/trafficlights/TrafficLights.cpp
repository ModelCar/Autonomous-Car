//#define DEBUG
//#define SHOWIMAGES
//#define PRINTMESSAGES

#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cmath>

using namespace cv;
using namespace std;

class TrafficLight;

Mat filterRed(Mat);
Mat filterGreen(Mat);
Mat filterYellow(Mat);
void CreateControllers();
void CalibrateDistance();
vector<TrafficLight> DetectLights(Mat imgOriginal);


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
int erodeDilate = 3;
int blurSize = 2;
int yLimit = 100;
int colorPercentage = 80;
int momentArea = 25;
int areaPercentage = 25;

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
		char color;		// "R", "Y", "G"
		int distance;		// Estimation how far away the traffic lights is
		double contorArea;
		double calcArea;

		/* getDistance uses radius as parameter (measures in pixels) and return distance of traffic light
		measured in centimeters
		*/
		void calcDistance()
		{
			this->distance = slope * radius + offset;
		}
};

Mat processFilterImage(Mat inputImage, char color)
{
	Mat imgMask;

	if (color == 'R')
	{
		imgMask = filterRed(inputImage);
	}
	else if (color == 'Y'){
		imgMask = filterYellow(inputImage);
	}
	else if (color == 'G'){
		imgMask = filterGreen(inputImage);
	}	

	return imgMask;
}

vector<TrafficLight> DetectLights(Mat imgOriginal)
{
	Mat imgMask;								//image in b/w after color filter has been applied
	Mat imgCircles = imgOriginal.clone();		//image with the circles detected drawn on it
	Mat imgProcessed = imgOriginal.clone();		//image after processing
	Mat imgCont;								//image for contours

	vector<TrafficLight> results;				// vector containing the results of the CV algorithm

	char list[] = {'R' , 'Y', 'G'};

	//Image Processing

	//brightness and contrast adjustment
    //imgProcessed.convertTo(imgProcessed, -1, (float)changeContrast/10, -decBrightness);

	// Blur the image
	// check if blurSize is odd
	//if (blurSize % 2 == 0) blurSize++;
	//GaussianBlur(imgProcessed, imgProcessed, Size(blurSize, blurSize), 2, 2);

	if (erodeDilate > 0)
    {
	    // Morphological opening (removes small objects from the foreground)
		erode(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilate, erodeDilate)));
		dilate(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilate, erodeDilate)));

		// Morphological closing (removes small holes from the foreground)
		dilate(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilate, erodeDilate)));
		erode(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilate, erodeDilate)));
	}

	#ifdef SHOWIMAGES
		imshow("Preprocess", imgProcessed);	//show the thresholded image of each color
	#endif

	// Repeat for every color (red, yellow, green)
	for (int z = 0; z < 3 ; z++)
	{
		//color filtering
		imgMask = processFilterImage(imgProcessed, list[z]);		

		imgCont = imgMask.clone();		//image with the circles detected drawn on it

		// find contours
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours( imgCont, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		// Get the moments
	  	vector<Moments> mu(contours.size() );
  		vector<float> radius( contours.size() );
  		vector<Point2f> center( contours.size() );

	  	for( int i = 0; i < contours.size(); i++ )
	    {

	    	mu[i] = moments( contours[i], false );

			//filter Moments according to Area (select only large Moments)
	       	if (mu[i].m00 > momentArea)
	    	{
				#ifdef DEBUG
					drawContours( imgCircles, contours, i, Scalar(153, 255, 255), 1, 1, hierarchy, 0, Point() );
				#endif

				minEnclosingCircle( (Mat)contours[i], center[i], radius[i] );


				TrafficLight tr_light;
				//calculate the position of the ball				
				tr_light.xPos = mu[i].m10 / mu[i].m00;
		        tr_light.yPos = mu[i].m01 / mu[i].m00;
		        tr_light.radius = (int)radius[i];
		        tr_light.calcDistance();
		        tr_light.contorArea = mu[i].m00;
		        tr_light.calcArea = 3.14 * tr_light.radius * tr_light.radius;

				// check if moment area is almost the same with circle area
				if (abs(tr_light.contorArea - tr_light.calcArea) > tr_light.calcArea * (float)areaPercentage/100) continue;

		        // check if center of circle is in the uppper yLimit part of the image
				// if not so continue with next circle	
				if (tr_light.yPos > imgCont.size().height * (float)yLimit/100) continue;

				//check how thick the circle is
				int x_radius_min = tr_light.xPos - tr_light.radius;
		        int x_radius_max = tr_light.xPos + tr_light.radius;
		        int y_radius_min = tr_light.yPos - tr_light.radius;
		        int y_radius_max = tr_light.yPos + tr_light.radius;

		        int x_counter = 0;
		        int y_counter = 0;
		        int d1_counter = 0;
		        int d2_counter = 0;
		        int pixel_counter = 2*tr_light.radius;

		       	Scalar colour;
		       	const float sin45 = 0.707;
		       	const int rad45 = (int)(sin45 * tr_light.radius);

				for (int j = 0; j < pixel_counter; j++)
				{
					if ( (x_radius_min+j) <= imgMask.size().width ){
						colour = imgMask.at<uchar>(Point(x_radius_min+j, tr_light.yPos));
						rectangle(imgCircles, Point(x_radius_min+j, tr_light.yPos), Point(x_radius_min+j, tr_light.yPos), Scalar(0,255,0), 1, 8);

						if (colour.val[0]>=220)
						{
							x_counter++;
						}
					}

					if ( (y_radius_min+j) <= imgMask.size().height ){

						colour = imgMask.at<uchar>(Point(tr_light.xPos, y_radius_min+j));
						rectangle(imgCircles, Point(tr_light.xPos, y_radius_min+j), Point(tr_light.xPos, y_radius_min+j), Scalar(0,255,0), 1, 8);

						if (colour.val[0]>=220)
						{
							y_counter++;
						}
					}
				}

				int xStart = tr_light.xPos - rad45;
				int yStart = tr_light.yPos - rad45;
				int pixel_counter45 = (float)pixel_counter * sin45;
				for (int j = 0; j < pixel_counter45; j++)
				{
					if ( (xStart <= imgMask.size().width) && ( yStart <= imgMask.size().height) ){
						colour = imgMask.at<uchar>(Point(xStart, yStart));
						if (colour.val[0]>=220) d1_counter++;
						xStart++;
						yStart++;
						rectangle(imgCircles, Point(xStart, yStart), Point(xStart, yStart), Scalar(0,255,0), 1, 8);
					}
				}

				xStart = tr_light.xPos - rad45;
				yStart = tr_light.yPos + rad45;
				for (int j = 0; j < pixel_counter45; j++)
				{
					if ( (xStart <= imgMask.size().width) && ( yStart <= imgMask.size().height) ){
						colour = imgMask.at<uchar>(Point(xStart, yStart));
						if (colour.val[0]>=220) d2_counter++;
						xStart++;
						yStart--;
						rectangle(imgCircles, Point(xStart, yStart), Point(xStart, yStart), Scalar(0,255,0), 1, 8);
					}
				}
							

				#ifdef PRINTMESSAGES
					printf ("x percentage: %.2f, y percentage: %.2f\n", (float)x_counter/pixel_counter, (float)y_counter/pixel_counter);
					printf ("d1 percentage: %.2f, d2 percentage: %.2f\n", (float)d1_counter/pixel_counter45, (float)d2_counter/pixel_counter45);
				#endif
				
				if ( ((float)x_counter/pixel_counter > (float)colorPercentage/100) &&
						((float)y_counter/pixel_counter > (float)colorPercentage/100) )
				{					
					if (list[z] == 'R'){
						#ifdef SHOWIMAGES
							circle( imgCircles, Point(tr_light.xPos, tr_light.yPos), 5, Scalar(153, 153, 255), -1, 4, 0 );
						#endif
						tr_light.color = 'R';
					}
					else if (list[z] == 'Y'){
						#ifdef SHOWIMAGES
							circle( imgCircles, Point(tr_light.xPos, tr_light.yPos), 5, Scalar(153, 255, 255), -1, 4, 0 );
						#endif
						tr_light.color = 'Y';
					}
					else if (list[z] == 'G'){
						#ifdef SHOWIMAGES
							circle( imgCircles, Point(tr_light.xPos, tr_light.yPos), 5, Scalar(153, 255, 153), -1, 4, 0 );
						#endif
						tr_light.color = 'G';
					}

					results.push_back(tr_light);
					#ifdef SHOWIMAGES
						circle( imgCircles, center[i], (int)radius[i], Scalar(255, 153, 255), 2, 2, 0 );
					#endif
				}
	    	} 
	    }
		#ifdef SHOWIMAGES
			imshow(string("Filter ") + list[z], imgMask);	//show the thresholded image of each color
		#endif
	}
	#ifdef SHOWIMAGES	
		imshow("Image", imgCircles);	//show the thresholded image of each color
	#endif

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

	return imgThresholded;
}

Mat filterYellow(Mat img) {

	Mat imgHSV;
	Mat imgThresholded;

	//Convert the captured frame from BGR to HSV
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	inRange(imgHSV, Scalar(iLowH_Yellow, iLowS_Yellow, iLowV_Yellow), Scalar(iHighH_Yellow, iHighS_Yellow, iHighS_Yellow), imgThresholded);

	return imgThresholded;
}

Mat filterGreen(Mat img) {

	Mat imgHSV;
	Mat imgThresholded;

	//Convert the captured frame from BGR to HSV
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	inRange(imgHSV, Scalar(iLowH_Green, iLowS_Green, iLowV_Green), Scalar(iHighH_Green, iHighS_Green, iHighS_Green), imgThresholded);

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
	createTrackbar("Upper part of image", "Effects", &yLimit, 100);
	createTrackbar("Color Percentage", "Effects", &colorPercentage, 100);
	createTrackbar("Moment Area", "Effects", &momentArea, 250);
	createTrackbar("Area Percentage", "Effects", &areaPercentage, 100);
}


/* CalibrateDistance uses 2 calibration points declared by 
	(calRadius1, calDist1) and (calRadius2, calDist2)
	and estimates slope and offset of linear function

	It does not return any results as the slope and offset variables are global and calculated once
*/
void CalibrateDistance()
{
	slope = (float)(calDist2 - calDist1) / (float)(calRadius2 - calRadius1);
	offset = calDist1 - slope*calRadius1;


	#ifdef PRINTMESSAGES
		cout << "Point 1: (" << calRadius1 << ", " << calDist1 << ")" << endl;
		cout << "Point 2: (" << calRadius2 << ", " << calDist2 << ")" << endl;
		cout << "Slope: " << slope << ", Offset: " << offset << endl;
	#endif
}