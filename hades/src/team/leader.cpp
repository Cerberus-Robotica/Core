//
// Created by caiom on 4/4/2025.
//

#include "leader.h"
#include <iostream>
#include <thread>
#include "../include/handlers.hpp"

void leader::start() {


    loop();
}

void leader::loop() {

    for (int i = 0; i < 2; i++) {
        robots[i].start(&play);
    }
    while (true) {
        recive_vision();
        for (int i = 0; i < 16; i++) {
            //se robo for detectado e nao estava no vetor active_robots
            //add_robot(i);
        }
    }

}

void leader::recive_vision() {

    //std::cout << han.new_vision.field.field_length<< std::endl;
    //std::cout << han.new_vision.balls.position_x << ", " << han.new_vision.balls.position_y << std::endl;
}

void leader::add_robot(int id) {
    Robot_controller robo(id);
    robo.start(&play);
}