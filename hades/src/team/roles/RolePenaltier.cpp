//
// Created by caiu on 19/09/25.
//

#include "RolePenaltier.h"
#include "../RobotController.h"
#include "../TeamInfo.h"

namespace roles {
	void RolePenaltier::act(RobotController& robot) {
		Point goal = robot.mWorld.getGoalPosition();
		if (robot.mTeam->event == TeamInfo::ourPenalty) {
			positionAndKick.act(robot, goal, true);
		}
		if (robot.mTeam->event == TeamInfo::runningOurPenalty) {
			positionAndKick.act(robot, goal, false);
		}
	}
} // roles