//
// Created by caiu on 18/08/25.
//

#include "RoleKickOffKicker.h"
#include "../RobotController.h"
#include "../TeamInfo.h"

namespace roles {
    void RoleKickOffKicker::act(RobotController& robot) {
        bool all_positioned = true;
        for (Robot r : robot.mWorld.allies) if (!r.positioned)  all_positioned = false;

    	if (all_positioned && robot.positioned && robot.mWorld.isPointOnOurSide(robot.getPosition()) && robot.mTeam->event == TeamInfo::ourKickOff) {
    		try {	//TODO ALGO MUITO ERRADO
    			Robot support = robot.mTeam->getRobotofRole(Robot::kickoff_support);
    			positionAndKick.act(robot, support);
    		} catch (...) { //NO SUPPORT
    			Point p = robot.mWorld.getGoalPosition();
    			positionAndKick.act(robot, Robot::kickoff_support);
    		}
    	} else if (!all_positioned) {
	        Point their_goal = {(robot.mWorld.field.theirGoal.getStart().getX() + robot.mWorld.field.theirGoal.getEnd().getX())/2, (robot.mWorld.field.theirGoal.getStart().getY() + robot.mWorld.field.theirGoal.getEnd().getY())/2};
        	Point center = {0, 0};
        	auto kickposition = robot.mWorld.getKickingPosition(center, their_goal, robot.mBall_avoidance_radius + robot.mRadius);
        	moveTo.act(robot, kickposition, true);
        }
    }
} // roles