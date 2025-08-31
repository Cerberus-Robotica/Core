//
// Created by caiu on 25/08/25.
//

#include "TacticFollowTrajectory.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"

namespace tactics {
	void TacticFollowTrajectory::act(RobotController& robot, std::vector<Point>& trajectory) {

		int i = 0;
		while (size(trajectory) > 0) {
			double distance = robot.getPosition().getDistanceTo(trajectory[0]);
			if (size(trajectory) == 1 && distance < robot.mStatic_position_tolarance) {
				trajectory.erase(trajectory.begin());
				break;
			}

			if (distance < robot.mDynamic_position_tolarance) {
				trajectory.erase(trajectory.begin());
				continue;
			}
			break;
		}
		if (size(trajectory) == 0) {
			robot.mtarget_vel = {0, 0};
			robot.positioned = true;
			robot.mTeam->positioned[robot.getId()] = true;
			return;
		}

		moveTo.act(robot, trajectory[0], true);

		robot.positioned = false;
		robot.mTeam->positioned[robot.getId()] = false;
	}
	void TacticFollowTrajectory::act(RobotController& robot) {
		std::cout << "this is a dummy method" << std::endl;
	}
} // tactics