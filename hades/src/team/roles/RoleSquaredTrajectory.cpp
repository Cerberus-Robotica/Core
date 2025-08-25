//
// Created by caiu on 25/08/25.
//

#include "RoleSquaredTrajectory.h"
#include "../RobotController.h"

namespace roles {
	void RoleSquaredTrajectory::act(RobotController& robot){
		if (size(robot.mCurrent_trajectory) == 0) {
			robot.mCurrent_trajectory = {{4000, 4000}, {4000, -4000}, {4000, 4000}, {-4000, 4000}};
		}
		followTrajectory.act(robot, robot.mCurrent_trajectory);
    }

} // roles