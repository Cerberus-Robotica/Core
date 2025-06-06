//
// Created by caiom on 4/7/2025.
//

#ifndef ROBOT_H
#define ROBOT_H
#include <deque>


class Robot {
public:
    int id = -1;
    double old_pos[2] = {0, 0};
    double pos[2] = {0, 0};
    double yaw = 0;
    double vel[2] = {0, 0};
    double vyaw = 0;
    std::deque<double> stored_speed_x = {};
    std::deque<double> stored_speed_y = {};
    bool detected = false;



    int role = -1;

    Robot(int id) {
        this->id = id;
    }
};



#endif //ROBOT_H
