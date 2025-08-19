//
// Created by caiu on 18/08/25.
//

#ifndef ROLEGOALKEEPER_H
#define ROLEGOALKEEPER_H
#include "RoleBase.h"

namespace roles {

class RoleGoalKeeper : public RoleBase {
public:
    void act(RobotController& robot);
};

} // roles

#endif //ROLEGOALKEEPER_H
