//
// Created by caiu on 06/08/25.
//

#include "../RobotController.h"
#include "../../c_trajectory/C_trajectory.h"
#include "tactics.h"

namespace tactics {
    void keep_a_location(RobotController& robot, Point keep) {
        if (robot.mWorld.ball.getVelocity().getNorm() == 0 || !robot.mWorld.isBallMovingIdDirection(robot.getId())) {
            skills::move_to(robot, keep);
        }
        else {
            if (robot.getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) > robot.mRadius*5) skills::move_to(robot, robot.mWorld.ball.findStopPosition());
            else skills::move_to(robot, robot.mWorld.ball.getPosition());
        }
    }
}