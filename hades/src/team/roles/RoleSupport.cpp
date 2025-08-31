//
// Created by caiu on 18/08/25.
//

#include "RoleSupport.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"

namespace roles {
    Point RoleSupport::getSupportPosition(RobotController robot) {
        try {
            AreaCircular a1 = {robot.mWorld.field.theirGoal.getMiddle(), robot.mKick_distance};
            AreaCircular a2 = {robot.mWorld.ball.getPosition(), robot.mKick_distance};
            try {
                return a1.getInterceptionPoints(a2)[0];
            } catch (...) {
                return a2.getInterceptionPoints(LineSegment(robot.mWorld.field.theirGoal.getMiddle(), robot.mWorld.ball.getPosition()))[0];
            }

        } catch (...) {
            return {0, 0};
        }
    }
    void RoleSupport::act(RobotController& robot) {
        //TODO continuar
        if (robot.mWorld.ball.isMoving() && robot.mWorld.isBallMovingRobotDirection(robot)) {
            intercept.act(robot);
        } else {
            keepLocation.act(robot, getSupportPosition(robot));
        }
    }
} // roles