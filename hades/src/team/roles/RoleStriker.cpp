//
// Created by caiu on 18/08/25.
//

#include "RoleStriker.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"
#include "../geometry/Point.h"

namespace roles {
    Point RoleStriker::getSupportPosition(RobotController robot) {
        int N = 12;
        int k1 = 1;
        std::vector<Point> points;
        points.reserve(N);
        for (int i = 0; i < N; i++) {
            double angle = 2.0 * M_PI * i / N;
            double x = 0;
            double y = 0;
            try {
                x = robot.mWorld.ball.getPosition().getX() + robot.mTeam->getRobotofRole(Robot::striker).getKickDistance() * cos(angle);
                y = robot.mWorld.ball.getPosition().getY() + robot.mTeam->getRobotofRole(Robot::striker).getKickDistance() * sin(angle);
            } catch (...) { // no striker
                x = robot.mWorld.ball.getPosition().getX() + robot.getKickDistance() * cos(angle);
                y = robot.mWorld.ball.getPosition().getY() + robot.getKickDistance() * sin(angle);
            }
            Point p(x, y);
            if (!robot.mWorld.ball.isVisible(p)) continue;
            if (!robot.mWorld.field.inside_dimensions.detectIfContains(p)) continue;    ////TODO problema quando posicoes caem dentro da area de defesa
            points.push_back(p);
        }


        int best_idx = 0;
        for (int i = 1; i < points.size(); i++) {
            if (points[best_idx].getDistanceTo(robot.mWorld.field.theirGoal.getMiddle())*k1 > points[i].getDistanceTo(robot.mWorld.field.theirGoal.getMiddle())*k1) { //TODO melhorar essa funcao
                best_idx = i;
            }
        }
        if (points.size() == 0) throw std::runtime_error("No support position found");
        return points[best_idx];
    }

    void RoleStriker::act(RobotController& robot) {
        Point goal = robot.mWorld.field.theirGoal.getMiddle();
        bool hasGoalPosition = false;
        try {
            goal = robot.mWorld.getGoalPosition(robot.mTeam->getEnemyofRole(Robot::goal_keeper, robot.mWorld.enemies));
            hasGoalPosition = true;
        } catch (...) {
            std::cout << "no score position found" << std::endl;
            goal = robot.mWorld.field.theirGoal.getMiddle();
        }
        LineSegment robot_goal = {robot.mWorld.ball.getPosition(), goal};
        if (robot.mWorld.ball.isMoving() && robot.mWorld.ball.getMovementLine().isPointAligned(robot.getPosition(), 3.1415/8)) {
            intercept.act(robot);
        }
        else if (robot.mWorld.ball.isMoving() || robot.mWorld.isPointOnOurArea(robot.mWorld.ball.getPosition())) {
            Point p = getSupportPosition(robot);
            keepLocation.act(robot, p);
        }
        else if (hasGoalPosition && robot_goal.getLength() <= robot.getKickDistance()) {
            positionAndKick.act(robot, goal);
        } else {
            try {
                Robot support = robot.mTeam->getRobotofRole(Robot::support);
                positionAndKick.act(robot, support);
            } catch (...) {
                positionAndKick.act(robot, goal);
            }
        }
    }
} // roles