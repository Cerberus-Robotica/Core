//
// Created by caiu on 26/08/25.
//

#include "TacticIntercept.h"
#include "../RobotController.h"

namespace tactics {
	void TacticIntercept::act(RobotController& robot) {
    	if (robot.mWorld.ball.isStopped()) moveTo.act(robot, robot.mWorld.ball.getPosition(), true);

        if (robot.mWorld.ball.isMoving()) {
          	LineSegment line = {robot.mWorld.ball.getPosition(), robot.mWorld.ball.getVelocity()};
        	if (robot.getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) < distanceThreshold && line.isPointAligned(robot.getPosition(), angle_tolerance)) {
                  cushion.act(robot);
        	} else moveTo.act(robot, robot.mWorld.ball.getStopPosition(), false);
        }
	}
} // tactics