//
// Created by caiu on 18/08/25.
//

#include "RoleKickOffKicker.h"
#include "../RobotController.h"

namespace roles {
    void RoleKickOffKicker::act(RobotController& robot) {
        std::vector<double> their_goal = {(robot.mWorld.their_goal[0][1] + robot.mWorld.their_goal[0][0])/2, (robot.mWorld.their_goal[1][1] + robot.mWorld.their_goal[1][0])/2};
        std::vector<double> center = {0, 0};
        auto kickposition = robot.mWorld.getKickingPosition(center, their_goal, robot.mBall_avoidance_radius + robot.mRadius);
        double goal[2] = {kickposition[0], kickposition[1]};
        skills::move_to(robot, goal);
    }
} // roles