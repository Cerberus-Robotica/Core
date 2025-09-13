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
        Point goal(0, 0);
        try {
            goal = robot.mWorld.getGoalPosition(robot.mTeam->getEnemyofRole(Robot::goal_keeper, robot.mWorld.enemies));
        } catch (...) {
            goal = robot.mWorld.field.theirGoal.getMiddle();
        }
        LineSegment robot_goal = {robot.mWorld.ball.getPosition(), goal};
        if (robot.mWorld.ball.isMoving() && robot.mWorld.ball.getMovementLine().isPointAligned(robot.getPosition(), 3.1415/8)) {
            intercept.act(robot);
        }
        else if (robot.mWorld.ball.isMoving() || robot.mWorld.isPointOnOurArea(robot.mWorld.ball.getPosition())) {
            stop.act(robot); //TODO implementar posicao de suporte
        }
        else if (!robot.mWorld.doInterceptAnyRobot(robot_goal) && robot_goal.getLength() <= robot.getKickDistance()) {
            positionAndKick.act(robot, goal); //TODO encontrar posicao do golasso
        } else {
            try {
                Robot support = robot.mTeam->getRobotofRole(Robot::support);
                positionAndKick.act(robot, support);
            } catch (...) {
                positionAndKick.act(robot, goal);
            }
        }
    }
} // roles