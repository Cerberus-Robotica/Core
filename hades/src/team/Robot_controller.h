//
// Created by caiom on 4/4/2025.
//

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H
#include <vector>
#include <lcm/lcm-cpp.hpp>


#include "robot.h"

class Robot_controller {

public:
    int id = -1; //id; -1 unsigned
    float radius = 90;

    //observed status
    float pos[2] = {0, 0};
    float yaw = 0;
    float vel[2] = {0, 0};
    float vyaw = 0;

    //target movimentation
    float target_yaw = 0;
    float target_vel[2] = {0, 0};
    float target_vyaw = 0;

    //actuators activation
    float kicker_x = 0;
    float kicker_z = 0;
    float dribbler = 0;

    //strategy status
    int role = 1;  //role; -1 unsigned
    int state = 0;  //estado
    int *play;

    //on-field detection
    bool detected = true;
    int offline_counter = 0;
    int max_offline_counter = 10000;
    bool terminate = false;

    //extreme params
    float vxy_max = 1;
    float vxy_min = 0.5;
    float vyaw_max = 1;
    float vyaw_min = 0.5;
    float kicker_x_max = 1;
    float kicker_x_min = 0.5;
    float kicker_z_max = 1;
    float kicker_z_min = 0.5;
    float dribbler_max = 1;
    float dribbler_min = 0.5;

    //field info
    float ball_pos[2] = {500, 500};
    float ball_speed[2] = {0, 0};


    Robot_controller(int new_id){
        id = new_id;
    }

    void start(int* play_ads);
    void loop();



private:
    void check_connection();
    void move_to(float goal[2]);
    std::vector<std::vector<double>> find_trajectory(float goal[2]);
    std::vector<double> motion_planner(std::vector<std::vector<double>> trajectory);
    std::vector<double> motion_control(std::vector<double> v_vet);

    void recive_vision();

    void role_table();
    void stricker_role();
};



#endif //ROBOT_H
