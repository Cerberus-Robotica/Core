//
// Created by caiu on 06/08/25.
//

#include "../RobotController.h"
#include "../../c_trajectory/C_trajectory.h"
#include "tactics.h"

namespace tactics {
    void keep_a_location(RobotController& robot, double keep[2]) {
        if (robot.mWorld.ball_speed_module == 0 || !robot.mWorld.isBallMovingIdDirection(robot.mId)) {
            skills::move_to(robot, keep);
        }
        else {
            if (distance_point(robot.mpos, robot.mWorld.ball_pos) > robot.mRadius*5) skills::move_to(robot, robot.mWorld.ball_stop_position);
            else skills::move_to(robot, robot.mWorld.ball_pos);
        }
    }
}