//
// Created by caiu on 11/04/25.
//

#include "WorldModel.h"

#include <iostream>
#include <ostream>
#include <cmath>
#include <bits/algorithmfwd.h>

#include "../c_trajectory/geometry/Vetop.h"

Point WorldModel::getKickingPosition(Point pos_0, Point pos_1, double distance) {
    Vector2d direction = {0, 0};
    direction.setX(-pos_1.getX() + pos_0.getX());
    direction.setY(-pos_1.getY() + pos_0.getY());
    direction = direction.getNormalized(distance);
    return {direction.getX() + pos_0.getX(), direction.getY() + pos_0.getY()};
}

bool WorldModel::isBallOnOurSide() {
    if (ball.getPosition().getX() == 0) {
        return false;
    }
    else if (ball.getPosition().getX()/fabs(ball.getPosition().getX()) == our_goal[0][0]/fabs(our_goal[0][0])) {
        return true;
    }
    return false;
}

bool WorldModel::isBallOnOurArea() {
    if (ball.getPosition().getX() > our_defese_area[0][0] && ball.getPosition().getX() < our_defese_area[1][0]) {
        if (ball.getPosition().getY() > our_defese_area[0][1] && ball.getPosition().getY() < our_defese_area[1][1]) {
            return true;
        }
    }
    return false;
}

bool WorldModel::isBallOnTheirArea() {
    if (ball.getPosition().getX() > their_defese_area[0][0] && ball.getPosition().getX() < their_defese_area[1][0]) {
        if (ball.getPosition().getY() > their_defese_area[0][1] && ball.getPosition().getY() < their_defese_area[1][1]) {
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

void WorldModel::generateBallStopPosition() {
    //FIXME gambiarra
    //FIXME calcula mal (provavel culpa da ball_speed
    ball.findStopPosition();
    //ball_stop_position[0] = std::clamp(ball_pos[0] + 1000*ball_speed[0]*ball_speed[0]/(2*ball_disacceleration), outside_field_x_minus[1][0] + 400, outside_field_x_plus[0][0] - 400);
    //ball_stop_position[1] = std::clamp(ball_pos[1] + 1000*ball_speed[1]*ball_speed[1]/(2*ball_disacceleration), outside_field_y_minus[0][1] + 400, outside_field_y_plus[0][1] - 400);
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

bool WorldModel::isBallMovingIdDirection(int id) {
    return angle_vectors_small({ball.getVelocity().getX(), ball.getVelocity().getY()},
        {allies[id].getPosition().getX() - ball.getPosition().getX(), allies[id].getPosition().getY() - ball.getPosition().getY()}) < M_PI/2;
}
