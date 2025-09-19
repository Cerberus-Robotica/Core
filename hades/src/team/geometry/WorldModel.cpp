//
// Created by caiu on 11/04/25.
//

#include "WorldModel.h"

#include <iostream>
#include <ostream>
#include <cmath>
#include <bits/algorithmfwd.h>

#include "Area.h"
#include "AreaCircular.h"
#include "../c_trajectory/geometry/Vetop.h"

Point WorldModel::getKickingPosition(Point pos_0, Point pos_1, double distance) {
    Vector2d direction = {0, 0};
    direction.setX(-pos_1.getX() + pos_0.getX());
    direction.setY(-pos_1.getY() + pos_0.getY());
    direction = direction.getNormalized(distance);
    return {direction.getX() + pos_0.getX(), direction.getY() + pos_0.getY()};
}

bool WorldModel::isPointOnOurSide(Point p) {
    if (p.getX() == 0) {
        return false;
    }
    else if (p.getX()/fabs(p.getX()) == field.ourGoal.getStart().getX()/fabs(field.ourGoal.getStart().getX())) {
        return true;
    }
    return false;
}

bool WorldModel::isPointOnOurArea(Point p) {
    if (p.getX() > field.ourDefenseArea.getMinorPoint().getX() && p.getX() < field.ourDefenseArea.getMajorPoint().getX()) {
        if (p.getY() > field.ourDefenseArea.getMinorPoint().getY() && p.getY() < field.ourDefenseArea.getMajorPoint().getY()) {
            return true;
        }
    }
    return false;
}

bool WorldModel::isPointOnTheirArea(Point p) {
    if (p.getX() > field.theirDefenseArea.getMinorPoint().getX() && p.getX() < field.theirDefenseArea.getMajorPoint().getX()) {
        if (p.getY() > field.theirDefenseArea.getMinorPoint().getY() && p.getY() < field.theirDefenseArea.getMajorPoint().getY()) {
            return true;
        }
    }
    return false;
}

std::vector<int> WorldModel::getAlliesIdsAccordingToDistanceToBall() {
    std::vector<double> distances_from_ball = {};
    std::vector<int> allies_ids = {};

    for (int i = 0; i < allies.size(); i++) {
        if (allies[i].isDetected()) {
            distances_from_ball.push_back(ball.getPosition().getDistanceTo(allies[i].getPosition()));
            allies_ids.push_back(i);
        }
    }

    for (int i = 0 ; i < size(distances_from_ball) ; i++) {
        for (int j = 0; j < size(distances_from_ball) - i - 1; j++) {
            if (distances_from_ball[j] > distances_from_ball[j + 1]) {
                std::swap(distances_from_ball[j], distances_from_ball[j + 1]);
                std::swap(allies_ids[j], allies_ids[j + 1]);
            }
        }
    }

    return allies_ids;
}

int WorldModel::findNearestAllyThatIsntTheGoalKeeper(int id, int goalkeeper_id) {
    double smallest_distance = 100000;
    int closest_id = -1;
    for (int i = 0; i < allies.size(); i++) {
        if (allies[i].isDetected() && i != id && i != goalkeeper_id) {
            double this_distance = allies[i].getPosition().getDistanceTo(allies[id].getPosition());
            if (this_distance < smallest_distance) {
                smallest_distance = this_distance;
                closest_id = i;
            }
        }
    }
    return closest_id;
}

int WorldModel::getIdOfTheBallInterceptor() {
    double a = 10000;
    if (ball.getVelocity().getX() != 0) a = ball.getVelocity().getY()/ball.getVelocity().getX();
    double c = ball.getPosition().getY() - a*ball.getPosition().getX();

    int closest_id = 0;
    double smallest_distance = 1000000;
    bool itsEnemy = false;
    for (int i = 0; i < allies.size(); i++) {
        if (allies[i].isDetected()) {
            if (!isBallMovingIdDirection(i)) continue;

            double this_robot_distance = (fabs(a * allies[i].getPosition().getX() - allies[i].getPosition().getY() + c) / sqrt(pow(a, 2) + 1));
            if (this_robot_distance < smallest_distance) {
                smallest_distance = this_robot_distance;
                closest_id = i;
            }
        }
    }
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].isDetected()) {
            double this_robot_distance = (fabs(a * enemies[i].getPosition().getX() - enemies[i].getPosition().getY() + c) / sqrt(pow(a, 2) + 1));
            if (this_robot_distance < smallest_distance) {
                smallest_distance = this_robot_distance;
                closest_id = i;
                itsEnemy = true;
            }
        }
    }

    return closest_id*!itsEnemy + 20*closest_id*itsEnemy;
}

