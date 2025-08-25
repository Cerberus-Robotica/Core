//
// Created by caiu on 25/08/25.
//

#include <stdio.h>
#include "SkillMoveTo.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"
#include "../c_trajectory/C_trajectory.h"

namespace skills {
    Vector2d SkillMoveTo::motion_planner(RobotController& robot, std::vector<Point> trajectory) {
            Vector2d delta = {trajectory[1].getX() - robot.getPosition().getX(), trajectory[1].getY() - robot.getPosition().getY()};
            double dist = delta.getNorm() / 1000.0; // metros
            auto direction = delta.getNormalized(1);

            double curve_safe_speed = robot.mVxy_min;
            double v_target_magnitude = robot.mVxy_max;
            if (trajectory.size() > 2) {
                Point p0 = robot.getPosition();
                Point p1 = trajectory[1];
                Point p2 = trajectory[2];

                Vector2d v1 = {p1 , p0};
                Vector2d v2 = {p2, p1};

                double angle_between = v1.getAngleBetween(v2);
                double chord_length = p2.getDistanceTo(p1); // mm
                chord_length /= 1000.0; // Para metros

                double radius = chord_length / (2.0 * sin(std::max(angle_between/2, 1e-3)));
                radius = std::max(radius, 0.05);
                curve_safe_speed = sqrt(robot.mA_xy_max * radius)/4;
                curve_safe_speed = std::clamp(curve_safe_speed, robot.mVxy_min, robot.mVxy_max);

                if (angle_between > 70.0 * M_PI / 180.0) {
                    curve_safe_speed = robot.mVxy_min;
                }

            }

            double current_speed = robot.getVelocity().getNorm();

            double brake_distance = (robot.mVxy_max*robot.mVxy_max - curve_safe_speed * curve_safe_speed) / (2.0 * robot.mA_xy_brake);
            brake_distance = std::max(brake_distance, 0.0);
            if (dist <= brake_distance) {
                v_target_magnitude = curve_safe_speed;
            } else {
                v_target_magnitude = robot.mVxy_max;
            }

            v_target_magnitude = std::clamp(v_target_magnitude, -robot.mVxy_max, robot.mVxy_max);
            std::vector<double> v_target = {v_target_magnitude * direction.getX(), v_target_magnitude * direction.getY()};
            std::vector<double> error = {v_target[0] - robot.mlast_target_vel.getX(), v_target[1] - robot.mlast_target_vel.getY()};
            std::vector<double> acceleration = {0, 0};

            if (fabs(v_target[0]) > fabs(robot.mlast_target_vel.getX())) {
                acceleration[0] = std::clamp(error[0] / robot.mDelta_time, -robot.mA_xy_max, robot.mA_xy_max);
            } else {
                acceleration[0] = std::clamp(error[0] / robot.mDelta_time, -robot.mA_xy_brake, robot.mA_xy_brake);
            }

            if (fabs(v_target[1]) > fabs(robot.mlast_target_vel.getY())) {
                acceleration[1] = std::clamp(error[1] / robot.mDelta_time, -robot.mA_xy_max, robot.mA_xy_max);
            } else {
                acceleration[1] = std::clamp(error[1] / robot.mDelta_time, -robot.mA_xy_brake, robot.mA_xy_brake);
            }


            Vector2d vel_cmd = {robot.mlast_target_vel.getX() + acceleration[0]*robot.mDelta_time, robot.mlast_target_vel.getY() + acceleration[1]*robot.mDelta_time};
            if (vel_cmd.getNorm() > robot.mVxy_max) {
                vel_cmd = vel_cmd.getNormalized(robot.mVxy_max);
            }

            if (std::isnan(vel_cmd.getX())) vel_cmd.setX(0);
            if (std::isnan(vel_cmd.getY())) vel_cmd.setY(0);

            robot.mlast_target_vel = vel_cmd;
            return vel_cmd;
        }

        Vector2d SkillMoveTo::motion_control(Vector2d v_vet, double yaw) {
            const double ang = yaw;
            return v_vet.getRotated(ang);
        }


