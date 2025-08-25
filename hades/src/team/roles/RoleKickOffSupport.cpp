//
// Created by caiu on 18/08/25.
//

#include "RoleKickOffSupport.h"
#include "../RobotController.h"
#include "../TeamInfo.h"

namespace roles {
    void RoleKickOffSupport::act(RobotController& robot) {
        Point goal = {robot.mTeam->our_side_sign*robot.mRadius , 1000};
        moveTo.act(robot, goal, true);
    }
} // roles