#ifndef OPENCV_UTILS
#define OPENCV_UTILS
#include <vector>

//default opencv includes
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

namespace cv_utils{

//Not needed yet
//inline cv::Mat croppResize(const cv::Mat& input, int width,int height, int alignX, int alignY);
//inline cv::Mat croppResizeCentered(const cv::Mat& input, int width, int height);

    inline cv::Mat croppResize(const cv::Mat& input, int width,int height, int alignLeft, int alignRight,int alignBot,int alignTop){
        cv::Mat cropped(input,cv::Rect(alignLeft,alignBot,input.cols-alignLeft-alignRight,input.rows-alignBot-alignTop));
        cv::Mat output;
        cv::resize(cropped,output,cv::Size(width,height));
        return output;
    }
    inline cv::Mat croppResizeCentered(const cv::Mat& input, int width, int height){
        float ratio = ((float)width)/height;
        float currentRatio = ((float)input.cols)/input.rows;
        int alignX = 0;
        int alignY = 0;
        if(ratio > currentRatio){
            alignX = input.cols-input.rows*ratio;
        }else if(ratio < currentRatio){
            alignY = input.rows-input.cols*ratio;
        }
        return croppResize(input,width,height,alignX/2,alignX/2,alignY/2,alignY/2);

    }
}

#endif
