//
// Created by caiu on 06/08/25.
//

#include "../RobotController.h"
#include "tactics.h"
#include "../TeamInfo.h"

namespace tactics {
    void follow_trajectory(RobotController& robot, std::vector<Point>& trajectory) {

        int i = 0;
        while (size(trajectory) > 0) {
            double distance = robot.getPosition().getDistanceTo(trajectory[0]);
            if (size(trajectory) == 1 && distance < robot.mStatic_position_tolarance) {
                trajectory.erase(trajectory.begin());
                break;
            }

            if (distance < robot.mDynamic_position_tolarance) {
                trajectory.erase(trajectory.begin());
                continue;
            }
            break;
        }
        if (size(trajectory) == 0) {
            robot.mtarget_vel = {0, 0};
            robot.mPositioned = true;
            robot.mTeam->positioned[robot.getId()] = true;
            return;
        }

        skills::move_to(robot, trajectory[0]);

        robot.mPositioned = false;
        robot.mTeam->positioned[robot.getId()] = false;
    }
}