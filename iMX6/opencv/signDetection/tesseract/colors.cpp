/*
signtypes:  1   Stop
            2   yield
            3   attention
            4   right of way street
            5   10km/h
            6   20km/h
            7   30km/h
            8   40km/h
            9   50km/h
            10  60km/h
            11  70km/h
            12  80km/h
            13  right of way
            14  circle
*/

/**
* requires tesseract-ocr
*/

#include "colors.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>

using namespace cv;
using namespace std;

vector<vector<Point> > references;
tesseract::TessBaseAPI tess;

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    cout << "Usage: main <image-file-name>" << endl;
    exit(0);
  }

  // load the image
  Mat img = imread(argv[1]);
  //references = getReferences();//load the reference images
  tess.Init(NULL, NULL, tesseract::OEM_DEFAULT);
  tess.SetVariable("tessedit_char_whitelist", "0123456789");
  //for speed testing
  /*double t0 = (double) getTickCount();/
  for(int i = 0; i < 100; i++)
  {
    signDetection(img);
  }
  double t1 = (double) getTickCount();
  cout << (t1-t0) / getTickFrequency() << endl;*/

  signDetection(img);

  waitKey(0);

  return 0;
}

/**
 * Detects all signs in the image
 * @param img   the input image
 * @return      all detected signs
 *              each sign consists of its center point and its type
 */
vector<pair<Point,uint> > signDetection(Mat img)
{
  Mat work;
  vector<pair<Point,uint> > result, tmp;
  imshow("original",img);

  cvtColor(img, work, COLOR_BGR2HSV);//change to HSV and create working copy

  result = red(work);//all red signs
  tmp = yellow(work);//all yellow signs
  result.insert(result.end(), tmp.begin(), tmp.end());
  tmp = blue(work);//all blue signs
  result.insert(result.end(), tmp.begin(), tmp.end());
  sort(result.begin(), result.end(), [](pair<Point,uint> a, pair<Point,uint> b){return a.first.x<b.first.x;});//needed for erase
  result.erase(unique(result.begin(), result.end(), equalSign), result.end());//erase duplicates

  for(uint i = 0; i < result.size(); i++)//print out all signs
  {
    cout << result[i].first<< ", " << result[i].second << endl;
  }

  //mark all detected signs in a black image
  Mat test = work.clone();
  test.setTo(Scalar(0,0,0));
  draw(test, red(work), Scalar(0,0,255));
  draw(test, yellow(work), Scalar(0,255,255));
  draw(test, blue(work), Scalar(255,0,0));
  imshow("signs", test);
  return result;
}

/**
 * returns all red colored signs
 * @param img   input image
 * @return      all detected red signs
 */
vector<pair<Point, uint> > red(Mat img)
{
  Mat imgr1, imgr;
  vector<vector<Point> > contours;
  vector<Point> approx;
  Moments mo;
  int k = 0;

  inRange(img, Scalar(170,150,120), Scalar(179,255,255), imgr1); //filter red
  inRange(img, Scalar(0,150,120), Scalar(20,255,255), imgr); //filter other red
  addWeighted(imgr1, 1.0, imgr, 1.0, 0.0, imgr);//add them

  GaussianBlur(imgr, imgr, Size(5,5), 0);//blur the image
  Canny(imgr, imgr, 5, 50, 5);//find the edges

  findContours(imgr.clone(), contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//extract the contours

  vector<pair<Point, uint> > signs(contours.size()), circles;
  for(uint i = 0; i < contours.size(); i++)//loop over all contours
  {
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.012, true);//approximate the contour

    if(fabs(contourArea(approx)) < 50)//filter out small contours
      continue;

    if(approx.size() == 3 && equalLength(approx, 1))//equilateral triangle
    {
      mo = moments(approx);
      signs[k].first.x = mo.m10 / mo.m00;
      signs[k].first.y = mo.m01 / mo.m00;
      if(orientation(approx, signs[k].first.y))//pointing upwards or downwards
        signs[k++].second = 2;
      else
      {
        int n = detectShape(copyContour(img, approx));//detect shapes inside the triangle
        if(n == -1)
          signs[k++].second = 3;
        else
          signs[k++].second = (uint) n;
      }
    }
    else if(approx.size() == 8 && isContourConvex(approx) && equalLength(approx, 1))//convex equilateral octagon
    {
      mo = moments(approx);
      signs[k].first.x = mo.m10 / mo.m00;
      signs[k].first.y = mo.m01 / mo.m00;
      signs[k++].second = 1;
    }
  }

  circles = findRedCircles(img, imgr);
  if(circles.size() != 0)
    signs.insert(signs.begin()+k, circles.begin(), circles.end());

  signs.resize(k+circles.size());
  return signs;
}

/**
 * returns all yellow colored signs
 * @param img   input image
 * @return      all detected yellow signs
 */
