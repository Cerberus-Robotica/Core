//
// Created by caiu on 06/08/25.
//

#include "../RobotController.h"
#include "../../c_trajectory/C_trajectory.h"
#include "tactics.h"
#include <iostream>

namespace tactics {
    namespace aux {
        std::vector<std::vector<double>> find_ball_trajectory(RobotController& robot, double start[2], double goal[2]) {
            C_trajectory pf(false, false, 0, 1000, 300, 0, robot.mWorld.boundariesMinor, robot.mWorld.boundariesMajor);
            std::vector<double> double_start = {start[0], start[1]};
            std::vector<double> double_goal = {goal[0], goal[1]};
            std::vector<Circle> obs_circular = {};
            std::vector<Rectangle> obs_rectangular = {};

            //add static enemies to obstacles
            for (int i = 0; i < size(robot.mWorld.enemies) ; i++) {
                if (!robot.mWorld.enemies[i].detected) {
                    continue;
                }
                Circle c({robot.mWorld.enemies[i].pos[0], robot.mWorld.enemies[i].pos[1]}, robot.mRadius);
                obs_circular.push_back(c);
            }

            auto trajectory = pf.path_find(double_start, double_goal, obs_circular, obs_rectangular);
            return trajectory;
        }

    }

    void position_and_kick_to_destination(RobotController& robot, double goal[2]) {
        auto traj = aux::find_ball_trajectory(robot, robot.mWorld.ball_pos, goal);
        std::vector<double> kick_pos = robot.mWorld.getKickingPosition(traj[0], traj[1], robot.mBall_avoidance_radius + robot.mRadius);
        double next_point[2] = {traj[1][0], traj[1][1]};

        if (robot.mState == 0) {
            double db_kick_pos[2] = {kick_pos[0], kick_pos[1]};
            skills::move_to(robot, db_kick_pos);
            skills::turn_to(robot, next_point);
            if (robot.mPositioned and robot.mOriented) {
                robot.mState = 1;
            }
        }
        else if (robot.mState == 1) {
            skills::kick(robot);
        }
        else if (robot.mState == 2) {
            robot.mState = 0;
        }
    }

    void position_and_kick_to_robot(RobotController& robot, int id) {
        double goal[2] = {robot.mWorld.allies[id].pos[0], robot.mWorld.allies[id].pos[1]};
        auto traj = aux::find_ball_trajectory(robot, robot.mWorld.ball_pos, goal);
        std::vector<double> kick_pos = robot.mWorld.getKickingPosition(traj[0], traj[1], robot.mBall_avoidance_radius + robot.mRadius);
        double next_point[2] = {traj[1][0], traj[1][1]};

        if (robot.mState == 0) {
            double db_kick_pos[2] = {kick_pos[0], kick_pos[1]};
            skills::move_to(robot, db_kick_pos);
            skills::turn_to(robot, next_point);
            if (robot.mPositioned and robot.mOriented and robot.mTeam->positioned[id]) {
                robot.mState = 1;
            }
        }
        else if (robot.mState == 1) {
            skills::kick(robot);
            robot.mTimer = 3;
        }
        else if (robot.mState == 2) {
            skills::move_to(robot, robot.mpos);
            skills::turn_to(robot, robot.mWorld.ball_pos);
            robot.mTimer -= robot.mDelta_time;
            if (robot.mTimer <= 0) {
                robot.mTimer = 0;
                robot.mState = 0;
            }
        }
    }
}