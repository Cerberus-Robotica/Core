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
		if (!robot.mPositioned or !robot.mOriented) {
			Point kick_pos = robot.mWorld.getKickingPosition(robot.mWorld.ball.getPosition(), goal, robot.mBall_avoidance_radius + robot.mRadius);
			moveTo.act(robot, kick_pos, true);
			turnTo.act(robot, goal);
		} else {
			kick.act(robot);
		}
	}

	void TacticPositionAndKick::act(RobotController& robot, int id) {
		Point goal = robot.mWorld.allies[id].getPosition();
		if (!robot.mPositioned or !robot.mOriented) {
			Point kick_pos = robot.mWorld.getKickingPosition(robot.mWorld.ball.getPosition(), goal, robot.mBall_avoidance_radius + robot.mRadius);
			moveTo.act(robot, kick_pos, true);
			turnTo.act(robot, goal);
		} else {
			kick.act(robot);
		}
	}

	std::vector<Point> TacticPositionAndKick::find_ball_trajectory(RobotController& robot, Point start, Point goal) {
		double minor[2] = {robot.mWorld.field.full_dimensions.getMinorPoint().getX(), robot.mWorld.field.full_dimensions.getMinorPoint().getY()};
		double major[2] = {robot.mWorld.field.full_dimensions.getMajorPoint().getX(), robot.mWorld.field.full_dimensions.getMajorPoint().getY()};
		C_trajectory pf(false, false, 0, 1000, 300, 0, minor, major);
		std::vector<Circle> obs_circular = {};
		std::vector<Rectangle> obs_rectangular = {};

		//add static enemies to obstacles
		for (int i = 0; i < size(robot.mWorld.enemies) ; i++) {
			if (!robot.mWorld.enemies[i].isDetected()) {
				continue;
			}
			Circle c({robot.mWorld.enemies[i].getPosition().getX(), robot.mWorld.enemies[i].getPosition().getY()}, robot.mRadius);
			obs_circular.push_back(c);
		}
		auto trajectory_vector = pf.path_find(start.getVector(), goal.getVector(), obs_circular, obs_rectangular);
		std::vector<Point> trajectory = {};
		for (int i = 0; i < trajectory_vector.size(); i++) {
			trajectory.emplace_back(trajectory_vector[i][0], trajectory_vector[i][1]);
		}
		return trajectory;
	}
	void TacticPositionAndKick::act(RobotController& robot) {
          std::cout << "this is a dummy method" << std::endl;
	}

} // tactics