vector<pair<Point,uint> > yellow(Mat img)
{
  Mat imgy;
  vector<vector<Point> > contours;
  vector<Point> approx;
  Moments mo;
  int k = 0;

  inRange(img, Scalar(16,130,80), Scalar(30,255,255), imgy); //filter yellow
  GaussianBlur(imgy, imgy, Size(5,5), 0);//blur the image
  Canny(imgy, imgy, 5, 50, 5);//only the edges left

  findContours(imgy, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//extract the contours

  vector<pair<Point,uint> > signs(contours.size());
  for(uint i = 0; i < contours.size(); i++)
  {
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);//approximate the contour

    if(fabs(contourArea(approx)) < 50)//filter out small contours
      continue;

    if(approx.size() == 4 && isContourConvex(approx) && equalLength(approx, 2))//convex equilateral tetragon
    {
      mo = moments(approx);
      signs[k].first.x = mo.m10 / mo.m00;
      signs[k].first.y = mo.m01 / mo.m00;
      signs[k++].second = 4;
    }
  }

  signs.resize(k);
  return signs;
}

/**
 * returns all blue colored signs
 * @param img   input image
 * @return      all detected blue signs
 */
vector<pair<Point,uint> > blue(Mat img)
{
  Mat imgb;
  vector<pair<Point, uint> > signs;

  inRange(img, Scalar(80,130,80), Scalar(120,255,255), imgb); //filter blue
  GaussianBlur(imgb, imgb, Size(5,5), 0);//blur the image
  Canny(imgb, imgb, 5, 50, 5);//only the edges left

  signs = findCircles(imgb);

  return signs;
}

/**
 * Detects circles
 * @param img   input image where to find the circles
 * @return      center of the detected circles and dummy sign type
 */
vector<pair<Point, uint> > findCircles(Mat img)
{
  vector<Vec3f> circles;

  HoughCircles(img, circles, CV_HOUGH_GRADIENT, 2, 50, 100, 70, 0, 200);//detect the circles

  vector<pair<Point, uint> > signs(circles.size());
  for(uint i = 0; i < circles.size(); i++)
  {
    signs[i].first.x = circles[i][0];
    signs[i].first.y = circles[i][1];
    signs[i].second = 14;//dummy type
  }

  return signs;
}

/**
 * Detects circles with a black number inside(speedsign)
 * @param img           original image
 * @param imgFiltered   input image where to find the circles
 * @return              center of the circles and sign type
 */
vector<pair<Point, uint> > findRedCircles(Mat img, Mat imgFiltered)
{
  vector<Vec3f> circles;
  int n, k = 0;

  HoughCircles(imgFiltered, circles, CV_HOUGH_GRADIENT, 2, 50, 100, 70, 0, 200);//detect the circles

  vector<pair<Point, uint> > signs(circles.size());
  for(uint i = 0; i < circles.size(); i++)
  {
    n = detectNumber(copyContour(img, circles[i]));//get the number inside

    if(n == -1)//no number detected
      continue;

    signs[k].first.x = circles[i][0];
    signs[k].first.y = circles[i][1];
    signs[k++].second = (uint) n;
  }

  signs.resize(k);
  return signs;
}

/**
 * detects black numbers in the image
 * @param img       input image
 * @return          sign type (-1 if no number was found)
 */


//int detectNumber (Mat img, Vec3f circle){

int detectNumber(Mat img){
 //Mat circ = copyContour(img, circle);

 inRange(img, Scalar(0, 0, 51), Scalar(255, 255, 255), img); //filter black
 tess.SetImage((uchar*)img.data, img.size().width, img.size().height, img.channels(), img.step1());
 //tess.SetRectangle(circle[0]-circle[2], circle[1]-circle[2],2*circle[2],2*circle[2]);
 string out = string (tess.GetUTF8Text());
 out.erase( std::remove_if( out.begin(), out.end(), ::isspace ), out.end() );
 const char* result = out.c_str();
 //printf("%s\n", result);
 switch(atoi(result)){
	case 10: return 5;
	case 20: return 6;
	case 30: return 7;
	case 40: return 8;
	case 50: return 9;
	case 60: return 10;
	case 70: return 11;
	case 80: return 12;
 }
 //rectangle(img, Point(circle[0]-circle[2], circle[1]-circle[2]), Point(circle[0]+circle[2], circle[1]+circle[2]), Scalar (100,255,255),3,8,0);
 // imwrite("./test.png", img);
 return -1;}

/*

int detectNumber(Mat img)
{
  vector<Vec4i> hierarchy;
  vector<vector<Point> > contours;
  double similarity;

  findContours(extractContours(img), contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//extract the contours

  vector<vector<Point> > filtered(contours.size());
  for(uint i = 0; i < contours.size(); i++)
  {
    if(hierarchy[i][3] == -1 || fabs(contourArea(contours[i])) < 50)//ignore duplicate or small contours
      continue;

    for(uint j = 1; j < 9; j++)//loop over all numbers except zero
    {
      similarity = matchShapes(contours[i], references[j], CV_CONTOURS_MATCH_I2, 0);//compare the contour an the reference
      if(similarity < 0.3){//0.05 for I1  0.3 for I2
        return (int) j+4;
      }
    }
  }
  return -1;
}
*/

