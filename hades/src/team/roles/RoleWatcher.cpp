//
// Created by caiu on 19/09/25.
//

#include "RoleWatcher.h"
#include "../RobotController.h"
#include "../TeamInfo.h"

namespace roles {
	void RoleWatcher::act(RobotController& robot) {
		if (robot.mTeam->event == TeamInfo::ourPenalty or robot.mTeam->event == TeamInfo::theirPenalty) {
			LineSegment line = {Point(robot.mWorld.ball.getPosition().getX(), 0), Point(0, 0)};
			line = line.getResized(1000);
			moveTo.act(robot, {line.getEnd().getX(), robot.getId()*robot.getRadius()*1.2}, true);
			turnTo.act(robot, robot.mWorld.ball.getPosition());
		} else {
			stop.act(robot);
		}
	}
} // roles