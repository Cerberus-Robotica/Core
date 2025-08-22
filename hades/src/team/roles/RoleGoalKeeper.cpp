//
// Created by caiu on 18/08/25.
//

#include "RoleGoalKeeper.h"
#include "../RobotController.h"

namespace roles {
    void RoleGoalKeeper::act(RobotController& robot) {
        //TODO melhorar goal_keeper_role
        if (!robot.mWorld.isBallOnOurArea() || robot.mWorld.ball.getVelocity().getNorm() != 0) {
            robot.mState = 0;
            LineSegment line = robot.mWorld.field.ourGoal;
            tactics::keep_x_line(robot, line, (robot.mWorld.field.ourGoal.getStart().getY() + robot.mWorld.field.ourGoal.getEnd().getY())/2);
        }
        else {
            Point goal = {(robot.mWorld.field.theirGoal.getStart().getX() + robot.mWorld.field.theirGoal.getEnd().getX())/2, (robot.mWorld.field.theirGoal.getStart().getY() + robot.mWorld.field.theirGoal.getEnd().getY())/2};
            tactics::position_and_kick_to_destination(robot, goal);
        }
    }
} // roles