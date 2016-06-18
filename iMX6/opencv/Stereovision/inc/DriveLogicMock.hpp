//
// Created by simonso on 13.06.16.
//

#ifndef SV_STEREOVISIONCALLBACK_H
#define SV_STEREOVISIONCALLBACK_H

#include <stdio.h>
#include "Interfaces/IStereovisionDelegate.hpp"
#include "Interfaces/ICollisionAvoidanceDelegate.hpp"
#include "StereoVision.hpp"

class DriveLogicMock : public IStereovisionDelegate, public ICollisionAvoidanceDelegate {

public:
    DriveLogicMock();
    ~DriveLogicMock();

    //callback functions
    void didReceiveStereoData();
    void didReceiveAvoidanceData(std::vector<S_Tentacle> tentacles);

};
#endif //SV_STEREOVISIONCALLBACK_H
