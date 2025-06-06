//
// Created by caiu on 15/04/25.
//

#ifndef PLAY_H
#define PLAY_H
#include "../geometry/WorldModel.h"
#include "../Robot.h"

class PlayAttack {
public:
    int score(WorldModel world, TeamInfo team);
    std::vector<TeamInfo::role> role_assing(WorldModel& world, TeamInfo& team, std::vector<TeamInfo::role> roles);

private:
    int required_robots = 3;
    TeamInfo::role required_roles[3] = {TeamInfo::goal_keeper, TeamInfo::stricker, TeamInfo::mid_field_support};
};



#endif //PLAY_H
