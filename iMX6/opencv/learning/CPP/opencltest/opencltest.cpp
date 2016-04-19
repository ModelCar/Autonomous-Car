#include <iostream>
#include <ctime>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/ocl.hpp>

using namespace std;

int main()
{
    cout << "Have OpenCL?: " << cv::ocl::haveOpenCL() << endl;

/*  SWITCH OPENCL ON/OFF IN LINE BELLOW */
    cv::ocl::setUseOpenCL(true);
/*                                      */
    int nBlurs = 50;

    cv::VideoCapture cam;

    if (!cam.open(0))
        std::cout << "Problem connecting to cam " << std::endl;
    else
        std::cout << "Successfuly connected to camera " << std::endl;

    long frameCounter = 0;

    std::time_t timeBegin = std::time(0);
    int tick = 0;

    cv::UMat frame;
    cv::UMat frameGray;
    cv::UMat frameSobelx;
    cv::UMat frameSobely;

    cv::UMat frameSobel;
    cv::UMat blurredSobel;

    while (1)
    {
        cam.read(frame);
        cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);

        cv::Sobel(frameGray, frameSobelx, frameGray.depth(), 1, 0, 3);
        cv::Sobel(frameGray, frameSobely, frameGray.depth(), 0, 1, 3);

        cv::bitwise_or(frameSobelx, frameSobely, frameSobel);

        for (int n = 0; n < nBlurs; n++)
            cv::blur(frameSobel, blurredSobel, cv::Size(3,3));

        //cv::imshow("Sobel blurred Frame", blurredSobel);
        //cv::waitKey(1);

        frameCounter++;

        std::time_t timeNow = std::time(0) - timeBegin;

        if (timeNow - tick >= 1)
        {
            tick++;
            cout << "Frames per second: " << frameCounter << endl;
            frameCounter = 0;
        }
    }

    return 0;
}