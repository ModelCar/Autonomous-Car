#ifndef BMMETHOD_HPP_
#define BMMETHOD_HPP_

#include "../matchingmethod.hpp"

/**
 * The block matching stereo correspondence algorithm class
 */
class BMMethod : public MatchingMethod{
public:
	BMMethod(bool show = true);
	cv::Mat getDisparity(cv::Mat left, cv::Mat right);
	void createParameterWindow(std::string,cv::Size position);
    virtual ~BMMethod(){}
};

#endif /* BMMETHOD_HPP_ */
