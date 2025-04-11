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


void Robot_controller::start(team_info* team_ads) {
    team = team_ads;
    terminate = false;
    std::thread t(&Robot_controller::loop, this);
    t.detach();
}

void Robot_controller::stop() {
    team->active_robots[id] = false;
    terminate = true;
}

void Robot_controller::loop() {
    auto t0 = std::chrono::steady_clock::now();
    int cycles = 0;
    double yaw_prev = yaw;
    while (not terminate) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        recive_vision();

        check_connection();
        team->roles[id] = 1;
        role_table();

        target_vyaw = 2;
        publish();

        double yaw_curr = yaw;
        if (yaw_curr != yaw_prev) {
            cycles++;
            yaw_prev = yaw_curr;
        }



        auto t1 = std::chrono::steady_clock::now();

        std::chrono::duration<double> delta = t1 - t0;
        delta_time = delta.count();
        if (delta_time != 0) {
            double frequency = cycles/delta_time;
            if (frequency < 500) {
                std::cout << id << std::endl;
                std::cout << "Frequencia: " << frequency << std::endl;
                std::cout << delta_time << std::endl;
            }
        }

    }

}




std::vector<std::vector<double>> Robot_controller::find_trajectory(double start[2], double goal[2]) {
    C_trajectory pf(false, false, 0, 1000, 50, 0);
    std::vector<double> double_start = {start[0], start[1]};
    std::vector<double> double_goal = {static_cast<double>(goal[0]), static_cast<double>(goal[1])};

    std::vector<circle> obs_circular = {};
    std::vector<rectangle> obs_rectangular = {};

    for (int i = 0; i < 16 ; i++) {
        if (!allies[i].detected) {
            continue;
        }
        circle c({allies[i].pos[0], allies[i].pos[1]}, radius);
        obs_circular.push_back(c);
    }
    for (int i = 0; i < 16 ; i++) {
        if (!enemies[i].detected) {
            continue;
        }
        circle c({enemies[i].pos[0], enemies[i].pos[1]}, radius);
        obs_circular.push_back(c);
    }

    auto trajectory = pf.path_find(double_start, double_goal, obs_circular, obs_rectangular);
    return trajectory;
}



std::vector<double> Robot_controller::motion_planner(std::vector<std::vector<double>> trajectory) {
    std::vector<double> vect_pos = {pos[0], pos[1]};
    std::vector<double> v_vet = subtract(trajectory[1], vect_pos);
    return v_vet;
}

std::vector<double> Robot_controller::motion_control(std::vector<double> v_vet) {
    //std::cout << "AAAAAAAAAA" << std::endl << std::endl;
    //std::cout << v_vet[0] << ", " << v_vet[1] << std::endl;
    const double ang = -yaw;
    double x = v_vet[0];
    double y = v_vet[1];
    v_vet = {x * cos(ang) - y * sin(ang), x * sin(ang) + y * cos(ang)};
    double db_vmax = static_cast<double>(vxy_max);
    v_vet = normalize(db_vmax, v_vet);
    return v_vet;
}

void Robot_controller::move_to(double goal[2]) {
    auto trajectory = find_trajectory(pos, goal);
    auto v_vet = motion_planner(trajectory);
    v_vet = motion_control(v_vet);
    target_vel[0] = v_vet[0];
    target_vel[1] = v_vet[1];
}


void Robot_controller::turn_to(double goal[2]) {
    double theta_final = atan2(goal[1] - pos[1], goal[0] - pos[0]);
    double orientation = yaw;
    double delta = theta_final - orientation;
    if (delta > M_PI) delta -= 2 * M_PI;
    if (delta < -M_PI) delta += 2 * M_PI;
    if (fabs(delta) < static_angle_tolarance) {
        target_vyaw = 0;
        return;
    }
    double KP_ang = 0.5;
    target_vyaw = KP_ang * abs(delta) * delta/fabs(delta);
    if (fabs(target_vyaw) > vyaw_max) {
        target_vyaw = vyaw_max*target_vyaw/fabs(target_vyaw);
    };
    if (fabs(target_vyaw) < vyaw_min) {
        target_vyaw = vyaw_min*target_vyaw/fabs(target_vyaw);
    }
}


void Robot_controller::role_table() {
    //TODO roles
    if (team->roles[id] == 0) {

    }
    else if (team->roles[id] == 1) {
        stricker_role();
    }
}



void Robot_controller::stricker_role() {
    //TODO melhorar stricker_role
    turn_to(ball_pos);
    return;
    if (state == 0) {

        double goal[2] = {(field.their_goal[0][1] - field.their_goal[0][0])/2, (field.their_goal[1][1] - field.their_goal[1][0])/2};
        auto traj = find_trajectory(ball_pos, goal);
        std::vector<double> kick_pos = field.kicking_position(traj[0], traj[1], radius*1.2);

        double db_kick_pos[2] = {kick_pos[0], kick_pos[1]};;
        move_to(db_kick_pos);
        if (sqrt(pow(pos[0] - db_kick_pos[0], 2) + pow(pos[1] - db_kick_pos[1], 2)) < radius*1.3) {
            target_vel[0] = 0;
            target_vel[1] = 0;
            state = 1;
            return;
        }
        move_to(ball_pos);

    } else if (state == 1) {
        target_vel[0] = 0;
        target_vel[1] = 0;
        if (sqrt(pow(pos[0] - ball_pos[0], 2) + pow(pos[1] - ball_pos[1], 2)) > radius*1.3) {
            state = 0;
        }
    }
}


