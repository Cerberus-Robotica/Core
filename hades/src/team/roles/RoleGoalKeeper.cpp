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
            LineSegment line(Point(robot.mWorld.our_goal[0][0], robot.mWorld.our_goal[1][0]), {robot.mWorld.our_goal[0][0], robot.mWorld.our_goal[1][1]});
            tactics::keep_x_line(robot, line, (robot.mWorld.our_goal[1][0] + robot.mWorld.our_goal[1][1])/2);
        }
        else {
            Point goal = {(robot.mWorld.their_goal[0][1] + robot.mWorld.their_goal[0][0])/2, (robot.mWorld.their_goal[1][1] + robot.mWorld.their_goal[1][0])/2};
            tactics::position_and_kick_to_destination(robot, goal);
        }
    }
} // roles