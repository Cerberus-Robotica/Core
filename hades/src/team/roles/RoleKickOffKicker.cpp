//
// Created by caiu on 18/08/25.
//

#include "RoleKickOffKicker.h"
#include "../RobotController.h"
#include "../TeamInfo.h"

namespace roles {
    void RoleKickOffKicker::act(RobotController& robot) {
        bool others_positioned = true;
        for (Robot r : robot.mWorld.allies) if (!r.positioned && !r.getId() != robot.getId()) others_positioned = false;

    	if (others_positioned && robot.mWorld.isAllAlliesOnOurSideorOnCenterCircle() && robot.mTeam->event == TeamInfo::ourKickOff && robot.mWorld.ball.getPosition().getDistanceTo({0, 0}) < 100) {
    		try {	//TODO ALGO MUITO ERRADO
    			Robot support(-1);
    			try {
    				support = robot.mTeam->getRobotofRole(Robot::kickoff_support);
    			} catch (...) {
    				support = robot.mTeam->getRobotofRole(Robot::support);
    			}
    			positionAndKick.act(robot, support);
    		} catch (...) { //NO SUPPORT
    			Point p = robot.mWorld.getGoalPosition();
    			positionAndKick.act(robot, p);
    		}
    	} else {
	        Point their_goal = robot.mWorld.field.theirGoal.getMiddle();
    		Point center = {0, 0};
        	auto kickposition = robot.mWorld.getKickingPosition(center, their_goal, robot.mBall_avoidance_radius + robot.mRadius + 10); //Nao sei, nao me pergunte
    		moveTo.act(robot, kickposition, true);
    		turnTo.act(robot, robot.mWorld.ball.getPosition());
        }
    }
} // roles