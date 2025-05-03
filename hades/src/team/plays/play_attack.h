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
    int required_robots = 3;
    int required_roles[3] = {1, 2, 2};
};



#endif //PLAY_H
