//
// Created by caiu on 25/08/25.
//

#include "RoleCircularTrajectory.h"
#include "../RobotController.h"

namespace roles {
	void RoleCircularTrajectory::act(RobotController& robot) {
		if (size(robot.mCurrent_trajectory) == 0) {
			int resolution = 120;
			for (int i = 0; i < resolution; i++) {
				robot.mCurrent_trajectory.emplace_back(1000*sin(2*i*M_PI/resolution + robot.getId()*2*M_PI/size(robot.mWorld.allies)), 1000*cos(2*i*M_PI/resolution + robot.getId()*2*M_PI/size(robot.mWorld.allies)));
			}
		}
		followTrajectory.act(robot, robot.mCurrent_trajectory);
    }
}