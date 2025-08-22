//
// Created by caiu on 18/08/25.
//

#include "RoleKickOffGoalKeeper.h"
#include "../RobotController.h"

namespace roles {
    void RoleKickOffGoalKeeper::act(RobotController& robot) {
        Point goal = {(robot.mWorld.field.ourGoal.getStart().getX() + robot.mWorld.field.ourGoal.getEnd().getX())/2, (robot.mWorld.field.ourGoal.getStart().getY() + robot.mWorld.field.ourGoal.getEnd().getY())/2};
            skills::move_to(robot, goal);
    }
} // roles