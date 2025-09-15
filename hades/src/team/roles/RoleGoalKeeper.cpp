//
// Created by caiu on 18/08/25.
//

#include "RoleGoalKeeper.h"

#include <iostream>
#include <ostream>

#include "../RobotController.h"

namespace roles {
    void RoleGoalKeeper::act(RobotController& robot) {
        //TODO melhorar goal_keeper_role
        if (!robot.mWorld.isPointOnOurArea(robot.mWorld.ball.getPosition()) || robot.mWorld.ball.isMoving()) {
            LineSegment line = robot.mWorld.field.ourGoal;
            if (line.getStart().getX() < 0) line = line.getMovedOnX(robot.getRadius());
            if (line.getStart().getX() > 0) line = line.getMovedOnX(-robot.getRadius());

            keepXLine.act(robot, line, (robot.mWorld.field.ourGoal.getStart().getY() + robot.mWorld.field.ourGoal.getEnd().getY())/2);
        } else {
            Point goal = {(robot.mWorld.field.theirGoal.getStart().getX() + robot.mWorld.field.theirGoal.getEnd().getX())/2, (robot.mWorld.field.theirGoal.getStart().getY() + robot.mWorld.field.theirGoal.getEnd().getY())/2};
            positionAndKick.act(robot, goal);
        }
    }
} // roles