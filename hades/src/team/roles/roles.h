//
// Created by caiu on 06/08/25.
//

#ifndef ROLES_H
#define ROLES_H

class RobotController;

#include <cmath>

namespace roles {
    void move_to(RobotController& robot, double goal[2], bool avoid_ball = true);
    void turn_to(RobotController& robot, double goal[2]);
    void kick(RobotController& robot);
}

#endif //ROLES_H
