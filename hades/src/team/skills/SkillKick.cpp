//
// Created by caiu on 25/08/25.
//

#include "SkillKick.h"
#include "../RobotController.h"
#include "../TeamInfo.h"

namespace skills {
	void SkillKick::act(RobotController& robot) {
		if (robot.getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) > distancethreshold) {
			robot.mkicker_x = 0;
			robot.mPositioned = false;
			robot.mTeam->positioned[robot.getId()] = false;
		}
		Vector2d v_vet = {robot.mWorld.ball.getPosition(), robot.getPosition()};
		v_vet = v_vet.getNormalized(robot.mVxy_min);
		robot.mtarget_vel = v_vet.getRotated(-robot.getYaw());
		robot.mkicker_x = 3.5;
	}
} // skills