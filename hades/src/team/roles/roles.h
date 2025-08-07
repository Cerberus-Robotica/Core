//
// Created by caiu on 06/08/25.
//

#ifndef ROLES_H
#define ROLES_H

class RobotController;

#include <cmath>

namespace roles {
    void goal_keeper(RobotController& robot);
    void mid_field(RobotController& robot);
    void striker(RobotController& robot);
    void defender(RobotController& robot);
    void halted(RobotController& robot);
}

#endif //ROLES_H
