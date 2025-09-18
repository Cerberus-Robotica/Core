//
// Created by caiu on 18/09/25.
//

#include "RoleRetaker.h"
#include "../TeamInfo.h"

namespace roles {
	void RoleRetaker::act(RobotController& robot) {
		bool has_goal = false;
		Point goal = robot.mWorld.field.theirGoal.getMiddle();
		try {
			goal = robot.mWorld.getGoalPosition();
			has_goal = true;
		} catch (...) {
		}

		if (robot.mWorld.ball.isMoving()) {
			intercept.act(robot);
		} else if (robot.mWorld.ball.isStopped() &&
			robot.mWorld.getClosestEnemyToPoint(robot.mWorld.ball.getPosition()).getPosition().getDistanceTo(robot.mWorld.ball.getPosition())
			 < robot.getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) && has_goal) {
			positionAndKick.act(robot, goal);
		}
		else {
			blockBall.act(robot);
		}
    }
} // roles