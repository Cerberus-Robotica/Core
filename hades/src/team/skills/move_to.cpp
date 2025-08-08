//
// Created by caiu on 05/08/25.
//

#include <cmath>
#include <iostream>
#include <vector>
#include <bits/stl_algo.h>
#include "../RobotController.h"
#include "../c_trajectory/C_trajectory.h"
#include "skills.h"

namespace skills {
    namespace {
        std::vector<double> motion_planner(RobotController& robot, std::vector<std::vector<double>> trajectory) {
            std::vector<double> delta = {trajectory[1][0] - robot.mpos[0], trajectory[1][1] - robot.mpos[1]};
            double dist = norm(delta) / 1000.0; // metros
            std::vector<double> direction = normalize(1, delta);

            double curve_safe_speed = robot.mVxy_min;
            double v_target_magnitude = robot.mVxy_max;
            if (trajectory.size() > 2) {
                std::vector<double> p0 = {robot.mpos[0], robot.mpos[1]};
                std::vector<double> p1 = {trajectory[1][0], trajectory[1][1]};
                std::vector<double> p2 = {trajectory[2][0], trajectory[2][1]};

                std::vector<double> v1 = {p1[0] - p0[0], p1[1] - p0[1]};
                std::vector<double> v2 = {p2[0] - p1[0], p2[1] - p1[1]};

                double dot_product = v1[0] * v2[0] + v1[1] * v2[1];
                double norm_v1 = sqrt(v1[0]*v1[0] + v1[1]*v1[1]);
                double norm_v2 = sqrt(v2[0]*v2[0] + v2[1]*v2[1]);
                double angle_between = acos(std::clamp(dot_product / (norm_v1 * norm_v2 + 1e-6), -1.0, 1.0));

                double chord_length = sqrt(pow(p2[0] - p0[0], 2) + pow(p2[1] - p0[1], 2)); // mm
                chord_length /= 1000.0; // Para metros

                double radius = chord_length / (2.0 * sin(std::max(angle_between/2, 1e-3)));
                radius = std::max(radius, 0.05);
                curve_safe_speed = sqrt(robot.mA_xy_max * radius)/4;
                curve_safe_speed = std::clamp(curve_safe_speed, robot.mVxy_min, robot.mVxy_max);

                if (angle_between > 70.0 * M_PI / 180.0) {
                    curve_safe_speed = robot.mVxy_min;
                }

            }

            double current_speed = sqrt(robot.mvel[0]*robot.mvel[0] + robot.mvel[1]*robot.mvel[1]);

            double brake_distance = (robot.mVxy_max*robot.mVxy_max - curve_safe_speed * curve_safe_speed) / (2.0 * robot.mA_xy_brake);
            brake_distance = std::max(brake_distance, 0.0);
            if (dist <= brake_distance) {
                v_target_magnitude = curve_safe_speed;
            } else {
                v_target_magnitude = robot.mVxy_max;
            }

            v_target_magnitude = std::clamp(v_target_magnitude, -robot.mVxy_max, robot.mVxy_max);
            std::vector<double> v_target = {v_target_magnitude * direction[0], v_target_magnitude * direction[1]};
            std::vector<double> error = {v_target[0] - robot.mlast_target_vel[0], v_target[1] - robot.mlast_target_vel[1]};
            std::vector<double> acceleration = {0, 0};

            if (fabs(v_target[0]) > fabs(robot.mlast_target_vel[0])) {
                acceleration[0] = std::clamp(error[0] / robot.mDelta_time, -robot.mA_xy_max, robot.mA_xy_max);
            } else {
                acceleration[0] = std::clamp(error[0] / robot.mDelta_time, -robot.mA_xy_brake, robot.mA_xy_brake);
            }

            if (fabs(v_target[1]) > fabs(robot.mlast_target_vel[1])) {
                acceleration[1] = std::clamp(error[1] / robot.mDelta_time, -robot.mA_xy_max, robot.mA_xy_max);
            } else {
                acceleration[1] = std::clamp(error[1] / robot.mDelta_time, -robot.mA_xy_brake, robot.mA_xy_brake);
            }


            std::vector<double> vel_cmd = {robot.mlast_target_vel[0] + acceleration[0]*robot.mDelta_time, robot.mlast_target_vel[1] + acceleration[1]*robot.mDelta_time};
            if (norm(vel_cmd) > robot.mVxy_max) {
                vel_cmd = normalize(robot.mVxy_max, vel_cmd);
            }

            if (std::isnan(vel_cmd[0])) vel_cmd[0] = 0;
            if (std::isnan(vel_cmd[1])) vel_cmd[1] = 0;

            robot.mlast_target_vel[0] = vel_cmd[0];
            robot.mlast_target_vel[1] = vel_cmd[1];
            return vel_cmd;
        }

