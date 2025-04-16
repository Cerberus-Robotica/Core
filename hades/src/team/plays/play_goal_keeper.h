//
// Created by caiu on 16/04/25.
//

#ifndef PLAY_GOAL_KEEPER_H
#define PLAY_GOAL_KEEPER_H
#include "../geometry/world.h"
#include "../robot.h"



class play_goal_keeper {
public:
    int score(world field, robot allies[16], robot enemies[16], double ball_pos[2], team_info team);
    std::vector<int> role_assing(int (&active_robots)[16], double ball_pos[2], team_info& team, robot allies[16], robot enemies[16], std::vector<int> roles);

private:
    int required_robots = 1;
    int required_roles[1] = {0};
};



#endif //PLAY_GOAL_KEEPER_H
