//
// Created by caiu on 06/08/25.
//

#include "skills.h"
#include "../RobotController.h"

namespace skills {
    void kick(RobotController& robot) {
        if (robot.mWorld.ball_speed_module > robot.mVxy_min) {
            robot.mkicker_x = 0;
            robot.mState += 1;
        }

        std::vector<double> v_vet = {robot.mWorld.ball_pos[0] - robot.mpos[0], robot.mWorld.ball_pos[1] - robot.mpos[1]};
        v_vet = normalize(robot.mVxy_min,v_vet);
        robot.mtarget_vel[0] = v_vet[0]*cos(-robot.myaw) - v_vet[1]*sin(-robot.myaw);
        robot.mtarget_vel[1] = v_vet[0]*sin(-robot.myaw) + v_vet[1]*cos(-robot.myaw);
        robot.mkicker_x = 3;
        skills::turn_to(robot, robot.mWorld.ball_pos);
    }
}
