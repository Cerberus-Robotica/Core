//
// Created by caiu on 18/08/25.
//

#include "RoleSupport.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"

namespace roles {
    Point RoleSupport::getSupportPosition(RobotController robot) {
        int N = 12;
        int k1 = 1;
        std::vector<Point> points;
        points.reserve(N);

        for (int i = 0; i < N; i++) {
            double angle = 2.0 * M_PI * i / N;
            double x = robot.mWorld.ball.getPosition().getX() + robot.mTeam->getRobotofRole(Robot::striker).getKickDistance() * cos(angle);
            double y = robot.mWorld.ball.getPosition().getY() + robot.mTeam->getRobotofRole(Robot::striker).getKickDistance() * sin(angle);
            Point p(x, y);
            if (!robot.mWorld.ball.isVisible(p)) continue;
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

        // try {
        //     AreaCircular a1 = {robot.mWorld.field.theirGoal.getMiddle(), robot.mKick_distance};
        //     AreaCircular a2 = {robot.mWorld.ball.getPosition(), robot.mKick_distance};
        //     try {
        //         return a1.getInterceptionPoints(a2)[0];
        //     } catch (...) {
        //         return a2.getInterceptionPoints(LineSegment(robot.mWorld.field.theirGoal.getMiddle(), robot.mWorld.ball.getPosition()))[0];
        //     }
        //
        // } catch (...) {
        //     return {0, 0};
        // }
    }
    void RoleSupport::act(RobotController& robot) {
        //TODO continuar
        if (robot.mWorld.ball.isMoving() && robot.mWorld.isBallMovingRobotDirection(robot)) {
            intercept.act(robot);
        } else {
            try {
                keepLocation.act(robot, getSupportPosition(robot));
            } catch (...) {
                keepLocation.act(robot, Point(0, 0));
            }
        }
    }
} // roles