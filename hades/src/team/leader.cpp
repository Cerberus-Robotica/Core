//
// Created by caiom on 4/4/2025.
//

#include "leader.h"
#include <iostream>
#include <thread>
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


        recive_vision();

        std::cout << std::endl << "[";
        for (int i = 0; i < 16 ; i++) {
            if (team.active_robots[i] == 1) {
                std::cout << i << ", ";
            }
        }
        std::cout << "]" << std::endl;

        for (int i = 0; i < 16; i++) {
            //se robo for detectado e nao estava no vetor active_robots
            //add_robot(i);
        }
    }

}

void leader::recive_vision() {
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
        }
        else {
            int rb_id = blue_robot.robot_id;
            enemies[rb_id].pos[0] = blue_robot.position_x;
            enemies[rb_id].pos[1] = blue_robot.position_y;
            enemies[rb_id].yaw = blue_robot.orientation;
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
}

void leader::add_robot(int id) {
    if (team.active_robots[id] == 0) {
        team.active_robots[id] = 1;
        robots[id].start(&team);
    }
}