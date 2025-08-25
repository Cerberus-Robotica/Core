//
// Created by caiu on 18/08/25.
//

#include "RoleKickOffKicker.h"
#include "../RobotController.h"

namespace roles {
    void RoleKickOffKicker::act(RobotController& robot) {
        Point their_goal = {(robot.mWorld.field.theirGoal.getStart().getX() + robot.mWorld.field.theirGoal.getEnd().getX())/2, (robot.mWorld.field.theirGoal.getStart().getY() + robot.mWorld.field.theirGoal.getEnd().getY())/2};
        Point center = {0, 0};
        auto kickposition = robot.mWorld.getKickingPosition(center, their_goal, robot.mBall_avoidance_radius + robot.mRadius);
        moveTo.act(robot, kickposition, true);
    }
} // roles