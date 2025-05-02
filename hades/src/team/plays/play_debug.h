//
// Created by caiu on 16/04/25.
//

#ifndef PLAY_DEBUG_H
#define PLAY_DEBUG_H
#include "../geometry/world_model.h"
#include "../robot.h"


class play_debug {
public:
    int score(world_model world, team_info team);
    std::vector<int> role_assing(world_model& world, team_info& team, std::vector<int> roles);

private:
    int required_robots = 16;
    int required_roles[16] = {990, 991, 991, 991, 991, 991, 990, 990, 990, 990, 990, 990, 990, 990, 990, 990};
};



#endif //PLAY_DEBUG_H
