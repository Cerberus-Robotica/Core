//
// Created by caiu on 06/08/25.
//

#ifndef TACTICS_H
#define TACTICS_H
#include <vector>

#include "../geometry/LineSegment.h"

class RobotController;

#include <cmath>

namespace tactics {
    void keep_a_location(RobotController& robot, Point keep);
    void keep_x_line(RobotController& robot, LineSegment y_segment, double y_rest);
    void position_and_kick_to_destination(RobotController& robot, Point goal);
    void position_and_kick_to_robot(RobotController& robot, int id);
    void follow_trajectory(RobotController& robot, std::vector<Point>& trajectory);
    namespace aux {
        std::vector<Point> find_ball_trajectory(RobotController& robot, Point start, Point goal);
    }
}

#endif //TACTICS_H
