//
// Created by caiu on 18/08/25.
//

#include "RoleMidField.h"
#include "../RobotController.h"

namespace roles {
    void RoleMidField::act(RobotController& robot) {
        //TODO continuar
        bool isPivot;
        int Pivot_id = -1;
        double their_goal[2] = {robot.mWorld.their_goal[0][0], 0};

        auto interceptor = robot.mWorld.getIdOfTheBallInterceptor();
        if (robot.mWorld.ball_speed_module != 0 && interceptor < 20 && robot.mTeam->num_of_active_robots > 1) {
            Pivot_id = interceptor;
        } else{
            auto closest_allies_to_ball = robot.mWorld.getAlliesIdsAccordingToDistanceToBall();
            if (closest_allies_to_ball.size() > 0) {
                if (closest_allies_to_ball[0] != robot.mTeam->goal_keeper_id) {
                    Pivot_id = closest_allies_to_ball[0];
                } else if (robot.mTeam->num_of_active_robots > 1) {
                    Pivot_id = closest_allies_to_ball[1];
                }
            }
            else {
                Pivot_id = robot.mId;
            }

        }


        if (Pivot_id == robot.mId || Pivot_id == -1) {
            if (robot.mWorld.ball_speed_module != 0) {
                tactics::keep_a_location(robot, robot.mpos);
            }
            if (tactics::aux::find_ball_trajectory(robot, robot.mWorld.ball_pos, their_goal).size() == 2 && distance_point(robot.mWorld.ball_pos, their_goal) < robot.mKick_distance) {
                tactics::position_and_kick_to_destination(robot, their_goal);
            }
            else {
                int nearest_ally_id = robot.mWorld.findNearestAllyThatIsntTheGoalKeeper(robot.mId, robot.mTeam->goal_keeper_id);
                nearest_ally_id != -1 ? tactics::position_and_kick_to_robot(robot, nearest_ally_id) : tactics::position_and_kick_to_destination(robot, their_goal);
            }
        }
        else {
            double mid_field_dislocation = fabs(robot.mWorld.ball_pos[0]);
            double x_position = std::clamp(robot.mTeam->central_line_x + (mid_field_dislocation)*robot.mTeam->our_side_sign, -robot.mTeam->mid_field_max_dislocation, robot.mTeam->mid_field_max_dislocation);
            double delta_y = sqrt(pow(robot.mKick_distance, 2) - pow(x_position - robot.mWorld.ball_pos[0], 2));
            double y_position;
            robot.mWorld.allies[Pivot_id].pos[1] != 0 ? y_position = robot.mWorld.ball_pos[1] - delta_y*robot.mWorld.allies[Pivot_id].pos[1]/fabs(robot.mWorld.allies[Pivot_id].pos[1])
                : y_position = robot.mWorld.ball_pos[1];
            if (y_position > robot.mWorld.their_defese_area[0][1] - robot.mRadius*2 && y_position < robot.mWorld.their_defese_area[1][1] + robot.mRadius*2) {
                if (y_position > 0) y_position = robot.mWorld.their_defese_area[1][1] + robot.mRadius*2;
                else y_position = robot.mWorld.their_defese_area[0][1] - robot.mRadius*2;
            }
            double wait_position[2] = {x_position, y_position};
            tactics::keep_a_location(robot, wait_position);
            skills::turn_to(robot, robot.mWorld.ball_pos);
        }
    }
} // roles