//
// Created by caiom on 4/4/2025.
//

#ifndef LEADER_H
#define LEADER_H
#include "RobotController.h"
#include <vector>
#include "geometry/WorldModel.h"
#include "TeamInfo.h"
#include "plays/PlayAttack.h"
#include "plays/PlayDebug.h"
#include "plays/PlayHalt.h"
#include "plays/PlayOurKickOff.h"

class Leader {
public:
    TeamInfo team;



    double ball_pos[2] = {0, 0};
    int64_t last_time_stamp = 0;
    double delta_time = 0;

    PlayAttack attack;
    PlayDebug debug;
    PlayHalt halt;
    PlayOurKickOff ourKickOff;

    std::vector<PlayBase*> plays = {&attack, &debug, &halt, &ourKickOff};


    WorldModel world;




    void start();
    void stop();

private:
    void loop();
    void receive_vision();
    void receive_field_geometry();
    void receive_config();
    void receive_gamecontroller();
    void add_robot(int id);
    void imprimir_ativos();
    void select_plays();
    void inspect_enemy_team();
    void world_analysis();
};



#endif //LEADER_H
