//
// Created by caiu on 08/08/25.
//

#include "../RobotController.h"
#include "roles.h"

namespace roles {
    void kickoff_goal_keeper(RobotController& robot) {
        double goal[2] = {robot.mWorld.our_goal[0][0], ((robot.mWorld.our_goal[1][0] + robot.mWorld.our_goal[1][1])/2)};
        skills::move_to(robot, goal);
    }
}