        std::vector<Point> SkillMoveTo::find_trajectory(RobotController& robot, Point start, Point goal, bool avoid_ball = true) {
            double minor[2] = {robot.mWorld.field.full_dimensions.getMinorPoint().getX(), robot.mWorld.field.full_dimensions.getMinorPoint().getY()};
            double major[2] = {robot.mWorld.field.full_dimensions.getMajorPoint().getX(), robot.mWorld.field.full_dimensions.getMajorPoint().getY()};
            C_trajectory pf(false, false, 0, 100, 50, 0, minor, major);

            std::vector<Circle> obs_circular = {};
            std::vector<Rectangle> obs_rectangular = {};
            Rectangle r({0, 0}, {0, 0});
            //rectangle r = field.their_defense_area;
            //obs_rectangular.push_back(r);

            //add static ball to obstacles according to avoidance radius
            if (avoid_ball) {
                Circle c({robot.mWorld.ball.getPosition().getX(), robot.mWorld.ball.getPosition().getY()}, robot.mBall_avoidance_radius + robot.mRadius);
                obs_circular.push_back(c);
            }

            //add static allies to obstacles
            for (int i = 0; i < size(robot.mWorld.allies) ; i++) {
                if (!robot.mWorld.allies[i].isDetected() || i == robot.getId()) {
                    continue;
                }
                Circle c({robot.mWorld.allies[i].getPosition().getX(), robot.mWorld.allies[i].getPosition().getY()}, robot.mRadius);
                obs_circular.push_back(c);
            }

            //add static enemies to obstacles
            for (int i = 0; i < size(robot.mWorld.enemies) ; i++) {
                if (!robot.mWorld.enemies[i].isDetected()) {
                    continue;
                }
                Circle c({robot.mWorld.enemies[i].getPosition().getX(), robot.mWorld.enemies[i].getPosition().getY()}, robot.mRadius);
                obs_circular.push_back(c);
            }
            if (robot.mTeam->roles[robot.getId()] != Robot::goal_keeper) {
                Rectangle r({robot.mWorld.field.ourDefenseArea.getMinorPoint().getX() - robot.mRadius, robot.mWorld.field.ourDefenseArea.getMinorPoint().getY() - robot.mRadius}, {robot.mWorld.field.ourDefenseArea.getMajorPoint().getX() + robot.mRadius, robot.mWorld.field.ourDefenseArea.getMajorPoint().getY() + robot.mRadius});
                obs_rectangular.push_back(r);
            }

            r.minor = {robot.mWorld.field.theirDefenseArea.getMinorPoint().getX() - robot.mRadius, robot.mWorld.field.theirDefenseArea.getMinorPoint().getY() - robot.mRadius};
            r.major = {robot.mWorld.field.theirDefenseArea.getMajorPoint().getX() + robot.mRadius, robot.mWorld.field.theirDefenseArea.getMajorPoint().getY() + robot.mRadius};
            obs_rectangular.push_back(r);


            //goal fisical barrier
            r.minor = {robot.mWorld.field.ourFisicalBarrier.getMinorPoint().getX() - robot.mRadius, robot.mWorld.field.ourFisicalBarrier.getMinorPoint().getY() + robot.mRadius};
            r.major = {robot.mWorld.field.ourFisicalBarrier.getMajorPoint().getX() - robot.mRadius, robot.mWorld.field.ourFisicalBarrier.getMajorPoint().getY() + robot.mRadius};
            obs_rectangular.push_back(r);

            r.minor = {robot.mWorld.field.theirFisicalBarrier.getMinorPoint().getX() - robot.mRadius, robot.mWorld.field.theirFisicalBarrier.getMinorPoint().getY() + robot.mRadius};
            r.major = {robot.mWorld.field.theirFisicalBarrier.getMajorPoint().getX() - robot.mRadius, robot.mWorld.field.theirFisicalBarrier.getMajorPoint().getY() + robot.mRadius};
            obs_rectangular.push_back(r);

            if (goal.getX() > robot.mWorld.field.full_dimensions.getMajorPoint().getX()) goal.setX(robot.mWorld.field.full_dimensions.getMajorPoint().getX());
            if (goal.getX() < robot.mWorld.field.full_dimensions.getMinorPoint().getX()) goal.setX(robot.mWorld.field.full_dimensions.getMinorPoint().getX());
            if (goal.getY() > robot.mWorld.field.full_dimensions.getMajorPoint().getY()) goal.setY(robot.mWorld.field.full_dimensions.getMajorPoint().getY());
            if (goal.getY() < robot.mWorld.field.full_dimensions.getMinorPoint().getY()) goal.setY(robot.mWorld.field.full_dimensions.getMinorPoint().getY());

            auto trajectory_vector = pf.path_find(start.getVector(), goal.getVector(), obs_circular, obs_rectangular);
            std::vector<Point> trajectory = {};
            for (int i = 0; i < trajectory_vector.size(); i++) {
                trajectory.emplace_back(trajectory_vector[i][0], trajectory_vector[i][1]);
            }

            return trajectory;
        }


    void SkillMoveTo::act(RobotController& robot, Point goal, bool avoid_ball) {
        if (robot.getPosition().getDistanceTo(goal) < robot.mStatic_position_tolarance) {
            robot.mtarget_vel = {0, 0};
            robot.mtarget_vyaw = 0; //TODO verificar
            robot.mPositioned = true;
            robot.mTeam->positioned[robot.getId()] = true;
            return;
        }
        robot.mPositioned = false;
        robot.mTeam->positioned[robot.getId()] = false;
        Vector2d v_vet;
        auto trajectory = find_trajectory(robot, robot.getPosition(), goal, avoid_ball);
        std::size(trajectory) > 0 ? v_vet = motion_planner(robot, trajectory) : v_vet = {0, 0};


        v_vet = motion_control(v_vet, -robot.getYaw());
        robot.mtarget_vel = v_vet;
        robot.mkicker_x = 0;
    }

    void SkillMoveTo::act(RobotController& robot) {
    	std::cout << "Dummy method" << std::endl;
    }
} // skills