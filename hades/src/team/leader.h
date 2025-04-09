//
// Created by caiom on 4/4/2025.
//

#ifndef LEADER_H
#define LEADER_H
#include "Robot_controller.h"
#include <vector>

class leader {
public:
    int active_robots[16];
    int play = -1;
    int game_state = -1;


    Robot_controller robots[16] = {
        Robot_controller(0), Robot_controller(1), Robot_controller(2), Robot_controller(3),
        Robot_controller(4), Robot_controller(5), Robot_controller(6), Robot_controller(7),
        Robot_controller(8), Robot_controller(9), Robot_controller(10), Robot_controller(11),
        Robot_controller(12), Robot_controller(13), Robot_controller(14), Robot_controller(15)
    };

    void start();
    void stop();

private:
    void loop();
    void recive_vision();
    void add_robot(int id);



};



#endif //LEADER_H
