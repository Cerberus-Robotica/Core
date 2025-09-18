//
// Created by caiu on 18/09/25.
//

#include "TacticBlockBall.h"
#include "../TeamInfo.h"

namespace tactics {
	void TacticBlockBall::act(RobotController& robot, double block_distance) {
		LineSegment line(Point(0, 0), Point(0, 0));
		try {
			line = LineSegment(robot.mWorld.ball.getPosition(), robot.mTeam->getEnemyofRole(Robot::striker, robot.mWorld.enemies).getPosition());
		} catch (...) {
			std::cout << "no enemy striker" << std::endl;
			line = LineSegment(robot.mWorld.field.ourGoal.getMiddle(), robot.mWorld.ball.getPosition());
		}
		line = line.getResized(line.getLength() + block_distance);
		Point p = line.getEnd();
		moveTo.act(robot, p, false, false);
		turnTo.act(robot, robot.mWorld.ball.getPosition());
	}
} // tactics