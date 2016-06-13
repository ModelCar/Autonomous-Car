//
// Created by simonso on 13.06.16.
//

#ifndef SV_ISTEREOVISIONCALLBACK_HPP
#define SV_ISTEREOVISIONCALLBACK_HPP

class IStereovisionCallback {

public:
    IStereovisionCallback() {}
    virtual ~IStereovisionCallback() {}

    //callback functions
    virtual void didReceiveStereoData() = 0;
    virtual void didReceiveAvoidanceData() = 0;

};

#endif //SV_ISTEREOVISIONCALLBACK_HPP
