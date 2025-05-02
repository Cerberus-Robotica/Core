//
// Created by caiom on 4/4/2025.
//

#include "leader.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include "../include/handlers.hpp"
#include "team_info.h"

void leader::start() {
    loop();
}

void leader::loop() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //for (int i = 0; i < 1; i++) {
    //    add_robot(i);
    //}
    while (true) {
        receive_config();
        receive_vision();
        select_plays();
        //imprimir_ativos();
        for (int i = 0; i < 16; i++) {
            //se robo for detectado e nao estava no vetor active_robots
            //add_robot(i);
        }
    }

}

void leader::receive_vision() {
    for (auto blue_robot : han.new_vision.robots_blue) {
        if (team.color == 0) {
            int rb_id = blue_robot.robot_id;
            if (team.active_robots[rb_id] == 0) {
                add_robot(rb_id);
                continue;
            }
            allies[rb_id].pos[0] = blue_robot.position_x;
            allies[rb_id].pos[1] = blue_robot.position_y;
            allies[rb_id].yaw = blue_robot.orientation;
            allies[rb_id].detected = true;
        }
        else {
            int rb_id = blue_robot.robot_id;
            enemies[rb_id].pos[0] = blue_robot.position_x;
            enemies[rb_id].pos[1] = blue_robot.position_y;
            enemies[rb_id].yaw = blue_robot.orientation;
            enemies[rb_id].detected = true;
        }
    }


    for (auto yellow_robot : han.new_vision.robots_yellow) {
        if (team.color == 1) {
            int rb_id = yellow_robot.robot_id;
            if (team.active_robots[rb_id] == 0) {
                add_robot(rb_id);
                continue;
            }
            allies[rb_id].pos[0] = yellow_robot.position_x;
            allies[rb_id].pos[1] = yellow_robot.position_y;
            allies[rb_id].yaw = yellow_robot.orientation;
        }
        else {
            int rb_id = yellow_robot.robot_id;
            enemies[rb_id].pos[0] = yellow_robot.position_x;
            enemies[rb_id].pos[1] = yellow_robot.position_y;
            enemies[rb_id].yaw = yellow_robot.orientation;
        }
    }
    ball_pos[0] = han.new_vision.balls.position_x;
    ball_pos[1] = han.new_vision.balls.position_y;
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

    plays_score[0] = goal_keeper.score(world, allies, enemies, ball_pos, team);
    plays_score[1] = attack.score(world, allies, enemies, ball_pos, team);
    plays_score[2] = -2;
    plays_score[3] = debug.score(team, allies);

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
            roles = goal_keeper.role_assing(team.active_robots, ball_pos, team, allies, enemies, roles);
        }

        if (play == 1) {
            roles = attack.role_assing(team.active_robots, ball_pos, team, allies, enemies, roles);
        }

        if (play == 99) {
            roles = debug.role_assing(team.active_robots, team, allies, roles);
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