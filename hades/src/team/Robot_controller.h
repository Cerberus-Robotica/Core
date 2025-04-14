//
// Created by caiom on 4/4/2025.
//

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H
#include <vector>
#include <lcm/lcm-cpp.hpp>


#include "robot.h"
#include "team_info.h"
#include "geometry/world.h"

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
    double radius = 220;

    //observed status
    double pos[2] = {0, 0};
    double yaw = 0;
    double vel[2] = {0, 0};
    double vyaw = 0;

    //target movimentation
    double target_yaw = 0;
    double target_vel[2] = {0, 0};
    double target_vyaw = 0;

    //actuators activation
    double kicker_x = 0;
    double kicker_z = 0;
    double dribbler = 0;

    //strategy status
    team_info* team;  //role; -1 unsigned
    int state = 0;  //estado
    robot allies[16] = {robot(0), robot(1), robot(2), robot(3), robot(4), robot(5), robot(6), robot(7), robot(8),
                        robot(9), robot(10), robot(11), robot(12), robot(13), robot(14), robot(15)};
    robot enemies[16] = {robot(0), robot(1), robot(2), robot(3), robot(4), robot(5), robot(6), robot(7), robot(8),
                        robot(9), robot(10), robot(11), robot(12), robot(13), robot(14), robot(15)};
    double delta_time = 0;
    std::vector<std::vector<double>> current_trajectory = {};



    //on-field detection
    bool detected = true;
    int offline_counter = 0;
    int max_offline_counter = 1000;
    bool terminate = false;

    //extreme params
    double vxy_max = 1;
    double vxy_min = 0.2;
    double vyaw_max = 1;
    double vyaw_min = 0.5;
    double kicker_x_max = 3;
    double kicker_x_min = 0.5;
    double kicker_z_max = 1;
    double kicker_z_min = 0.5;
    double dribbler_max = 1;
    double dribbler_min = 0.5;

    double static_position_tolarance = radius/2;
    double dynamic_position_tolarance = radius;
    double static_angle_tolarance = 0.005;

    //field info
    world field;
    int64_t last_time_stamp = 0;
    double ball_pos[2] = {0, 0};
    double ball_speed[2] = {0, 0};


    void check_connection();
    void dynamic_calculations();

    void move_to(double goal[2], bool avoid_ball);
    std::vector<std::vector<double>> find_trajectory(double start[2], double goal[2], bool avoid_ball);
    std::vector<double> motion_planner(std::vector<std::vector<double>> trajectory);
    std::vector<double> motion_control(std::vector<double> v_vet);


    void turn_to(double goal[2]);
    void kick();
    void follow_trajectory(std::vector<std::vector<double>>& trajectory);

    void recive_vision();
    void publish();

    void role_table();
    void stricker_role();
};



#endif //ROBOT_H
