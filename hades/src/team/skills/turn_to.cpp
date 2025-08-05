//
// Created by caiu on 05/08/25.
//
#include <cmath>
#include "../RobotController.h"

namespace skills {
    double find_angle_error(RobotController robot, double goal[2]) {
        double theta_final = atan2(goal[1] - robot.mpos[1], goal[0] - robot.mpos[0]);
        double orientation = robot.myaw;
        double delta = theta_final - orientation;
        if (delta > M_PI) delta -= 2 * M_PI;
        if (delta < -M_PI) delta += 2 * M_PI;
        return delta;
    }

    double turn_control(RobotController robot, double delta) {
        double P = robot.mKP_ang * delta;
        robot.mI_ang = robot.mI_ang + delta*robot.mDelta_time*robot.mKI_ang;
        double D = (delta-robot.mLast_delta)*robot.mKD_ang;
        double PID_vyaw = P + robot.mI_ang + D;

        robot.mLast_delta = delta;

        if (fabs(PID_vyaw) > robot.mVyaw_max) {
            PID_vyaw = robot.mVyaw_max*PID_vyaw/fabs(PID_vyaw);
        };
        if (fabs(PID_vyaw) < robot.mVyaw_min && fabs(PID_vyaw) != 0) {
            PID_vyaw = robot.mVyaw_min*PID_vyaw/fabs(PID_vyaw);
        }
        return PID_vyaw;
    }

    void turn_to(RobotController robot, double goal[2]) {
        double delta = find_angle_error(robot, goal);
        if (fabs(delta) < robot.mStatic_angle_tolarance) {
            robot.mtarget_vyaw = 0;
            robot.mOriented = true;
            return;
        }
        robot.mOriented = false;
        robot.mtarget_vyaw = turn_control(robot, delta);
    }
}