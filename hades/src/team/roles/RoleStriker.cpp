//
// Created by caiu on 18/08/25.
//

#include "RoleStriker.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"
#include "../geometry/Point.h"

namespace roles {
    void RoleStriker::act(RobotController& robot) {
        LineSegment robot_goal = {robot.mWorld.ball.getPosition(), robot.mWorld.field.theirGoal.getMiddle()};
        if (robot.mWorld.ball.isMoving() && robot.mWorld.ball.getMovementLine().isPointAligned(robot.getPosition(), 3.1415/8)) {
            intercept.act(robot);
        }
        else if (robot.mWorld.ball.isMoving() || robot.mWorld.isPointOnOurArea(robot.mWorld.ball.getPosition())) {
            stop.act(robot); //TODO implementar posicao de suporte
        }
        else if (!robot.mWorld.doInterceptAnyRobot(robot_goal) && robot_goal.getLength() <= robot.mKick_distance) {
            positionAndKick.act(robot, robot.mWorld.field.theirGoal.getMiddle()); //TODO encontrar posicao do golasso
        } else {
            try {
                Robot support = robot.mTeam->getRobotofRole(Robot::support);
                positionAndKick.act(robot, support);
            } catch (...) {
                positionAndKick.act(robot, robot.mWorld.field.theirGoal.getMiddle());
            }
        }
    }
} // roles