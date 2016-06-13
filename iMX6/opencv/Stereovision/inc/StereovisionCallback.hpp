//
// Created by simonso on 13.06.16.
//

#ifndef SV_STEREOVISIONCALLBACK_H
#define SV_STEREOVISIONCALLBACK_H

#include "Interfaces/IStereovisionCallback.hpp"

class StereovisionCallback : IStereovisionCallback {

public:
    StereovisionCallback();
    ~StereovisionCallback();

    //callback functions
    void didReceiveStereoData();
    void didReceiveAvoidanceData();
};
#endif //SV_STEREOVISIONCALLBACK_H
