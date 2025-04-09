//
// Created by caiom on 4/4/2025.
//

#include "Robot_controller.h"
#include <iostream>
#include <thread>
#include "../c_trajectory/C_trajectory.h"
#include <math.h>
#include "../include/handlers.hpp"
#include <chrono>

void Robot_controller::start(int* play_ads) {
    play = play_ads;
    std::thread t(&Robot_controller::loop, this);
    t.detach();
}

void Robot_controller::loop() {
    while (true) {
        recive_vision();
        std::cout << id << std::endl;
        std::cout << target_vel[0] << target_vel[1] << std::endl;
        std::cout << han.new_vision.field.field_length<< std::endl;
        std::cout << han.new_vision.balls.position_x << ", " << han.new_vision.balls.position_y << std::endl;
        std::cout << pos[0] << ", " << pos[1] << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        check_connection();
        if (terminate) {
            break;
        }
        role_table();

    }

}

void Robot_controller::check_connection() {
    if (!detected) {
        offline_counter++;
    } else {
        offline_counter = 0;
    }

    if (offline_counter >= max_offline_counter) {
        terminate = true;
    }
}


std::vector<std::vector<double>> Robot_controller::find_trajectory(float goal[2]) {
    C_trajectory pf(false, false, 0, 1000, 300, 0);
    std::vector<double> start = {pos[0], pos[1]};
    std::vector<double> double_goal = {double(goal[0]), double(goal[1])};

    std::vector<circle> obs_circular = {};
    std::vector<rectangle> obs_rectangular = {};

    auto trajectory = pf.path_find(start, double_goal, obs_circular, obs_rectangular);

    return trajectory;
}

std::vector<double> Robot_controller::motion_planner(std::vector<std::vector<double>> trajectory) {
    std::vector<double> pos = {pos[0], pos[1]};
    std::vector<double> v_vet = subtract(trajectory[1], pos);
    return v_vet;
}

std::vector<double> Robot_controller::motion_control(std::vector<double> v_vet) {
    return v_vet;
}

void Robot_controller::move_to(float goal[2]) {
    auto trajectory = find_trajectory(goal);
    auto v_vet = motion_planner(trajectory);
    v_vet = motion_control(v_vet);
    target_vel[0] = v_vet[0];
    target_vel[1] = v_vet[1];
}

void Robot_controller::role_table() {

    if (role == 0) {

    }
    else if (role == 1) {
        stricker_role();
    }
}



void Robot_controller::stricker_role() {
    if (state == 0) {
        if (sqrt(pow(pos[0] - ball_pos[0], 2) + pow(pos[1] - ball_pos[1], 2)) < radius*1.1) {
            target_vel[0] = 0;
            target_vel[1] = 0;
            state = 1;
            return;
        }
        move_to(ball_pos);

    } else if (state == 1) {
        target_vel[0] = 0;
        target_vel[1] = 0;
        if (sqrt(pow(pos[0] - ball_pos[0], 2) + pow(pos[1] - ball_pos[1], 2)) > radius*1.1) {
            state = 0;
        }
    }
}

void Robot_controller::recive_vision() {
    if (size(han.new_vision.robots_blue) > id) {
        pos[0] = han.new_vision.robots_blue[id].position_x;
        pos[1] = han.new_vision.robots_blue[id].position_y;
    }
}