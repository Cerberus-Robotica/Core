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
        std::cout << "00" << std::endl;
        if (robot.mWorld.isBallMovingRobotDirection(robot) && robot.mWorld.ball.isMoving()) {
            intercept.act(robot);
            std::cout << "1" << std::endl;
        } else if (robot.mWorld.getClosestAllyToPoint(robot.mWorld.ball.getPosition()).getId() == robot.getId()) {
            std::cout << "2" << std::endl;
            try {
                positionAndKick.act(robot, robot.mTeam->getRobotofRole(Robot::support));
            } catch (...) {
                positionAndKick.act(robot, robot.mWorld.field.theirGoal.getMiddle());
            }
        } else {
            LineSegment line = {Point(0, 0), Point(0, 0)};
            if (robot.mTeam->our_side == TeamInfo::left) line = {robot.mWorld.field.ourDefenseArea.getMajorPoint(), Point(robot.mWorld.field.ourDefenseArea.getMajorPoint().getX(), robot.mWorld.field.ourDefenseArea.getMinorPoint().getY())};
            else line = {robot.mWorld.field.ourDefenseArea.getMinorPoint(), Point(robot.mWorld.field.ourDefenseArea.getMinorPoint().getX(), robot.mWorld.field.ourDefenseArea.getMajorPoint().getY())};
            std::cout << "line: " << line.getStart().getX() << "," << line.getStart().getY() << std::endl;
            std::cout << "lineend: " << line.getEnd().getX() << "," << line.getEnd().getY() << std::endl;
            keepXLine.act(robot, line, line.getMiddle().getY());
        }
    }
} // roles