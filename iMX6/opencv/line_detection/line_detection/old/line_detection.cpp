#include "line_detection.h"

using namespace std;
using namespace cv;

/**
 * @brief OpencvLineDetection::showBirdView shows a Bird view from an image
 * @param srcImage src Image to convert
 * @param H Homography
 * @param Z heigth of the bird view (default: 1)
 * @param dstImageSize size of the bird view
 */
void OpencvLineDetection::showBirdView(Mat srcImage, Mat H, double Z, Size dstImageSize){
    H.at<float>(2,2) = Z ;
    Mat birdsImage;
    warpPerspective( srcImage, birdsImage, H, dstImageSize ,
                         CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS ) ;
    imshow( "BirdView",  birdsImage) ;
    waitKey(0);
}

/**
 * @brief OpencvLineDetection::readHomography reads a File and parse its Homography (!!opencvMatrix)
 * @param path to XML/YAML file of homography
 * @return Mat homography
 */
Mat OpencvLineDetection::readHomography(string path){
    FileStorage fsReadH(path, FileStorage::READ);
    fsReadH.open(path, FileStorage::READ);
    Mat H;
    fsReadH["Homography"] >> H;
    fsReadH.release();
    return H;
}

/**
 * @brief OpencvLineDetection::convertPoint Converts a point from Cartesian coordinates in world coordinates
 * @param h Homography
 * @param p 2D Point Point2d (x,y) to convert
 * @return the new point in world coordinates
 */
Point2d OpencvLineDetection::convertPoint(Mat h, Point2d p){
    double x = p.x;
    double y = p.y;
    double Z = 1./(h.at<double>(2,0)*x + h.at<double>(2,1)*y + h.at<double>(2,2));
    int px = (int)((h.at<double>(0,0)*x + h.at<double>(0,1)*y + h.at<double>(0,2))*Z);
    int py = (int)((h.at<double>(1,0)*x + h.at<double>(1,1)*y + h.at<double>(1,2))*Z);
    Point2d res = Point2d(px,py);
    return res;
}

/**
 * @brief OpencvLineDetection::searchLine
 * @param imagen
 * @return
 */
bool OpencvLineDetection::searchLine(Mat imagen, u_int64_t cycle) {


    // Load a new Image from Path
    // Load image as greyscaled
    // Check if image has data and return -1 if not
    if ( !imagen.data )
        {
            printf("No image data \n");
            return 0;
        } else {
            //printf("Got image\n");
    }

    if (imagen.rows <= 1 || imagen.cols <= 1){
        throw std::invalid_argument( "image to small exception" );
    }

    imgHeight = imagen.rows; // y
    imgWidth = imagen.cols; // x

    if (startX + roiX > imgWidth || startY + roiY > imgHeight){
         throw std::invalid_argument( "ROI out of image exception" );
    }

    // init new image and copy content from previous loaded one
    Mat imagecp, image;
    image = imagen;
    cvtColor(imagen, imagen, CV_BGR2GRAY);
    Rect rect(0, 0, imgWidth, imgHeight);
    imagen(rect).copyTo(imagecp);
    cvtColor(imagecp, imagecp, CV_GRAY2RGB);

    // init a new region of interest
    Mat roiImg;

    Rect roi(startX, startY, roiX, roiY);
    imagen(roi).copyTo(roiImg);

    // prepare with blurring
    blur(roiImg, edge, Size(3,3));

    // Run the edge detector on grayscale
    Canny(edge, edge, edgeThresh, edgeThresh*3, 3);
    cedge = Scalar::all(0);

    roiImg.copyTo(cedge, edge);
    cv_utils::croppResizeCentered(cedge,roiX,roiY);

    //threshold(cedge,cedge,thresh,255,0);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);

    findContours( cedge, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    Mat drawing = Mat::zeros( cedge.size(), CV_8UC3 );

    // Filter small contours by thrMinContour
    for(vector<vector<Point> >::iterator it = contours.begin(); it!=contours.end();){
        if (it->size()<thrMinContour){
            it=contours.erase(it);
        }
        else{
            ++it;
        }
    }

    // convert greyimage to rgb colors to mark with colored points later
    cvtColor(roiImg, roiImg, CV_GRAY2RGB);
    for( int i = 0; i< contours.size(); i++ ){
        //set colors
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

        //draw different contours
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );

        //Approximate Poly Lines with Douglas Peucker alg
        approxPolyDP(Mat(contours[i]), contours[i], thrDP, true);
        //printf("Points:\n");
        for(unsigned int j=0;j<contours[i].size();j++){
            //printf("line_detection Point(x,y)= %i , %i\n", contours[i][j].x , contours[i][j].y);
            //draw circles
            circle( roiImg, contours[i][j], 3, Scalar(0,0,255), FILLED, LINE_AA );
        }

    }

    rectangle(image,Point(startX,startY),Point(startX+roiX-1,startY+roiY-1),Scalar(255,255,0),1,LINE_8,0);

    // Check for similar contours next to each other
    // if so it is a line
    for(int i = 0; i< contours.size(); i++){
        for(int j = 0; j< contours.size(); j++){
            int isEq = 0;
            if(i != j && i < j){
                isEq = matchContours(contours[i],contours[j]);
            }
            if(isEq){
                Point pStart = Point((int) (startX + (contours[i][0].x+contours[j][0].x)/2) , (int) (startY + (contours[i][0].y+contours[j][0].y)/2) );
                Point pEnd = Point((int) (startX + (contours[i][1].x+contours[j][1].x)/2) , (int) (startY + (contours[i][1].y+contours[j][1].y)/2) );
                line(image,pStart,pEnd,Scalar(0,255,255),2,LINE_8,0);
                vector<Point> joinedContour;
                joinedContour.push_back(pStart);
                joinedContour.push_back(pEnd);

                // Check if found contour is already existing
                for(vector<Lane>::iterator it = lanes.begin(); it!=lanes.end();){
                    int updateLanes = matchContours(it->line, joinedContour);
                    if(updateLanes){
                        it->frame = cycle;
                        //cout << "Prio" << it->priority << endl;
                        it->priority += 1;
                        it->line = joinedContour;
                        //cout << "Update Lane" << endl;
                        break;
                    }else{

                    }
                    ++it;
                }

                // add contour to vector
                Lane l;
                l.frame = cycle;
                l.line = joinedContour;
                l.priority = 0;
                lanes.push_back(l);

                break;

                //printf("matched %i with %i result: %d \n",i,j,isEq);
                //printf("lanesFound: %d \n",(int) lanes.size());
            }else{

            }
        }

    }

    // delete non-verifiable contours
    for(vector<Lane>::iterator it = lanes.begin(); it!=lanes.end();){
        // draw contours with high priority in green
        if(it->priority > thrPriority){
            line(image,it->line[0],it->line[1],Scalar(0,255,0),2,LINE_8,0);
        }

        if (cycle - it->frame > bufferNewLines){
            it=lanes.erase(it);
        }
        else{
            ++it;
        }
    }

    /*
     * not used right now
    vector<vector<Point> > joined = joinContours(contours);
    for(int i = 0; i< joined.size(); i++){
        for(int j = 0; j< joined.size(); j++){
            if(i != j && i < j){
                Point pStart = Point((int) (startX + joined[i][0].x) , (int) (startY + joined[i][0].y) );
                Point pEnd = Point((int) (startX + joined[j][1].x) , (int) (startY + joined[j][1].y) );
                line(imagecp,pStart,pEnd,Scalar(0,0,255),2,LINE_8,0);

            }
        }
    }
    */

    /*
    vector<Vec4i> lines;
    HoughLinesP( cedge, lines, 1, CV_PI/180, 0, 30, 1 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
      line( cedge, Point(lines[i][0], lines[i][1]),
          Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
      printf("x: %i \n", lines[i][0]);
    }
    */


    //imshow("cp",image); //imgcp
    //imshow("contours",roiImg);
    //imshow("canny",drawing);
    video = image;
    //if(waitKey(30) >= 0);

    if(lanes.size()>0){
        return true;
    }

    return false;
}

