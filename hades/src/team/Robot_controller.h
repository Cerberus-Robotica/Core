//
// Created by caiom on 4/4/2025.
//

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H
#include <vector>
#include <lcm/lcm-cpp.hpp>

#include "robot.h"
#include "team_info.h"
#include "geometry/world_model.h"

class Robot_controller {

public:


    Robot_controller(int new_id){
        id = new_id;
    }

    void start(team_info* team_ads);
    void stop();
    void loop();



private:

    int id = -1; //id; -1 unsigned
    double radius = 160;
    double ball_avoidance_radius = 0;

    //observed status
    double pos[2] = {0, 0};
    double yaw = 0;
    double vel[2] = {0, 0};
    double vyaw = 0;

    //target movimentation
    double target_yaw = 0;
    double target_vel[2] = {0, 0};
    double target_vyaw = 0;
    double last_target_vel[2] = {0, 0};

    //actuators activation
    double kicker_x = 0;
    double kicker_z = 0;
    double dribbler = 0;

    //strategy status
    team_info* team;  //role; -1 unsigned

    int state = 0;  //estado

    double delta_time = 0;
    std::vector<std::vector<double>> current_trajectory = {};



    //on-field detection
    bool detected = true;
    int offline_counter = 0;
    int max_offline_counter = 100;
    bool terminate = false;


    //extreme params
    double vxy_max = 1.5;
    double vxy_min = 0.2;
    double a_xy_max = 2;
    double a_xy_brake = 2;
    double vyaw_max = 10;
    double vyaw_min = 0.5;
    double kicker_x_max = 3;
    double kicker_x_min = 0.5;
    double kicker_z_max = 1;
    double kicker_z_min = 0.5;
    double dribbler_max = 1;
    double dribbler_min = 0.5;

    //angle and position tolerance
    double static_position_tolarance = radius/8;
    double dynamic_position_tolarance = radius/16;
    double static_angle_tolarance = 0.005;

    //PID control
    double KP_ang = 10;
    double KI_ang = 0.0005;
    double KD_ang = 5;
    double I_ang = 0;
    double last_delta = 0;

    double KP_mov = 0;
    double KI_mov = 0;
    double KD_mov = 0;
    double I_mov = 0;

    //flags
    bool positioned = true;
    bool alligned = true;
    bool oriented = true;


    //field info
    world_model world;
    int64_t last_time_stamp = 0;


    void check_connection();
    void dynamic_calculations();

    void move_to(double goal[2], bool avoid_ball);
    std::vector<std::vector<double>> find_trajectory(double start[2], double goal[2], bool avoid_ball, bool is_ball);
    std::vector<double> motion_planner(std::vector<std::vector<double>> trajectory);
    std::vector<double> motion_control(std::vector<double> v_vet);


    void turn_to(double goal[2]);
    double find_angle_error(double goal[2]);
    double turn_control(double delta);


    void kick();
    void follow_trajectory(std::vector<std::vector<double>>& trajectory);

    void receive_vision();
    void publish();

    void role_table();
    void stricker_role();
    void goal_keeper_role();
    void attack_support_role();
};



#endif //ROBOT_H
