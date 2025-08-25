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
		if (robot.mWorld.ball.getVelocity().getNorm() == 0 || !robot.mWorld.isBallMovingIdDirection(robot.getId())) {
			moveTo.act(robot, keep, true);
		}
		else {
			if (robot.getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) > robot.mRadius*5) moveTo.act(robot, robot.mWorld.ball.findStopPosition(), true);
			else moveTo.act(robot, robot.mWorld.ball.getPosition(), true);
		}
	}
} // tactics