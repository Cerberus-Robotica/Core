//
// Created by caiu on 26/08/25.
//

#include "SkillCushion.h"
#include "../geometry/Vector2d.h"
#include "../RobotController.h"

namespace skills {
void SkillCushion::act(RobotController& robot) {
    Vector2d vector = robot.mWorld.ball.getVelocity().getNormalized(robot.mVxy_min);
    robot.mtarget_vel = vector.getRotated(-robot.getYaw());
}
} // skills