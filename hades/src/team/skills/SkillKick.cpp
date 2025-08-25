//
// Created by caiu on 25/08/25.
//

#include "SkillKick.h"
#include "../RobotController.h"

namespace skills {
	void SkillKick::act(RobotController& robot) {
		if (robot.mWorld.ball.getVelocity().getNorm() > robot.mVxy_min) {
			robot.mkicker_x = 0;
			robot.mState += 1;
		}

		Vector2d v_vet = {robot.mWorld.ball.getPosition(), robot.getPosition()};
		v_vet = v_vet.getNormalized(robot.mVxy_min);
		robot.mtarget_vel = v_vet.getRotated(-robot.getYaw());
		robot.mkicker_x = 3.5;
		//skills::turn_to(robot, robot.mWorld.ball.getPosition());
	}
} // skills