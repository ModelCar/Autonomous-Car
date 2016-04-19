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

#include "Signs.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

vector<vector<Point> > references;
vector<String> images = {"../sign_references/0.png","../sign_references/1.png","../sign_references/2.png",
                        "../sign_references/3.png","../sign_references/4.png","../sign_references/5.png",
                        "../sign_references/6.png","../sign_references/7.png","../sign_references/8'.png",
                        "../sign_references/vorfahrt.png"};

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

  Rect box(0, 0, img.cols, img.rows-20);
  work = img(box).clone();

  cvtColor(work, work, COLOR_BGR2HSV);//change to HSV and create working copy

  result = red(work, img.at<Vec3b>(Point(108,232)));//all red signs
  tmp = yellow(work, img.at<Vec3b>(Point(98,232)));//all yellow signs
  result.insert(result.end(), tmp.begin(), tmp.end());

  if(result.size() > 5 || result.size() == 0)
    return {};

  sort(result.begin(), result.end(), [](pair<Point,uint> a, pair<Point,uint> b){return a.first.x<b.first.x;});//needed for erase
  result.erase(unique(result.begin(), result.end(), equalSign), result.end());//erase duplicates
  return result;
}

/**
 * returns all red colored signs
 * @param img   input image
 * @param color	the reference color for the filter
 * @return      all detected red signs
 */
