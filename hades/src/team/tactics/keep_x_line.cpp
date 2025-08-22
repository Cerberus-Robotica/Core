//
// Created by caiu on 06/08/25.
//

#include "../RobotController.h"
#include "../../c_trajectory/C_trajectory.h"
#include "tactics.h"
#include "../TeamInfo.h"

namespace tactics {
    void keep_x_line(RobotController& robot, LineSegment y_segment, double y_rest) {
        std::vector<int> enemies_ids = {};
        int enemy_stricker_id = 0;



        for (int i = 0; i < enemies_ids.size(); i++) {
            if (robot.mTeam->enemy_roles[i] == Robot::striker) {
                enemy_stricker_id = i;
                break;
            }
        }

        double a = 1000;
        if (size(enemies_ids) != 0) {
            if (robot.mWorld.ball.getVelocity().getNorm() > robot.mVxy_min) {
                if (robot.mWorld.ball.getVelocity().getX() != 0) {
                    a = (robot.mWorld.ball.getVelocity().getY()) / (robot.mWorld.ball.getVelocity().getX());
                }
            }
            else if (robot.mWorld.ball.getPosition().getX() - robot.mWorld.enemies[enemy_stricker_id].getPosition().getX() != 0) {    //para bola parada
                a = (robot.mWorld.ball.getPosition().getY() - robot.mWorld.enemies[enemy_stricker_id].getPosition().getY()) / (robot.mWorld.ball.getPosition().getX() - robot.mWorld.enemies[enemy_stricker_id].getPosition().getX());
            }
        }

        double y_meet = a*(y_segment.getStart().getX()) + robot.mWorld.ball.getPosition().getY() -a*robot.mWorld.ball.getPosition().getX();
        double y_max = y_segment.getStart().getY() - robot.mRadius;
        double y_min = y_segment.getEnd().getY() + robot.mRadius;
        y_meet = std::clamp(y_meet, y_min, y_max);
        double x_meet = y_segment.getStart().getX() - robot.mRadius*y_segment.getStart().getX()/fabs(y_segment.getStart().getX());
        Point meet = {x_meet, y_meet};

        if (robot.mWorld.enemies.size() > 0) {
            if (!robot.mWorld.isBallOnOurSide() || (robot.mWorld.enemies[enemy_stricker_id].getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) > 500 && robot.mWorld.ball.getVelocity().getNorm() == 0)) {
                meet.setY(y_rest);
            }
        }


        skills::move_to(robot, meet, false);
        robot.mkicker_x = 0;
        skills::turn_to(robot, robot.mWorld.ball.getPosition());
    }
}