/**
 * detects black shapes in the image which are similar to the references
 * @param img       input image
 * @return          sign number (-1 if no matching contour was found)
 */

int detectShape(Mat img)
{
  vector<Vec4i> hierarchy;
  vector<vector<Point> > contours;
  double similarity;

  findContours(extractContours(img), contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//extract the contours

  vector<vector<Point> > filtered(contours.size());
  for(uint i = 0; i < contours.size(); i++)
  {
    if(hierarchy[i][3] == -1 || fabs(contourArea(contours[i])) < 50)//ignore duplicate or small contours
      continue;

    for(uint j = 9; j < references.size(); j++)//loop over all references
    {
      similarity = matchShapes(contours[i], references[j], CV_CONTOURS_MATCH_I2, 0);//compare the contour an the reference
      if(similarity < 0.4){//0.05 for I1  0.4 for I2
        return (int) j+4;
      }
    }
  }
  return -1;//no match found
}


/**
 * extracts all contours of black shapes
 * @param img   source image
 * @return      image with only contours left
 */
Mat extractContours(Mat img)
{
  Mat gray(img.rows, img.cols, CV_8UC1);
  int ch[] = {2, 0};//only value is used
  mixChannels(&img, 1, &gray, 1, ch, 1);//convert to grayscale
  threshold(gray, gray, 170, 255, THRESH_BINARY);//threshold that only the black is left
  Canny(gray, gray, 5, 50, 5);//find the edges
  return gray;
}

/**
 * computes the square of the length between two points
 * @param a     first point
 * @param b     second point
 * @return      the square of the length between a and b
 */
double sqLength(Point a, Point b)
{
  return (fabs(a.x-b.x)*fabs(a.x-b.x)) + (fabs(a.y-b.y)*fabs(a.y-b.y));
}

/**
 * Checks whether the distances between all neighbored points are nearly equal
 * @param shape     the shape that shall be checked
 * @param factor    factor for the allowed variation
 * @return          if the shape is nearly equilateral
 */
bool equalLength(vector<Point> shape, int factor)
{
  double dist = 0;
  for(uint i = 0; i < shape.size()-1; i++)
  {
    dist += sqLength(shape[i], shape[i+1]);
  }
  dist /= shape.size();

  for(uint i = 0; i< shape.size()-1; i++)
  {
    if((sqLength(shape[i], shape[i+1])-dist) > (dist / factor))
      return false;
  }
  return true;
}

/**
 * checks whether the triangle is pointing up or downwards
 * @param triangle  the triangle
 * @param cy    the y-coordinate of the center of the triangle
 * @return      true if it points downwards
 */
bool orientation(vector<Point> triangle, double cy)
{
  int k = 0;
  for(int i = 0; i < 3; i++)
  {
    if(triangle[i].y > cy)
      k++;
  }
  return k < 2;
}

/**
 * returns whether two signs have nearly the same center
 * @param a     first sign
 * @param b     second sign
 * @return      if a and b have nearly the same center
 */
bool equalSign(pair<Point,uint> a, pair<Point,uint> b)
{
    return (((b.first.x-a.first.x) < 5) && (abs(b.first.y - a.first.y) < 5));
}

/**
 * draws a small circle into the image for each sign at its location
 * @param img       the image to draw into
 * @param signs     the signs to be marked
 * @param color     the color in which to draw the circles
 */
void draw(Mat img, vector<pair<Point,uint> > signs, Scalar color)
{
  for(uint i = 0; i < signs.size(); i++)
  {
    circle(img, signs[i].first, 5, color);
  }
}

/**
 * copies the image inside the circle into a new image
 * @param img       source image
 * @param contour   the circle where to copy the image
 * @result          new image of everything inside the bounding box of the circle
 */
Mat copyContour(Mat img, Vec3f contour)
{
  Rect bbox(contour[0]-contour[2], contour[1]-contour[2], 2*contour[2], 2*contour[2]);
  return img(bbox).clone();
}

/**
 * copies the image inside the contour into a new image
 * @param img       source image
 * @param contour   the contour where to copy the image
 * @result          new image of everything inside the bounding rectangle of the contour
 */
Mat copyContour(Mat img, vector<Point> contour)
{
  return img(boundingRect(contour)).clone();
}

/**
 * loads all references defined in images
 * @return      the contours of the references
 */
/*
vector<vector<Point> > getReferences(){
  vector<vector<Point> > contours, result;
  Mat im;

  for(uint i = 0; i < images.size(); i++)
  {
    im = imread(images[i], CV_LOAD_IMAGE_GRAYSCALE);
    findContours(im, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    result.push_back(contours[0]);
  }

  return result;
}
*/