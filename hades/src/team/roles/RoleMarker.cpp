//
// Created by caiu on 15/09/25.
//

#include "RoleMarker.h"
#include "../TeamInfo.h"


namespace roles {
	void RoleMarker::act(RobotController& robot) {


		if (robot.mWorld.ball.isMoving() && robot.mWorld.isBallMovingRobotDirection(robot)) {
			intercept.act(robot);
		} else {
			Robot enemy_support = {0};
			bool supportFound = false;
			try {
				enemy_support = robot.mTeam->getEnemyofRole(Robot::support, robot.mWorld.enemies);
				supportFound = true;
			} catch (...) {
				std::cout << "no enemy support found" << std::endl;
			}

        	if (supportFound) {
				mark.act(robot, enemy_support);
			}
            else {
            	intercept.act(robot); //Nao deveria chegar aqui
            }
		}


    }

} // roles