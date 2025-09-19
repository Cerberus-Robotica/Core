//
// Created by caiu on 25/08/25.
//

#include "TacticPositionAndKick.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"
#include "../../c_trajectory/C_trajectory.h"

namespace tactics {
	void TacticPositionAndKick::act(RobotController& robot, Point goal, bool wait) {
		Point kick_pos = robot.mWorld.getKickingPosition(robot.mWorld.ball.getPosition(), goal, robot.mBall_avoidance_radius + robot.mRadius);
		if (wait or !robot.positioned or !robot.oriented or !robot.mWorld.isBallReachable(robot.getRole() != Robot::goal_keeper) or robot.getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) > distance_to_kick) {
			moveTo.act(robot, kick_pos, true);
			turnTo.act(robot, goal);
		} else {
			kick.act(robot);
		}
	}

	void TacticPositionAndKick::act(RobotController &robot, Robot sup, bool wait) {
		Point kick_pos = robot.mWorld.getKickingPosition(robot.mWorld.ball.getPosition(), sup.getPosition(), robot.mBall_avoidance_radius + robot.mRadius);

		if (wait or !robot.positioned or !robot.oriented or !sup.positioned or !robot.mWorld.isBallReachable(robot.getRole() != Robot::goal_keeper) or robot.getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) > distance_to_kick) {
			moveTo.act(robot, kick_pos, true);
			turnTo.act(robot, sup.getPosition());
		} else {
			kick.act(robot);
		}
	}


	void TacticPositionAndKick::act(RobotController& robot) {
          std::cout << "this is a dummy method" << std::endl;
	}

} // tactics