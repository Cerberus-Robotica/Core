//
// Created by caiu on 25/08/25.
//

#include "TacticPositionAndKick.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"
#include "../../c_trajectory/C_trajectory.h"

namespace tactics {
	void TacticPositionAndKick::act(RobotController& robot, Point goal) {
		Point kick_pos = robot.mWorld.getKickingPosition(robot.mWorld.ball.getPosition(), goal, robot.mBall_avoidance_radius + robot.mRadius);
		if (!robot.positioned or !robot.oriented or robot.getPosition().getDistanceTo(kick_pos) > (robot.mBall_avoidance_radius + robot.mRadius)*1.2) {
			moveTo.act(robot, kick_pos, true);
			turnTo.act(robot, goal);
		} else {
			kick.act(robot);
		}
	}

	void TacticPositionAndKick::act(RobotController &robot, Robot sup) {
		Point kick_pos = robot.mWorld.getKickingPosition(robot.mWorld.ball.getPosition(), sup.getPosition(), robot.mBall_avoidance_radius + robot.mRadius);

		if (!robot.positioned or !robot.oriented or !sup.positioned or robot.getPosition().getDistanceTo(kick_pos) > (robot.mBall_avoidance_radius + robot.mRadius)*1.2) {
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