//
// Created by simonso on 13.06.16.
//

#ifndef SV_ISTEREOVISIONDELEGATE_HPP
#define SV_ISTEREOVISIONDELEGATE_HPP

class IStereovisionDelegate {

public:
    IStereovisionDelegate() {}
    virtual ~IStereovisionDelegate() {}

    //callback functions
    virtual void didReceiveStereoData() = 0;

};

#endif //SV_ISTEREOVISIONDELEGATE_HPP
