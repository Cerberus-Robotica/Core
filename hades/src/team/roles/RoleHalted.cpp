//
// Created by caiu on 18/08/25.
//

#include "RoleHalted.h"
#include "../RobotController.h"

namespace roles {
    void RoleHalted::act(RobotController& robot) {
        stop.act(robot);
    }
} // rolesSkillKick