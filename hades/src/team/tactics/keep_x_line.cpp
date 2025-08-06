//
// Created by caiu on 06/08/25.
//

#include "../RobotController.h"
#include "../../c_trajectory/C_trajectory.h"
#include "tactics.h"

namespace tactics {
    void keep_x_line(RobotController& robot, double x_line, const double y_segment[2], double y_rest) {
        std::vector<int> enemies_ids = {};
        int enemy_stricker_id = 0;



        for (int i = 0; i < enemies_ids.size(); i++) {
            if (robot.mTeam->enemy_roles[i] == TeamInfo::stricker) {
                enemy_stricker_id = i;
                break;
            }
        }

        double a = 1000;
        if (size(enemies_ids) != 0) {
            if (sqrt(pow(robot.mWorld.ball_speed[0], 2) + pow(robot.mWorld.ball_speed[1], 2)) > robot.mVxy_min) {
                if (robot.mWorld.ball_speed[0] != 0) {
                    a = (robot.mWorld.ball_speed[1]) / (robot.mWorld.ball_speed[0]);
                }
            }
            else if (robot.mWorld.ball_pos[0] - robot.mWorld.enemies[enemy_stricker_id].pos[0] != 0) {    //para bola parada
                a = (robot.mWorld.ball_pos[1] - robot.mWorld.enemies[enemy_stricker_id].pos[1]) / (robot.mWorld.ball_pos[0] - robot.mWorld.enemies[enemy_stricker_id].pos[0]);
            }
        }

        double y_meet = a*(x_line) + robot.mWorld.ball_pos[1] -a*robot.mWorld.ball_pos[0];
        double y_max = y_segment[0] - robot.mRadius;
        double y_min = y_segment[1] + robot.mRadius;
        y_meet = std::clamp(y_meet, y_min, y_max);
        double x_meet = x_line - robot.mRadius*x_line/fabs(x_line);
        double meet[2] = {x_meet, y_meet};

        if (!robot.mWorld.isBallOnOurSide() || (distance_point(robot.mWorld.enemies[enemy_stricker_id].pos, robot.mWorld.ball_pos) > 500 && robot.mWorld.ball_speed_module == 0)) {
            meet[1] = y_rest;
        }


        skills::move_to(robot, meet, false);
        robot.mkicker_x = 0;
        skills::turn_to(robot, robot.mWorld.ball_pos);
    }
}