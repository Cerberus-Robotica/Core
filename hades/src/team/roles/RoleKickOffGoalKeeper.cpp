//
// Created by caiu on 18/08/25.
//

#include "RoleKickOffGoalKeeper.h"
#include "../RobotController.h"

namespace roles {
    void RoleKickOffGoalKeeper::act(RobotController& robot) {
        double goal[2] = {robot.mWorld.our_goal[0][0], ((robot.mWorld.our_goal[1][0] + robot.mWorld.our_goal[1][1])/2)};
        skills::move_to(robot, goal);
    }
} // roles