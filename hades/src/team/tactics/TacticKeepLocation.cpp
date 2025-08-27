//
// Created by caiu on 25/08/25.
//

#include "TacticKeepLocation.h"
#include "../RobotController.h"
#include <iostream>

namespace tactics {
	void TacticKeepLocation::act(RobotController& robot){
        std::cout << "this is a dummy method" << std::endl;
    }

	void TacticKeepLocation::act(RobotController& robot, Point keep){
		if (robot.mWorld.ball.isStopped() || !robot.mWorld.isBallMovingIdDirection(robot.getId())) {
			moveTo.act(robot, keep, true);
		}
		else {
			if (robot.mWorld.ball.isMoving()) {
				LineSegment line = {robot.mWorld.ball.getPosition(), robot.mWorld.ball.getVelocity()};
				if (robot.getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) < distanceThreshold && line.isPointAligned(robot.getPosition(), angle_tolerance)) {
					cushion.act(robot);
				} else moveTo.act(robot, robot.mWorld.ball.getStopPosition(), false);
			}
		}
	}
} // tactics