void Robot_controller::check_connection() {
    if (!detected) {
        offline_counter++;
    } else {
        offline_counter = 0;
    }

    if (offline_counter >= max_offline_counter) {
        stop();
    }
}


void Robot_controller::recive_vision() {
    detected = false;
    for (int i = 0; i < 16; i++) {
        allies[i].detected = false;
        enemies[i].detected = false;
    }
    for (auto blue_robot : han.new_vision.robots_blue) {
        if (team->color == 0) {
            int rb_id = blue_robot.robot_id;
            if (rb_id == id) {
                vel[0] = (pos[0] - blue_robot.position_x)/delta_time;
                vel[1] = (pos[1] - blue_robot.position_x)/delta_time;
                pos[0] = blue_robot.position_x;
                pos[1] = blue_robot.position_y;
                yaw = blue_robot.orientation;
                if (yaw < 0) yaw += 2*M_PI;
                detected = true;
                continue;
            }
            allies[rb_id].vel[0] = (blue_robot.position_x - allies[rb_id].pos[0])/delta_time;
            allies[rb_id].vel[1] = (blue_robot.position_y - allies[rb_id].pos[1])/delta_time;
            allies[rb_id].pos[0] = blue_robot.position_x;
            allies[rb_id].pos[1] = blue_robot.position_y;
            allies[rb_id].yaw = blue_robot.orientation;
            if (allies[rb_id].yaw < 0) allies[rb_id].yaw += 2*M_PI;
            allies[rb_id].detected = true;
        }
        else {
            int rb_id = blue_robot.robot_id;
            enemies[rb_id].vel[0] = (blue_robot.position_x - enemies[rb_id].pos[0])/delta_time;
            enemies[rb_id].vel[1] = (blue_robot.position_y - enemies[rb_id].pos[1])/delta_time;
            enemies[rb_id].pos[0] = blue_robot.position_x;
            enemies[rb_id].pos[1] = blue_robot.position_y;
            enemies[rb_id].yaw = blue_robot.orientation;
            if (enemies[rb_id].yaw < 0) enemies[rb_id].yaw += 2*M_PI;
            enemies[rb_id].detected = true;
        }
    }


    for (auto yellow_robot : han.new_vision.robots_yellow) {
        if (team->color == 1) {
            int rb_id = yellow_robot.robot_id;
            if (rb_id == id) {
                vel[0] = (pos[0] - yellow_robot.position_x)/delta_time;
                vel[1] = (pos[1] - yellow_robot.position_x)/delta_time;
                pos[0] = yellow_robot.position_x;
                pos[1] = yellow_robot.position_y;
                yaw = yellow_robot.orientation;
                if (yaw < 0) yaw += 2*M_PI;
                detected = true;
                continue;
            }
            allies[rb_id].vel[0] = (yellow_robot.position_x - allies[rb_id].pos[0])/delta_time;
            allies[rb_id].vel[1] = (yellow_robot.position_y - allies[rb_id].pos[1])/delta_time;
            allies[rb_id].pos[0] = yellow_robot.position_x;
            allies[rb_id].pos[1] = yellow_robot.position_y;
            allies[rb_id].yaw = yellow_robot.orientation;
            if (allies[rb_id].yaw < 0) allies[rb_id].yaw += 2*M_PI;
            allies[rb_id].detected = true;
        }
        else {
            int rb_id = yellow_robot.robot_id;
            enemies[rb_id].vel[0] = (yellow_robot.position_x - enemies[rb_id].pos[0])/delta_time;
            enemies[rb_id].vel[1] = (yellow_robot.position_y - enemies[rb_id].pos[1])/delta_time;
            enemies[rb_id].pos[0] = yellow_robot.position_x;
            enemies[rb_id].pos[1] = yellow_robot.position_y;
            enemies[rb_id].yaw = yellow_robot.orientation;
            if (enemies[rb_id].yaw < 0) enemies[rb_id].yaw += 2*M_PI;
            enemies[rb_id].detected = true;
        }
    }
    ball_pos[0] = han.new_vision.balls.position_x;
    ball_pos[1] = han.new_vision.balls.position_y;
}





void Robot_controller::publish() {
    //std::cout << pos[0] << ", " << pos[1] << std::endl;
    //std::cout << id << std::endl;
    //std::cout << target_vel[0] << " " << target_vel[1] << std::endl;
    han.new_ia.robots[id].vel_normal = target_vel[1];
    han.new_ia.robots[id].vel_tang = target_vel[0];
    han.new_ia.robots[id].vel_ang = target_vyaw;
    han.lc->publish("IA", &han.new_ia);
    han.new_tartarus.estrategia = 2;
    han.lc->publish("tartarus", &han.new_tartarus);

}
