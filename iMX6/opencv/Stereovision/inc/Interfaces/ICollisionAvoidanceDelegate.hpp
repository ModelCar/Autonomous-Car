//
// Created by simonso on 18.06.16.
//

#ifndef SV_ICOLLISIONAVOIDANCEDELEGATE_H
#define SV_ICOLLISIONAVOIDANCEDELEGATE_H

#include "vector"
class S_Tentacle;

class ICollisionAvoidanceDelegate {

public:
    ICollisionAvoidanceDelegate() {}
    virtual ~ICollisionAvoidanceDelegate() {}
    virtual void didReceiveAvoidanceData(std::vector<S_Tentacle> tentacles) = 0;
};

#endif //SV_ICOLLISIONAVOIDANCEDELEGATE_H