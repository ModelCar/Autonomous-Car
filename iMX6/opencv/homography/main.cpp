#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Arguments dont match.\n");
        return -1;
    }

    FileStorage fsReadCalibration(argv[2], FileStorage::READ);

    string date;
    fsReadCalibration["calibration_Time"] >> date;

    int nrFrames, imageWidth, imageHeight, boardW, boardH;
    double squareSize, fixAspectRatio;

    fsReadCalibration["nrOfFrames"] >> nrFrames;
    fsReadCalibration["image_Width"] >> imageWidth;
    fsReadCalibration["image_Height"] >> imageHeight;
    fsReadCalibration["board_Width"] >> boardW;
    fsReadCalibration["board_Height"] >> boardH;
    fsReadCalibration["square_Size"] >> squareSize;
    fsReadCalibration["FixAspectRatio"] >> fixAspectRatio;

    cout << "Board W:" << boardW << " H: " << boardH << endl;

    Mat cameraMatrix, distCoeffs;
    fsReadCalibration["Camera_Matrix"] >> cameraMatrix;
    fsReadCalibration["Distortion_Coefficients"] >> distCoeffs;

    int boardN = boardW * boardH;
    Size boardSZ = Size( boardW, boardH );

    Mat image = imread(argv[1]);
    Mat grayImage, tmp, H , birdsImage;

    Mat undistorted;
    undistort(image,undistorted,cameraMatrix,distCoeffs,cameraMatrix);

    image = undistorted;

    Point2f objPts[4], imgPts[4] ;
    std::vector<cv::Point2f> corners;
    float Z = 1 ;
    int key = 0 ;
    int found = findChessboardCorners( image, boardSZ, corners );

    if (found) {
        drawChessboardCorners(image, boardSZ, corners, 1);
        cvtColor( image, grayImage, CV_RGB2GRAY );
        cornerSubPix( grayImage, corners, Size(11, 11), Size(-1, -1),
                      TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1) );
        resize( image, tmp, Size(), .5, .5 ) ;
        //namedWindow( "image" );
        imshow( "image" , tmp );
        waitKey(1);

    }

    imgPts[0] = corners[0];
    imgPts[1] = corners[boardW-1];
    imgPts[2] = corners[(boardH-1)*boardW];
    imgPts[3] = corners[(boardH-1)*boardW + boardW-1];

    int offsetX = 900;
    int offsetY = 2000;
    //squareSize = 5;
    objPts[0].x = offsetX + 0;
    objPts[0].y = offsetY + 0;
    objPts[1].x = offsetX + (boardW -1) * squareSize;
    objPts[1].y = offsetY + 0;
    objPts[2].x = offsetX + 0;
    objPts[2].y = offsetY + (boardH -1) * squareSize;
    objPts[3].x = offsetX + (boardW -1) * squareSize;
    objPts[3].y = offsetY + (boardH -1) * squareSize;

    // find homography
    H.create( 3, 3, CV_32F);
    H = getPerspectiveTransform( objPts, imgPts);

    // Write homography to file
    FileStorage fs("Homography.xml", FileStorage::WRITE);
    fs.open("Homography.xml",FileStorage::WRITE);
    fs << "Homography" << H;
    fs.release();
    //We can reuse H for the same camera mounting
    namedWindow( "Bird" ) ;
    birdsImage = image;
    while ( key != 27 ) {
        H.at<float>(2,2) = Z ;

        warpPerspective( image, birdsImage, H, Size(image.cols *4 , 5*image.rows ) ,
                             CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS ) ;
        Mat tmp2;
        resize( birdsImage, tmp2, Size(), .25, .25 ) ;
        imshow( "Bird", tmp2 ) ;
        key = waitKey(0);
        if(key == 'u') Z += 1;
        if(key == 'd') Z -= 1;
    }
    return 0;
}
