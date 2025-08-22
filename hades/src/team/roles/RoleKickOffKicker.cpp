//
// Created by caiu on 18/08/25.
//

#include "RoleKickOffKicker.h"
#include "../RobotController.h"

namespace roles {
    void RoleKickOffKicker::act(RobotController& robot) {
        Point their_goal = {(robot.mWorld.their_goal[0][1] + robot.mWorld.their_goal[0][0])/2, (robot.mWorld.their_goal[1][1] + robot.mWorld.their_goal[1][0])/2};
        Point center = {0, 0};
        auto kickposition = robot.mWorld.getKickingPosition(center, their_goal, robot.mBall_avoidance_radius + robot.mRadius);
        skills::move_to(robot, kickposition);
    }
} // roles