//
// Created by caiu on 06/08/25.
//

#include "../RobotController.h"
#include "tactics.h"

namespace tactics {
    void follow_trajectory(RobotController& robot, std::vector<std::vector<double>>& trajectory) {

        int i = 0;
        while (size(trajectory) > 0) {
            double distance = sqrt(pow(trajectory[0][0] - robot.mpos[0], 2) + pow(trajectory[0][1] - robot.mpos[1], 2));
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
            robot.mtarget_vel[0] = 0;
            robot.mtarget_vel[1] = 0;
            robot.mPositioned = true;
            robot.mTeam->positioned[robot.mId] = true;
            return;
        }

        double next_point[2] = {trajectory[0][0], trajectory[0][1]};
        skills::move_to(robot, next_point);

        robot.mPositioned = false;
        robot.mTeam->positioned[robot.mId] = false;
    }
}