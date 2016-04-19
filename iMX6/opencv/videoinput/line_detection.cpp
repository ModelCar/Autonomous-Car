#include "line_detection.h"

/**
 * @brief OpencvLineDetection::showBirdView shows a Bird view from an image
 * @param srcImage src Image to convert
 * @param H Homography
 * @param Z heigth of the bird view (default: 1)
 * @param dstImageSize size of the bird view
 */
void OpencvLineDetection::showBirdView(cv::Mat srcImage, cv::Mat H, double Z, cv::Size dstImageSize){
    H.at<float>(2,2) = Z ;
    cv::Mat birdsImage;
    cv::warpPerspective( srcImage, birdsImage, H, dstImageSize ,
                         CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS ) ;
    cv::imshow( "BirdView",  birdsImage) ;
    cv::waitKey(0);
}

/**
 * @brief OpencvLineDetection::readHomography reads a File and parse its Homography (!!opencvMatrix)
 * @param path to XML/YAML file of homography
 * @return Mat homography
 */
cv::Mat OpencvLineDetection::readHomography(std::string path){
    cv::FileStorage fsReadH(path, cv::FileStorage::READ);
    fsReadH.open(path, cv::FileStorage::READ);
    cv::Mat H;
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
cv::Point2d OpencvLineDetection::convertPoint(cv::Mat h, cv::Point2d p){
    double x = p.x;
    double y = p.y;
    double Z = 1./(h.at<double>(2,0)*x + h.at<double>(2,1)*y + h.at<double>(2,2));
    int px = (int)((h.at<double>(0,0)*x + h.at<double>(0,1)*y + h.at<double>(0,2))*Z);
    int py = (int)((h.at<double>(1,0)*x + h.at<double>(1,1)*y + h.at<double>(1,2))*Z);
    cv::Point2d res = cv::Point2d(px,py);
    return res;
}

/**
 * @brief OpencvLineDetection::searchLine
 * @param imagen
 * @return
 */
bool OpencvLineDetection::searchLine(cv::Mat imagen) {

    // Load a new Image from Path
    // Load image as greyscaled
    // Check if image has data and return -1 if not
    if ( !imagen.data )
        {
            printf("No image data \n");
            return 0;
        } else {
//             printf("Got image\n");
    }

    // init new image and copy content from previous loaded one
    cv::Mat imagecp;
    cv::Rect rect(0, 0, imgWidth, imgHeight);
    imagen(rect).copyTo(imagecp);
    cv::cvtColor(imagecp, imagecp, CV_GRAY2RGB);


    // show first loaded start image

    //cv::imshow("Start Image",imagen);
    //cv::waitKey(1);

    // init a new region of interest
    cv::Mat roiImg;


    cv::Rect roi(startX, startY, roiX, roiY);
    imagen(roi).copyTo(roiImg);

    // prepare with blurring
    cv::blur(roiImg, edge, cv::Size(3,3));

    // Run the edge detector on grayscale
    cv::Canny(edge, edge, edgeThresh, edgeThresh*3, 3);
    cedge = cv::Scalar::all(0);

    roiImg.copyTo(cedge, edge);
    cv_utils::croppResizeCentered(cedge,roiX,roiY);

    //cv::threshold(cedge,cedge,thresh,255,0);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::RNG rng(12345);

    findContours( cedge, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    cv::Mat drawing = cv::Mat::zeros( cedge.size(), CV_8UC3 );

    // Filter small contours by thrMinContour
    for(std::vector<std::vector<cv::Point> >::iterator it = contours.begin(); it!=contours.end();){
        if (it->size()<thrMinContour){
            it=contours.erase(it);
        }
        else{
            ++it;
        }
    }

    // convert greyimage to rgb colors to mark with colored points later
    cv::cvtColor(roiImg, roiImg, CV_GRAY2RGB);
    for( int i = 0; i< contours.size(); i++ ){
        //set colors
        cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

        //draw different contours
        cv::drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );

        //Approximate Poly Lines with Douglas Peucker alg
        cv::approxPolyDP(cv::Mat(contours[i]), contours[i], 10, true);
        printf("Points:\n");
        for(unsigned int j=0;j<contours[i].size();j++){
            printf("line_detection Point(x,y)= %i , %i\n", contours[i][j].x , contours[i][j].y);
            //draw circles
            cv::circle( roiImg, contours[i][j], 3, cv::Scalar(0,0,255), cv::FILLED, cv::LINE_AA );
        }

    }

    // Check for similar contours next to each other
    // if so it is a line
    for(int i = 0; i< contours.size(); i++){
        for(int j = 0; j< contours.size(); j++){
            int isEq = 0;
            if(i != j && i < j){
                isEq = matchContours(contours[i],contours[j]);
            }
            if(isEq){
                cv::Point pStart = cv::Point((int) (startX + (contours[i][0].x+contours[j][0].x)/2) , (int) (startY + (contours[i][0].y+contours[j][0].y)/2) );
                cv::Point pEnd = cv::Point((int) (startX + (contours[i][1].x+contours[j][1].x)/2) , (int) (startY + (contours[i][1].y+contours[j][1].y)/2) );
                cv::line(imagecp,pStart,pEnd,cv::Scalar(0,255,0),2,cv::LINE_8,0);
                std::vector<cv::Point> joinedContour;
                joinedContour.push_back(pStart);
                joinedContour.push_back(pEnd);
                lanes.push_back(joinedContour);

                //printf("matched %i with %i result: %d \n",i,j,isEq);
                //printf("lanesFound: %d \n",(int) lanes.size());
            }
        }
    }

    /*
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP( cedge, lines, 1, CV_PI/180, 0, 30, 1 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
      cv::line( cedge, cv::Point(lines[i][0], lines[i][1]),
          cv::Point(lines[i][2], lines[i][3]), cv::Scalar(0,0,255), 3, 8 );
      printf("x: %i \n", lines[i][0]);
    }
    */

    /*
    cv::imshow("cp",imagecp);
    cv::waitKey(1);
    cv::imshow("roi",roiImg);
    cv::waitKey(1);
    cv::imshow("cedge",cedge);
    cv::waitKey(1);
    cv::imshow("drawing",drawing);
    cv::waitKey(1);


    while(true) {

        if(!imagen.empty()){

        }
        cv::waitKey(50);
    }
    */
    if(lanes.size()>0){
        return true;
    }

    return false;
}

/**
 * @brief OpencvLineDetection::matchContours Matches start and endpoints of contours
 * @param c1 Contour
 * @param c2 Contour
 * @return true if start and endpoints of both contours are in the same area specified by threshholds
 */
bool OpencvLineDetection::matchContours(std::vector<cv::Point> c1, std::vector<cv::Point> c2) {
    int thrX = 25;
    int thrY = 15;
    if(c1.size() != c2.size() && c1.size() != 2){
        return 0;
    }
    if(abs(c1[0].x - c2[0].x) < thrX && abs(c1[0].y - c2[0].y) < thrY && abs(c1[1].x - c2[1].x) < thrX && abs(c1[1].y - c2[1].y) < thrY){
        return 1;
    }else{
        return 0;
    }
}
