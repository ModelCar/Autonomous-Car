#ifndef MATCHINGMETHOD_HPP_
#define MATCHINGMETHOD_HPP_

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.hpp"
#include "opencv2/highgui/highgui.hpp"
/**
 * Main class implementing derived functions inherited by matching methods classes
 */
class MatchingMethod {
public:
	/**
	 * Class constructor
	 */
	MatchingMethod(bool show = true):
		preFilterCap(25),
        blockSize(143),
		minDisparity(0),
		numberOfDisparities(50),
		textureThreshold(10),
		uniquenessRatio(0),
		speckleWindowSize(37),
		speckleRange(36),
		disp12MaxDiff(39),
        showParamWin(show){}
	/**
	 * Obtains depth map from camera stream images using algorithm specified by inheriting method
	 * @param Mat - left frame
	 * @param Mat - right frame
	 * @return Mat - depthmap
	 */
	virtual cv::Mat getDisparity(cv::Mat left, cv::Mat right) = 0;


	void setRoi(cv::Rect,cv::Rect);
	/**
	 * Creates parameter window for specific method
	 * @param string - window name
	 * @param cv::Size - window position
	 */
	virtual void createParameterWindow(std::string,cv::Size) = 0;
	/**
	 * Class destructor
	 */
    virtual ~MatchingMethod() {}
protected:

	bool showParamWin;
	/** DEPTH SUBSTRACTION METHODS PARAMETERS**/
	int preFilterCap;
    int blockSize;
	int minDisparity;
	int numberOfDisparities;
	int textureThreshold;
	int uniquenessRatio;
	int speckleWindowSize;
	int speckleRange;
	int disp12MaxDiff;
	cv::Rect roi1,roi2;
};

#endif /* MATCHINGMETHOD_HPP_ */
