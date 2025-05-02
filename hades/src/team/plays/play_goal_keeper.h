//
// Created by caiu on 16/04/25.
//

#ifndef PLAY_GOAL_KEEPER_H
#define PLAY_GOAL_KEEPER_H
#include "../geometry/world_model.h"
#include "../robot.h"



class play_goal_keeper {
public:
    int score(world_model field, team_info team);
    std::vector<int> role_assing(world_model& world, team_info team, std::vector<int> roles);

private:
    int required_robots = 1;
    int required_roles[1] = {0};
};



#endif //PLAY_GOAL_KEEPER_H