/**
 * @brief OpencvLineDetection::joinContours concatenates contours
 * @param contours
 * @return a vector of joined contours
 */
vector<vector<Point> > OpencvLineDetection::joinContours(vector<vector<Point> > contours){

    vector<vector<Point> > result;

    for(int i = 0; i< contours.size(); i++){
        for(int j = 0; j< contours.size(); j++){
            int isJoin = 0;
            vector<Point> joinedContour;
            if(i != j && i < j){
                joinedContour = isJoinable(contours[i],contours[j]);
                if(joinedContour.size() > 0){
                    isJoin = 1;
                }
            }
            if(isJoin){


                result.push_back(joinedContour);

                //printf("joined %i with %i result: %d \n",i,j,isJoin);
                //printf("lanesFound: %d \n",(int) lanes.size());
            }else{

            }
        }
    }
    return result;
}

/**
 * @brief OpencvLineDetection::isJoinable concatenates contours
 * @param c1 Contour 1
 * @param c2 Contour 2
 * @return concatenates two contours with each other
 * not used right now
 */
vector<Point> OpencvLineDetection::isJoinable(vector<Point> c1, vector<Point> c2){
    vector<Point> joined;
    int thrX = 40;
    int thrY = 40;
    if(c1.size() != c2.size() && c1.size() != 2){
        //return
    }
    if(abs(c1[0].x-c2[1].x) < thrX && abs(c1[0].y-c2[1].y) < thrY){
        joined.push_back(Point(c2[0].x,c2[0].y));
        joined.push_back(Point(c1[1].x,c1[1].y));
    }
    if(abs(c1[1].x-c2[0].x) < thrX && abs(c1[1].y-c2[0].y) < thrY){
        joined.push_back(Point(c1[0].x,c1[0].y));
        joined.push_back(Point(c2[1].x,c2[1].y));

    }
    return joined;
}

/**
 * @brief OpencvLineDetection::matchContours Matches start and endpoints of contours
 * @param c1 Contour
 * @param c2 Contour
 * @return true if start and endpoints of both contours are in the same area specified by threshholds
 */
bool OpencvLineDetection::matchContours(vector<Point> c1, vector<Point> c2) {
    if(c1.size() != c2.size() && c1.size() != 2){
        return 0;
    }
    if(abs(c1[0].x - c2[0].x) < thrX && abs(c1[0].y - c2[0].y) < thrY && abs(c1[1].x - c2[1].x) < thrX && abs(c1[1].y - c2[1].y) < thrY){
        return 1;
    }else{
        return 0;
    }
}
