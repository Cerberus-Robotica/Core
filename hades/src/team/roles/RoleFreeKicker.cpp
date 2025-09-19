//
// Created by caiu on 19/09/25.
//

#include "RoleFreeKicker.h"
#include "../RobotController.h"
#include "../TeamInfo.h"

namespace roles {

	void RoleFreeKicker::act(RobotController& robot) {
		Point goal = robot.mWorld.field.theirGoal.getMiddle();
		bool can_score = false;
		try {
			goal = robot.mWorld.getGoalPosition();
			can_score = true;
		} catch (...) {}

		Robot support = Robot(-1);
		bool has_support = false;
		try {
			robot.mTeam->getRobotofRole(Robot::support);
			has_support = true;
		} catch (...) {}

		if (can_score) {
			positionAndKick.act(robot, goal, false);
		} else if (has_support) {
			positionAndKick.act(robot, support, false);
		} else {
			positionAndKick.act(robot, goal, false);
		}
	}
} // roles