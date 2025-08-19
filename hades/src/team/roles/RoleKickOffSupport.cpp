//
// Created by caiu on 18/08/25.
//

#include "RoleKickOffSupport.h"
#include "../RobotController.h"

namespace roles {
    void RoleKickOffSupport::act(RobotController& robot) {
        double goal[2] = {robot.mTeam->our_side_sign*robot.mRadius , 1000};
        skills::move_to(robot, goal);
    }
} // roles