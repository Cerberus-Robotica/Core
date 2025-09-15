//
// Created by caiu on 25/08/25.
//

#include "TacticKeepXLine.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"

namespace tactics {
void TacticKeepXLine::act(RobotController& robot, LineSegment y_segment, double y_rest) {
        bool hasEnemies = false;
        for (Robot rob : robot.mWorld.enemies) if (rob.isDetected()) {hasEnemies = true; break;};

        bool hasStriker = false;
        int enemy_striker_id = -1;
        Robot enemy_striker(0);
        try {
            enemy_striker = robot.mTeam->getEnemyofRole(Robot::striker, robot.mWorld.enemies);
            enemy_striker_id = enemy_striker.getId();
            hasStriker = true;
        } catch (...) {
            std::cout << "no enemy striker" << std::endl;
        }

        double a = 1000;
        LineSegment line(Point(0, 0), Point(0, 0));
        if (hasEnemies) {
            if (robot.mWorld.ball.isMoving()) {
                line = robot.mWorld.ball.getMovementLine();
            }
            else if (robot.mWorld.ball.isStopped() && hasStriker) {    //para bola parada
                line = LineSegment(enemy_striker.getPosition(), robot.mWorld.ball.getPosition()).getResized(10000);
            } else {
                line = LineSegment(robot.mWorld.ball.getPosition(), y_segment.getMiddle()).getResized(10000);
            }
        }
        Point p = {y_segment.getStart().getX(), y_rest};
        try {
            p = line.intersection(y_segment);
        } catch (...) {
        }


        double y_max = y_segment.getEnd().getY() - robot.mRadius;
        double y_min = y_segment.getStart().getY() + robot.mRadius;
        p.setY(std::clamp(p.getY(), y_min, y_max));

        if (robot.mWorld.enemies.size() > 0 && hasStriker) {
            if ((robot.mWorld.enemies[enemy_striker_id].getPosition().getDistanceTo(robot.mWorld.ball.getPosition()) > 500 && robot.mWorld.ball.isStopped())) {
                p.setY(y_rest);
            }
        }
        moveTo.act(robot, p, false);
        robot.mkicker_x = 0;
        turnTo.act(robot, robot.mWorld.ball.getPosition());
    }
    void TacticKeepXLine::act(RobotController& robot) {
        std::cout << "this is a dummy method" << std::endl;
    }
} // tactics