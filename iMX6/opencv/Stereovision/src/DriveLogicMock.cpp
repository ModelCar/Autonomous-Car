//
// Created by simonso on 13.06.16.
//

#include "../inc/DriveLogicMock.hpp"

using namespace std;

DriveLogicMock::DriveLogicMock() {

}

DriveLogicMock::~DriveLogicMock() {
}

void DriveLogicMock::didReceiveStereoData() {
    cout << "Did receive Stereo Data!" << endl;
}

void DriveLogicMock::didReceiveAvoidanceData(vector<S_Tentacle> tentacles) {
    cout << "Did receive avoidance Data!" << endl;

    double preferredDrivingAngle = 0.0;

    for(auto it = tentacles.begin(); it != tentacles.end(); it++) {
        if(it->isSafePath) {
            cout << "Safe path at: " << it->steeringAngle << endl;
            if(it->steeringAngle == preferredDrivingAngle) {
                //TODO: choose preferred angle for driving
            }
        }
    }
}