//
// Created by caiu on 08/08/25.
//

#include "../RobotController.h"
#include "roles.h"

namespace roles {
    void kickoff_support(RobotController& robot) {
        double goal[2] = {robot.mTeam->our_side_sign*robot.mRadius , 1000};
        skills::move_to(robot, goal);
    }
}
