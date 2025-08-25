//
// Created by caiu on 25/08/25.
//

#include "SkillTurnTo.h"
#include "../RobotController.h"
#include "../TeamInfo.h"
#include "../c_trajectory/C_trajectory.h"


namespace skills {
	double SkillTurnTo::find_angle_error(RobotController robot, Point goal) {
		double theta_final = atan2(goal.getY() - robot.getPosition().getY(), goal.getX() - robot.getPosition().getX());
		double orientation = robot.getYaw();
		double delta = theta_final - orientation;
		if (delta > M_PI) delta -= 2 * M_PI;
		if (delta < -M_PI) delta += 2 * M_PI;
		return delta;
	}

	double SkillTurnTo::turn_control(RobotController robot, double delta) {
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

void SkillTurnTo::act(RobotController& robot, Point goal) {
	double delta = find_angle_error(robot, goal);
	if (fabs(delta) < robot.mStatic_angle_tolarance) {
		robot.mtarget_vyaw = 0;
		robot.mOriented = true;
		return;
	}
	robot.mOriented = false;
	double new_vyaw = turn_control(robot, delta);
	if (new_vyaw > robot.mtarget_vyaw + robot.mDelta_time*robot.mA_ang_max) {
		new_vyaw = robot.mtarget_vyaw + robot.mDelta_time*robot.mA_ang_max;
	}
	if (new_vyaw < robot.mtarget_vyaw - robot.mDelta_time*robot.mA_ang_max) {
		new_vyaw = robot.mtarget_vyaw - robot.mDelta_time*robot.mA_ang_max;
	}
	robot.mtarget_vyaw = new_vyaw;
}
} // skills