#ifndef SGBMMETHOD_HPP_
#define SGBMMETHOD_HPP_

#include "../matchingmethod.hpp"

/**
 * Semi-Global Block Matching Stereo Correspondence Algorithm class
 */
class SGBMMethod: public MatchingMethod {
public:
	SGBMMethod(bool show = true);
	cv::Mat getDisparity(cv::Mat left, cv::Mat right);
	void createParameterWindow(std::string,cv::Size position);
    virtual ~SGBMMethod() {}
private:
	int mode;
};

#endif /* SGBMMETHOD_HPP_ */
