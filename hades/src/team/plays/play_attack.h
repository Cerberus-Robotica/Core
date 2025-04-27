//
// Created by caiu on 15/04/25.
//

#ifndef PLAY_H
#define PLAY_H
#include "../geometry/world_model.h"
#include "../robot.h"

class play_attack {
public:
    int score(world_model field, robot allies[16], robot enemies[16], double ball_pos[2], team_info team);
    std::vector<int> role_assing(int active_robots[16], double ball_pos[2], team_info& team, robot allies[16], robot enemies[16], std::vector<int> roles);

private:
    int required_robots = 2;
    int required_roles[2] = {1, 1};
};



#endif //PLAY_H
