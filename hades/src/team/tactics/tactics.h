//
// Created by caiu on 06/08/25.
//

#ifndef TACTICS_H
#define TACTICS_H
#include <vector>

class RobotController;

#include <cmath>

namespace tactics {
    void keep_a_location(RobotController& robot, double keep[2]);
    void keep_x_line(RobotController& robot, double x_line, const double y_segment[2], double y_rest);
    void position_and_kick_to_destination(RobotController& robot, double goal[2]);
    void position_and_kick_to_robot(RobotController& robot, int id);
    void follow_trajectory(RobotController& robot, std::vector<std::vector<double>>& trajectory);
    namespace aux {
        std::vector<std::vector<double>> find_ball_trajectory(RobotController& robot, double start[2], double goal[2]);
    }
}

#endif //TACTICS_H
