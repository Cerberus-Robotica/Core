//
// Created by caiu on 18/08/25.
//

#ifndef ROLEKICKOFFGOALKEEPER_H
#define ROLEKICKOFFGOALKEEPER_H
#include "RoleBase.h"

namespace roles {

class RoleKickOffGoalKeeper : public RoleBase{
public:
    void act(RobotController& robot);
};

} // roles

#endif //ROLEKICKOFFGOALKEEPER_H
