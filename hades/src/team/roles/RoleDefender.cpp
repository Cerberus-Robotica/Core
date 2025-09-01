//
// Created by caiu on 18/08/25.
//

#include "RoleDefender.h"
#include "../RobotController.h"
#include <iostream>
#include <ostream>

#include "../TeamInfo.h"

namespace roles {
    void RoleDefender::act(RobotController& robot) {
        if (robot.mWorld.isBallMovingRobotDirection(robot)) {
            intercept.act(robot);
        } else if (robot.mWorld.getClosestAllyToPoint(robot.mWorld.ball.getPosition()).getId() == robot.getId()) {
            try {
                positionAndKick.act(robot, robot.mTeam->getRobotofRole(Robot::support));
            } catch (...) {
                positionAndKick.act(robot, robot.mWorld.field.theirGoal.getMiddle());
            }
        } else {
            LineSegment line = {Point(0, 0), Point(0, 0)};
            if (robot.mTeam->our_side == TeamInfo::left) line = {robot.mWorld.field.ourDefenseArea.getMajorPoint(), Point(robot.mWorld.field.ourDefenseArea.getMajorPoint().getX(), robot.mWorld.field.ourDefenseArea.getMinorPoint().getY())};
            else line = {robot.mWorld.field.ourDefenseArea.getMinorPoint(), Point(robot.mWorld.field.ourDefenseArea.getMinorPoint().getX(), robot.mWorld.field.ourDefenseArea.getMajorPoint().getY())};
            keepXLine.act(robot, line, line.getMiddle().getY());
        }
    }
} // roles