//
// Created by caiu on 18/08/25.
//

#include "RoleStriker.h"

#include <iostream>

#include "../RobotController.h"
#include "../TeamInfo.h"
#include "../geometry/Point.h"

namespace roles {
    void RoleStriker::act(RobotController& robot) {
        //TODO melhorar striker_role para chutar para o passe
        //TODO fazer interceptar a bola melhor
        bool isPivot;
        int Pivot_id = -1;
        Point their_goal = {robot.mWorld.field.theirGoal.getStart().getX(), 0};

        auto interceptor = robot.mWorld.getIdOfTheBallInterceptor();
        if (robot.mWorld.ball.getVelocity().getNorm() != 0 && interceptor < 20 && robot.mTeam->num_of_active_robots > 1) {
            Pivot_id = interceptor;
        } else{
            auto closest_allies_to_ball = robot.mWorld.getAlliesIdsAccordingToDistanceToBall();
            if (closest_allies_to_ball.size() > 0) {
                if (closest_allies_to_ball[0] != robot.mTeam->goal_keeper_id) {
                    Pivot_id = closest_allies_to_ball[0];
                } else if (robot.mTeam->num_of_active_robots > 1) {
                    Pivot_id = closest_allies_to_ball[1];
                }
            } else Pivot_id = robot.getId();
        }

        if (Pivot_id == robot.getId() || Pivot_id == -1) { //TODO resolver aqui VVVVVVV
            if (positionAndKick.find_ball_trajectory(robot, robot.mWorld.ball.getPosition(), their_goal).size() == 2 && robot.mWorld.ball.getPosition().getDistanceTo(their_goal) < robot.mKick_distance) {
                positionAndKick.act(robot, their_goal);
            }
            else {
                int nearest_ally_id = robot.mWorld.findNearestAllyThatIsntTheGoalKeeper(robot.getId(), robot.mTeam->goal_keeper_id);
                nearest_ally_id != -1 ? positionAndKick.act(robot, nearest_ally_id) : positionAndKick.act(robot, their_goal);
            }
        }
        else {

            double striker_dislocation = fabs(robot.mWorld.ball.getPosition().getX()) + 1000;
            double x_position = std::clamp(robot.mTeam->central_line_x + (striker_dislocation)*robot.mTeam->our_side_sign, -robot.mTeam->striker_max_dislocation, robot.mTeam->striker_max_dislocation);
            double delta_y = sqrt(fabs(pow(robot.mKick_distance, 2) - pow(x_position - robot.mWorld.ball.getPosition().getX(), 2)));
            double y_position;
            robot.mWorld.allies[Pivot_id].getPosition().getY() != 0 ? y_position = robot.mWorld.ball.getPosition().getY() - delta_y*robot.mWorld.allies[Pivot_id].getPosition().getY()/fabs(robot.mWorld.allies[Pivot_id].getPosition().getY())
                : y_position = robot.mWorld.ball.getPosition().getY();
            if (y_position > robot.mWorld.field.theirDefenseArea.getMinorPoint().getY() - robot.mRadius*2 && y_position < robot.mWorld.field.theirDefenseArea.getMajorPoint().getY() + robot.mRadius*2) {
                if (y_position > 0) y_position = robot.mWorld.field.theirDefenseArea.getMajorPoint().getY() + robot.mRadius*2;
                else y_position = robot.mWorld.field.theirDefenseArea.getMinorPoint().getY() - robot.mRadius*2;
            }
            Point wait_position = {x_position, y_position};
            keepLocation.act(robot, wait_position);
            turnTo.act(robot, robot.mWorld.ball.getPosition());
        }
    }
} // roles