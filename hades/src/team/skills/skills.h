//
// Created by caiu on 05/08/25.
//

#ifndef SKILLS_H
#define SKILLS_H

class RobotController;

#include <cmath>

namespace skills {
    void move_to(RobotController& robot, double goal[2], bool avoid_ball = true);
    void turn_to(RobotController& robot, double goal[2]);
    void kick(RobotController& robot);
}

#endif //SKILLS_H