vector<pair<Point, uint> > red(Mat img, Vec3b color)
{
  Mat imgr1, imgr;
  vector<vector<Point> > contours;
  vector<Point> approx;
  Moments mo;
  int k = 0;

  inRange(img, Scalar(170,color[1]-40,color[2]-40), Scalar(179,255,255), imgr1); //filter red
  inRange(img, Scalar(0,color[1]-40,color[2]-40), Scalar(15,255,255), imgr); //filter other red
  addWeighted(imgr1, 1.0, imgr, 1.0, 0.0, imgr);//add them

  GaussianBlur(imgr, imgr, Size(5,5), 0);//blur the image
  Canny(imgr, imgr1, 5, 50);//find the edges

  findContours(imgr1, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//extract the contours

  vector<pair<Point, uint> > signs(contours.size()), circles;
  for(uint i = 0; i < contours.size(); i++)//loop over all contours
  {
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.03, true);//approximate the contour

    if(fabs(contourArea(approx)) < 50)//filter out small contours
      continue;

    if(approx.size() == 3 && equalLength(approx, 1.5))//equilateral triangle
    {
      mo = moments(approx);
      signs[k].first.x = mo.m10 / mo.m00;
      signs[k].first.y = mo.m01 / mo.m00;
      if(orientation(approx, signs[k].first.y))
      {//pointing upwards or downwards
        if(img.at<Vec3b>(Point(signs[k].first.x,signs[k].first.y))[2] > 150)
        {//is center white
          signs[k++].second = 2;
        }
      }
      else
      {
        int n = detectShape(copyContour(img, approx));//detect shapes inside the triangle
        if(n == -1)//None detected
          signs[k++].second = 3;
        else
          signs[k++].second = (uint) n;
      }
    }
    else if(approx.size() == 8 && isContourConvex(approx) && equalLength(approx, 1))//convex equilateral octagon
    {
      approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.01, true);//approximate the contour finer
      if(approx.size() != 8)
        continue;
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
 * @param color	the reference color for the filter
 * @return      all detected yellow signs
 */
vector<pair<Point,uint> > yellow(Mat img, Vec3b color)
{
  Mat imgy;
  vector<vector<Point> > contours;
  vector<Point> approx;
  Moments mo;
  int k = 0;

  inRange(img, Scalar(20,color[1]-40,color[2]-40), Scalar(40,255,255), imgy); //filter yellow
  GaussianBlur(imgy, imgy, Size(5,5), 0);//blur the image
  Canny(imgy, imgy, 5, 50);//only the edges left

  findContours(imgy, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//extract the contours

  vector<pair<Point,uint> > signs(contours.size());
  for(uint i = 0; i < contours.size(); i++)
  {
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.03, true);//approximate the contour

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
 * Detects circles with a black number inside(speedsign)
 * @param img           original image
 * @param imgFiltered   input image where to find the circles
 * @return              center of the circles and sign type
 */
vector<pair<Point, uint> > findRedCircles(Mat img, Mat imgFiltered)
{
  vector<Vec3f> circles;
  int n, k = 0;

  HoughCircles(imgFiltered, circles, HOUGH_GRADIENT, 2, 60, 100, 70, 15, 60);//detect the circles

  vector<pair<Point, uint> > signs(circles.size());
  for(uint i = 0; i < circles.size(); i++)
  {
    if(circles[i][0]-circles[i][2]>0 && circles[i][1]-circles[i][2]>0 && circles[i][0]+circles[i][2]<img.cols && circles[i][1]+circles[i][2]<img.rows)
    {

      n = detectNumber(copyContour(img, circles[i]));//get the number inside

	  if(n == -1)//no number detected
        continue;

      signs[k].first.x = circles[i][0];
      signs[k].first.y = circles[i][1];
      signs[k++].second = (uint) n;
    }
  }

  signs.resize(k);
  return signs;
}

/**
 * detects black numbers in the image
 * @param img       input image
 * @return          sign type (-1 if no number was found)
 */
int detectNumber(Mat img)
{
  vector<Vec4i> hierarchy;
  vector<vector<Point> > contours;
  double similarity, temp = 0.5;
  int result = -5;

  findContours(extractContours(img), contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//extract the contours

  vector<vector<Point> > filtered(contours.size());
  for(uint i = 0; i < contours.size(); i++)
  {
    if(fabs(contourArea(contours[i])) < 40)//ignore duplicate or small contours
      continue;

    if(matchShapes(contours[i], references[0], CV_CONTOURS_MATCH_I2, 0) < 0.3)//check if it's the 0
      continue;

    for(uint j = 1; j < 9; j++)//loop over all numbers except zero
    {
      similarity = matchShapes(contours[i], references[j], CV_CONTOURS_MATCH_I2, 0);//compare the contour and the reference
      if(similarity < temp){
        temp = similarity;
        result = (int) j;
      }
    }
  }

  return result+4;
}

/**
 * detects black shapes in the image which are similar to the references
 * @param img       input image
 * @return          sign number (-1 if no matching contour was found)
 */
int detectShape(Mat img)
{
  vector<Vec4i> hierarchy;
  vector<vector<Point> > contours;
  double similarity, temp = 0.5;
  int result = -5;

  findContours(extractContours(img), contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//extract the contours

  vector<vector<Point> > filtered(contours.size());
  for(uint i = 0; i < contours.size(); i++)
  {
    if(fabs(contourArea(contours[i])) < 40)//ignore duplicate or small contours
      continue;

    for(uint j = 9; j < references.size(); j++)//loop over all references
    {
      similarity = matchShapes(contours[i], references[j], CV_CONTOURS_MATCH_I2, 0);//compare the contour and the reference
      if(similarity < temp){
        temp = similarity;
        result = (int) j;
      }
    }
  }

  return result + 4;
}

/**
 * extracts all contours of black shapes
 * @param img   source image
 * @return      image with only contours left
 */
Mat extractContours(Mat img)
{
  Mat temp, gray(img.rows, img.cols, CV_8UC1);
  int ch[] = {2, 0};//only value is used
  mixChannels(&img, 1, &gray, 1, ch, 1);//convert to grayscale
  int thresh = threshold(gray, temp, 0, 255, THRESH_OTSU);//threshold that only the black is left
  if(thresh < 170)
  {
    Canny(temp, temp, 5, 50);//find the edges
    return temp;
  }
  threshold(gray, gray, 170, 255, THRESH_BINARY);//threshold that only the black is left
  Canny(gray, gray, 5, 50);//find the edges
  return gray;
}

/**
 * computes the square of the length between two points
 * @param a     first point
 * @param b     second point
 * @return      the square of the length between a and b
 */
int sqLength(Point a, Point b)
{
  return ((a.x-b.x)*(a.x-b.x)) + ((a.y-b.y)*(a.y-b.y));
}

/**
 * Checks whether the distances between all neighbored points are nearly equal
 * @param shape     the shape that shall be checked
 * @param factor    factor for the allowed variation
 * @return          if the shape is nearly equilateral
 */
bool equalLength(vector<Point> shape, double factor)
{
  double dist = 0;
  for(uint i = 0; i < shape.size()-1; i++)
  {
    dist += sqLength(shape[i], shape[i+1]);
  }
  dist /= shape.size();
  dist = (dist / factor) + dist;

  for(uint i = 0; i< shape.size()-1; i++)
  {
    if(sqLength(shape[i], shape[i+1]) > dist)
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
 * copies the image inside the circle into a new image
 * @param img       source image
 * @param circle   the circle where to copy the image
 * @result          new image of everything inside the bounding box of the circle
 */
Mat copyContour(Mat img, Vec3f circle)
{
  Rect bbox(circle[0]-circle[2], circle[1]-circle[2], 2*circle[2], 2*circle[2]);
  return (img(bbox).clone());
}

/**
 * copies the image inside the contour into a new image
 * @param img       source image
 * @param contour   the contour where to copy the image
 * @result          new image of everything inside the bounding rectangle of the contour
 */
Mat copyContour(Mat img, vector<Point> contour)
{
  return (img(boundingRect(contour)).clone());
}

/**
 * loads all references defined in images
 * @return      the contours of the references
 */
void loadReferences(){
  vector<vector<Point> > contours;
  Mat im;

  for(uint i = 0; i < images.size(); i++)
  {
    im = imread(images[i], CV_LOAD_IMAGE_GRAYSCALE);
    if(!im.data)
    {
      cout << "failed to load image: " << images[i] << endl;
      continue;
    }
    findContours(im, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    references.push_back(contours[0]);
  }
}
