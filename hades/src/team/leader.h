//
// Created by caiom on 4/4/2025.
//

#ifndef LEADER_H
#define LEADER_H
#include "Robot_controller.h"
#include <vector>
#include "geometry/world_model.h"
#include "team_info.h"
#include "plays/play_attack.h"
#include "plays/play_goal_keeper.h"
#include "plays/play_debug.h"

class leader {
public:
    team_info team;

    Robot_controller robots[16] = {
        Robot_controller(0), Robot_controller(1), Robot_controller(2), Robot_controller(3),
        Robot_controller(4), Robot_controller(5), Robot_controller(6), Robot_controller(7),
        Robot_controller(8), Robot_controller(9), Robot_controller(10), Robot_controller(11),
        Robot_controller(12), Robot_controller(13), Robot_controller(14), Robot_controller(15)
    };



    double ball_pos[2] = {0, 0};
    int64_t last_time_stamp = 0;
    double delta_time = 0;

    play_goal_keeper goal_keeper;
    play_attack attack;
    play_debug debug;

    world_model world;




    void start();
    void stop();

private:
    void loop();
    void receive_vision();
    void receive_config();
    void add_robot(int id);
    void imprimir_ativos();
    void select_plays();
    void inspect_enemy_team();
};



#endif //LEADER_H
