//
// Created by caiom on 4/4/2025.
//

#include "leader.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include <math.h>
#include <numeric>
#include <unordered_set>

#include "../include/handlers.hpp"
#include "team_info.h"

void leader::start() {
    loop();
}

void leader::loop() {
    std::cout << "ASDASDASD";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto t0 = std::chrono::steady_clock::now();
    receive_vision();
    delta_time = 0.016;

    while (true) {
        if (last_time_stamp == han.new_ia.timestamp) {
            continue;
        }
        auto t1 = std::chrono::steady_clock::now();

        receive_config();
        receive_vision();
        select_plays();
        //imprimir_ativos();

        std::chrono::duration<double> delta = t1 - t0;
        t0 = std::chrono::steady_clock::now();
        delta_time = delta.count();
        if (delta_time == 0 or delta_time < 0) {
            std::cout << "??" << std::endl;
            delta_time = 1/60;
        }

    }

}

void leader::receive_vision() {
    std::unordered_set<int> allies_detected = {};
    std::unordered_set<int> enemies_detected = {};

    for (auto blue_robot : han.new_vision.robots_blue) {
        if (team.color == 0) {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(world.allies)) {
                for (int i = size(world.allies); i <= rb_id; i++) {
                    world.allies.push_back(robot(i));
                }
            }
            if (team.active_robots[rb_id] == 0) {
                add_robot(rb_id);
                continue;
            }
            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.allies[rb_id].stored_speed_x.push_back((blue_robot.position_x - world.allies[rb_id].pos[0])/(delta_time*1000));
                world.allies[rb_id].stored_speed_y.push_back((blue_robot.position_y - world.allies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.allies[rb_id].stored_speed_x) > 10) {
                    world.allies[rb_id].stored_speed_x.pop_front();
                    world.allies[rb_id].stored_speed_y.pop_front();
                }
                world.allies[rb_id].vel[0] = std::accumulate(world.allies[rb_id].stored_speed_x.begin(), world.allies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.allies[rb_id].vel[1] = std::accumulate(world.allies[rb_id].stored_speed_y.begin(), world.allies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.allies[rb_id].yaw = new_yaw;
            world.allies[rb_id].pos[0] = blue_robot.position_x;
            world.allies[rb_id].pos[1] = blue_robot.position_y;
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(world.enemies)) {
                for (int i = size(world.enemies); i <= rb_id; i++) {
                    world.enemies.push_back(robot(i));
                }
            }

            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.enemies[rb_id].stored_speed_x.push_back((blue_robot.position_x - world.enemies[rb_id].pos[0])/(delta_time*1000));
                world.enemies[rb_id].stored_speed_y.push_back((blue_robot.position_y - world.enemies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.enemies[rb_id].stored_speed_x) > 10) {
                    world.enemies[rb_id].stored_speed_x.pop_front();
                    world.enemies[rb_id].stored_speed_y.pop_front();
                }
                world.enemies[rb_id].vel[0] = std::accumulate(world.enemies[rb_id].stored_speed_x.begin(), world.enemies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.enemies[rb_id].vel[1] = std::accumulate(world.enemies[rb_id].stored_speed_y.begin(), world.enemies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.enemies[rb_id].yaw = new_yaw;
            world.enemies[rb_id].pos[0] = blue_robot.position_x;
            world.enemies[rb_id].pos[1] = blue_robot.position_y;
            enemies_detected.insert(rb_id);
        }
    }


    for (auto yellow_robot : han.new_vision.robots_yellow) {
        if (team.color == 1) {
            int rb_id = yellow_robot.robot_id;
            if (rb_id >= size(world.allies)) {
                for (int i = size(world.allies); i <= rb_id; i++) {
                    world.allies.push_back(robot(i));
                }
            }
            if (team.active_robots[rb_id] == 0) {
                add_robot(rb_id);
                continue;
            }
            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.allies[rb_id].stored_speed_x.push_back((yellow_robot.position_x - world.allies[rb_id].pos[0])/(delta_time*1000));
                world.allies[rb_id].stored_speed_y.push_back((yellow_robot.position_y - world.allies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.allies[rb_id].stored_speed_x) > 10) {
                    world.allies[rb_id].stored_speed_x.pop_front();
                    world.allies[rb_id].stored_speed_y.pop_front();
                }
                world.allies[rb_id].vel[0] = std::accumulate(world.allies[rb_id].stored_speed_x.begin(), world.allies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.allies[rb_id].vel[1] = std::accumulate(world.allies[rb_id].stored_speed_y.begin(), world.allies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.allies[rb_id].yaw = new_yaw;
            world.allies[rb_id].pos[0] = yellow_robot.position_x;
            world.allies[rb_id].pos[1] = yellow_robot.position_y;
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = yellow_robot.robot_id;
            if (rb_id >= size(world.enemies)) {
                for (int i = size(world.enemies); i <= rb_id; i++) {
                    world.enemies.push_back(robot(i));
                }
            }
            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.enemies[rb_id].stored_speed_x.push_back((yellow_robot.position_x - world.enemies[rb_id].pos[0])/(delta_time*1000));
                world.enemies[rb_id].stored_speed_y.push_back((yellow_robot.position_y - world.enemies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.enemies[rb_id].stored_speed_x) > 10) {
                    world.enemies[rb_id].stored_speed_x.pop_front();
                    world.enemies[rb_id].stored_speed_y.pop_front();
                }
                world.enemies[rb_id].vel[0] = std::accumulate(world.enemies[rb_id].stored_speed_x.begin(), world.enemies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.enemies[rb_id].vel[1] = std::accumulate(world.enemies[rb_id].stored_speed_y.begin(), world.enemies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.enemies[rb_id].yaw = new_yaw;
            world.enemies[rb_id].pos[0] = yellow_robot.position_x;
            world.enemies[rb_id].pos[1] = yellow_robot.position_y;
            enemies_detected.insert(rb_id);
        }
    }

    for (int i = 0; i < size(world.allies); i++) {
        if (allies_detected.find(i) != allies_detected.end()) world.allies[i].detected = true;
        else world.allies[i].detected = false;
    }

    for (int i = 0; i < size(world.enemies); i++) {
        if (enemies_detected.find(i) != enemies_detected.end()) world.enemies[i].detected = true;
        else world.enemies[i].detected = false;
    }

    if (delta_time != 0) {
        world.ball_speed[0] = (han.new_vision.balls.position_x - world.ball_pos[0])/(delta_time*1000);
        world.ball_speed[1] = (han.new_vision.balls.position_y - world.ball_pos[1])/(delta_time*1000);
    }
    world.ball_pos[0] = han.new_vision.balls.position_x;
    world.ball_pos[1] = han.new_vision.balls.position_y;

    last_time_stamp = han.new_vision.timestamp;
}

void leader::receive_config() {
    int is_team_blue = int(han.new_tartarus.team_blue);
    if (is_team_blue == 1) {
        team.color = 0;
    }
    else if (is_team_blue == 0) {
        team.color = 1;
    }

}

void leader::add_robot(int id) {
    if (id >= sizeof(team.active_robots)) {
        return;
    }
    if (team.active_robots[id] == 0) {
        team.active_robots[id] = 1;
        robots[id].start(&team);
    }
}

void leader::select_plays() {
    int num_active = 0;
    int plays_score[4] = {0, 0, 0, 0};
    int plays[4] = {0, 1, 2, 99};
    int num_plays = 4;

    for (int id : team.active_robots) {
        if (id) {
            num_active++;
        }
    }

    plays_score[0] = goal_keeper.score(world, team);
    plays_score[1] = attack.score(world, team);
    plays_score[2] = -2;
    plays_score[3] = debug.score(world, team);

    //std::cout << plays_score[0] << ", " << plays_score[1] << ", " << plays_score[2] << std::endl;


    for (int i = 0; i < num_plays - 1; i++) {
        // Flag para detectar se houve troca
        bool swapped = false;

        for (int j = 0; j < num_plays - i - 1; j++) {
            // Se o elemento atual é maior que o próximo, troca
            if (plays_score[j] < plays_score[j + 1]) {
                std::swap(plays_score[j], plays_score[j + 1]);
                std::swap(plays[j], plays[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
    int robot_id = 0;


    std::vector<int> roles;
    roles.reserve(16);
    for (int i = 0; i < 16; i++) {
        roles.push_back(-1);
    }

    for (int play : plays) {

        if (play == 0) {
            roles = goal_keeper.role_assing(world, team, roles);
        }

        if (play == 1) {
            roles = attack.role_assing(world, team, roles);
        }

        if (play == 99) {
            roles = debug.role_assing(world, team, roles);
        }

    }
    for (int i = 0 ; i < 16 ; i++) {
        team.roles[i] = roles[i];
    }
}






void leader::imprimir_ativos() {
    std::cout << std::endl << "[";
    for (int i = 0; i < 16 ; i++) {
        if (team.active_robots[i] == 1) {
            std::cout << i << ", ";
        }
    }
    std::cout << "]" << std::endl;
}