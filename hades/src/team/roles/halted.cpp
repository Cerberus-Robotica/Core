//
// Created by caiu on 07/08/25.
//

#include "../RobotController.h"
#include "roles.h"

namespace roles {
    void halted(RobotController& robot) {
            skills::stop(robot);
        }
}