        std::vector<double> motion_control(std::vector<double> v_vet, double yaw) {
            const double ang = yaw;
            double x = v_vet[0];
            double y = v_vet[1];
            v_vet = {x * cos(ang) - y * sin(ang), x * sin(ang) + y * cos(ang)};
            return v_vet;
        }


        std::vector<std::vector<double>> find_trajectory( RobotController& robot, double start[2], double goal[2], bool avoid_ball = true) {
            C_trajectory pf(false, false, 0, 100, 50, 0, robot.mWorld.boundariesMinor, robot.mWorld.boundariesMajor);
            std::vector<double> double_start = {start[0], start[1]};
            std::vector<double> double_goal = {goal[0], goal[1]};

            std::vector<Circle> obs_circular = {};
            std::vector<Rectangle> obs_rectangular = {};
            Rectangle r({0, 0}, {0, 0});
            //rectangle r = field.their_defense_area;
            //obs_rectangular.push_back(r);

            //add static ball to obstacles according to avoidance radius
            if (avoid_ball) {
                Circle c({robot.mWorld.ball_pos[0], robot.mWorld.ball_pos[1]}, robot.mBall_avoidance_radius + robot.mRadius);
                obs_circular.push_back(c);
            }

            //add static allies to obstacles
            for (int i = 0; i < size(robot.mWorld.allies) ; i++) {
                if (!robot.mWorld.allies[i].detected || i == robot.mId) {
                    continue;
                }
                Circle c({robot.mWorld.allies[i].pos[0], robot.mWorld.allies[i].pos[1]}, robot.mRadius);
                obs_circular.push_back(c);
            }

            //add static enemies to obstacles
            for (int i = 0; i < size(robot.mWorld.enemies) ; i++) {
                if (!robot.mWorld.enemies[i].detected) {
                    continue;
                }
                Circle c({robot.mWorld.enemies[i].pos[0], robot.mWorld.enemies[i].pos[1]}, robot.mRadius);
                obs_circular.push_back(c);
            }

            if (robot.mTeam->roles[robot.mId] != TeamInfo::goal_keeper) {
                Rectangle r({robot.mWorld.our_defese_area[0][0] - robot.mRadius, robot.mWorld.our_defese_area[0][1] - robot.mRadius}, {robot.mWorld.our_defese_area[1][0] + robot.mRadius, robot.mWorld.our_defese_area[1][1] + robot.mRadius});
                obs_rectangular.push_back(r);
            }

            r.minor = {robot.mWorld.their_defese_area[0][0] - robot.mRadius, robot.mWorld.their_defese_area[0][1] - robot.mRadius};
            r.major = {robot.mWorld.their_defese_area[1][0] + robot.mRadius, robot.mWorld.their_defese_area[1][1] + robot.mRadius};
            obs_rectangular.push_back(r);


            //goal fisical barrier

            r.minor = {robot.mWorld.back_fisical_left_goal[0][0] - robot.mRadius, robot.mWorld.back_fisical_left_goal[0][1] - robot.mRadius};
            r.major = {robot.mWorld.back_fisical_left_goal[1][0] - robot.mRadius, robot.mWorld.back_fisical_left_goal[1][1] + robot.mRadius};
            obs_rectangular.push_back(r);
            r.minor = {robot.mWorld.left_fisical_left_goal[0][0] - robot.mRadius, robot.mWorld.left_fisical_left_goal[0][1] - robot.mRadius};
            r.major = {robot.mWorld.left_fisical_left_goal[1][0] - robot.mRadius, robot.mWorld.left_fisical_left_goal[1][1] - robot.mRadius};
            obs_rectangular.push_back(r);
            r.minor = {robot.mWorld.right_fisical_left_goal[0][0] - robot.mRadius, robot.mWorld.right_fisical_left_goal[0][1] + robot.mRadius};
            r.major = {robot.mWorld.right_fisical_left_goal[1][0] - robot.mRadius, robot.mWorld.right_fisical_left_goal[1][1] + robot.mRadius};
            obs_rectangular.push_back(r);


            r.minor = {robot.mWorld.back_fisical_right_goal[0][0] + robot.mRadius, robot.mWorld.back_fisical_right_goal[0][1] - robot.mRadius};
            r.major = {robot.mWorld.back_fisical_right_goal[1][0] + robot.mRadius, robot.mWorld.back_fisical_right_goal[1][1] + robot.mRadius};
            obs_rectangular.push_back(r);
            r.minor = {robot.mWorld.left_fisical_right_goal[0][0] + robot.mRadius, robot.mWorld.left_fisical_right_goal[0][1] - robot.mRadius};
            r.major = {robot.mWorld.left_fisical_right_goal[1][0] + robot.mRadius, robot.mWorld.left_fisical_right_goal[1][1] - robot.mRadius};
            obs_rectangular.push_back(r);
            r.minor = {robot.mWorld.right_fisical_right_goal[0][0] + robot.mRadius, robot.mWorld.right_fisical_right_goal[0][1] + robot.mRadius};
            r.major = {robot.mWorld.right_fisical_right_goal[1][0] + robot.mRadius, robot.mWorld.right_fisical_right_goal[1][1] + robot.mRadius};
            obs_rectangular.push_back(r);

            r.minor = {robot.mWorld.outside_field_x_minus[0][0], robot.mWorld.outside_field_x_minus[0][1]};
            r.major = {robot.mWorld.outside_field_x_minus[1][0], robot.mWorld.outside_field_x_minus[1][1]};
            obs_rectangular.push_back(r);
            r.minor = {robot.mWorld.outside_field_x_plus[0][0], robot.mWorld.outside_field_x_plus[0][1]};
            r.major = {robot.mWorld.outside_field_x_plus[1][0], robot.mWorld.outside_field_x_plus[1][1]};
            obs_rectangular.push_back(r);
            r.minor = {robot.mWorld.outside_field_y_minus[0][0], robot.mWorld.outside_field_y_minus[0][1]};
            r.major = {robot.mWorld.outside_field_y_minus[1][0], robot.mWorld.outside_field_y_minus[1][1]};
            obs_rectangular.push_back(r);
            r.minor = {robot.mWorld.outside_field_y_plus[0][0], robot.mWorld.outside_field_y_plus[0][1]};
            r.major = {robot.mWorld.outside_field_y_plus[1][0], robot.mWorld.outside_field_y_plus[1][1]};
            obs_rectangular.push_back(r);

            auto trajectory = pf.path_find(double_start, double_goal, obs_circular, obs_rectangular);

            return trajectory;
        }
    }

    void move_to(RobotController& robot, double goal[2], bool avoid_ball) {
        if (sqrt(pow(goal[0] - robot.mpos[0], 2) + pow(goal[1] - robot.mpos[1], 2)) < robot.mStatic_position_tolarance) {
            robot.mtarget_vel[0] = 0;
            robot.mtarget_vel[1] = 0;
            robot.mtarget_vyaw = 0;
            robot.mPositioned = true;
            robot.mTeam->positioned[robot.mId] = true;
            return;
        }
        robot.mPositioned = false;
        robot.mTeam->positioned[robot.mId] = false;
        std::vector<double> v_vet;
        auto trajectory = find_trajectory(robot, robot.mpos, goal, avoid_ball);
        std::size(trajectory) > 0 ? v_vet = motion_planner(robot, trajectory) : v_vet = {0, 0};


        v_vet = motion_control(v_vet, -robot.myaw);
        robot.mtarget_vel[0] = v_vet[0];
        robot.mtarget_vel[1] = v_vet[1];
        robot.mkicker_x = 0;
    }
}