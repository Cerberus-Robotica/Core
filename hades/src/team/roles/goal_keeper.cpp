//
// Created by caiu on 06/08/25.
//

#include "../RobotController.h"
#include "roles.h"

namespace roles {

    void goal_keeper(RobotController& robot) {
        //TODO melhorar goal_keeper_role
        if (!robot.mWorld.isBallOnOurArea() || robot.mWorld.ball_speed_module != 0) {
            robot.mState = 0;
            tactics::keep_x_line(robot, robot.mWorld.our_goal[0][0], robot.mWorld.our_goal[1], (robot.mWorld.our_goal[1][0] + robot.mWorld.our_goal[1][1])/2);
        }
        else {
            double goal[2] = {(robot.mWorld.their_goal[0][1] + robot.mWorld.their_goal[0][0])/2, (robot.mWorld.their_goal[1][1] + robot.mWorld.their_goal[1][0])/2};
            tactics::position_and_kick_to_destination(robot, goal);
        }
    }
}