bool WorldModel::isBallMovingIdDirection(int id, double angle_tolerance) {
    return angle_vectors_small({ball.getVelocity().getX(), ball.getVelocity().getY()},
        {allies[id].getPosition().getX() - ball.getPosition().getX(), allies[id].getPosition().getY() - ball.getPosition().getY()}) < angle_tolerance;
}

bool WorldModel::isBallMovingRobotDirection(Robot robot, double tolerance) {
    return angle_vectors_small({ball.getVelocity().getX(), ball.getVelocity().getY()},
        {robot.getPosition().getX() - ball.getPosition().getX(), robot.getPosition().getY() - ball.getPosition().getY()}) < tolerance;
}

bool WorldModel::doInterceptAnyRobot(LineSegment l) {
    for (Robot ally : allies) {
        if (ally.isDetected()) if (AreaCircular(ally.getPosition(), ally.getRadius()).detectIfIntercepts(l)) return true;
    }
    for (Robot enemy : enemies) {
        if (enemy.isDetected()) if (AreaCircular(enemy.getPosition(), enemy.getRadius()).detectIfIntercepts(l)) return true;
    }
    return false;
}

bool WorldModel::isBallReachable(bool includeOurArea) {
    // Create a list of areas to check
    std::vector<AreaRectangular> areasToCheck = {
        field.theirDefenseArea,
        field.ourFisicalBarrier,
        field.theirFisicalBarrier
    };
    if (includeOurArea) {
        areasToCheck.push_back(field.ourDefenseArea);
    }

    // Check if ball is in any of these areas
    for (AreaRectangular& area : areasToCheck) {
        if (area.detectIfContains(ball.getPosition())) {
            return false;
        }
    }

    return true;
}

Robot WorldModel::getClosestAllyToPoint(Point p) {
    Robot closest = allies[0];
    while (int i = 0 < allies.size()) {
        if (allies[i].isDetected()) {
            closest = allies[i];
            break;
        }
        if (i == 16) throw std::runtime_error("No allies");
    }
    for (Robot r : allies) {
        if (r.getPosition().getDistanceTo(p) < closest.getPosition().getDistanceTo(p)) closest = r;
    }
    return closest;
}

Robot WorldModel::getClosestEnemyToPoint(Point p) {
    Robot closest = enemies[0];
    while (int i = 0 < enemies.size()) {
        if (enemies[i].isDetected()) {
            closest = enemies[i];
            break;
        }
        if (i == 16) throw std::runtime_error("No allies");
    }
    for (Robot r : enemies) {
        if (r.getPosition().getDistanceTo(p) < closest.getPosition().getDistanceTo(p)) closest = r;
    }
    return closest;
}

bool WorldModel::isBallHittingTheGoal() {
    return ball.getMovementLine().intersects(field.theirGoal);
}

Point WorldModel::getGoalPosition(Robot goalkeeper) {
    std::vector<Point> points = {};
    int num = 12;
    int k1 = 1;
    int k2 = 1;
    int k3 = 1;

    for (int i = 1; i < num; i++) {
        Point p = field.theirGoal.getResized(i*field.theirGoal.getLength()/num).getEnd();
        if (!ball.isVisible(p)) continue;

        points.push_back(p);
    }

    int best_idx = 0;
    for (int i = 1; i < points.size(); i++) {
        if (points[best_idx].getDistanceTo(goalkeeper.getPosition()) < points[i].getDistanceTo(goalkeeper.getPosition())) { //TODO melhorar essa funcao
            best_idx = i;
        }
    }
    if (points.size() == 0) throw std::runtime_error("No goals found");
    return points[best_idx];
}


bool WorldModel::isAllAlliesOnOurSide() {
    for (Robot& r : allies) {
        if (!isPointOnOurSide(r.getPosition())) return false;
    }
    return true;
}

bool WorldModel::isAllEnemiesOnTheirSide() {
    for (Robot& r : enemies) {
        if (isPointOnOurSide(r.getPosition())) return false;
    }
    return true;
}

bool WorldModel::isAllAlliesOnOurSideorOnCenterCircle() {
    for (Robot& r : allies) {
        if (!isPointOnOurSide(r.getPosition()) && r.getPosition().getDistanceTo({0, 0}) > field.center_circle_radius) return false;
    }
    return true;
}
