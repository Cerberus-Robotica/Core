//
// Created by caiu on 05/08/25.
//

#ifndef SKILLS_H
#define SKILLS_H

class RobotController;
class Point;

#include <cmath>

namespace skills {
    void move_to(RobotController& robot, Point goal, bool avoid_ball = true);
    void turn_to(RobotController& robot, Point goal);
    void kick(RobotController& robot);
    void stop(RobotController& robot);
}

#endif //SKILLS_H