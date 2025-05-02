//
// Created by caiu on 15/04/25.
//

#ifndef PLAY_H
#define PLAY_H
#include "../geometry/world_model.h"
#include "../robot.h"

class play_attack {
public:
    int score(world_model world, team_info team);
    std::vector<int> role_assing(world_model& world, team_info& team, std::vector<int> roles);

private:
    int required_robots = 2;
    int required_roles[2] = {1, 1};
};



#endif //PLAY_H
