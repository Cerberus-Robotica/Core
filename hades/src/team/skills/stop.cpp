//
// Created by caiu on 07/08/25.
//

#include <cmath>
#include "../RobotController.h"
#include "skills.h"

namespace skills {
    void stop(RobotController& robot) {
        robot.mtarget_vel[0] = 0;
        robot.mtarget_vel[1] = 0;
        robot.mtarget_vyaw = 0;
    }
}