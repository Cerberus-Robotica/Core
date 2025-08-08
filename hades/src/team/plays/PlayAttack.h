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
    std::array<TeamInfo::role, 16> role_assing(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles);

private:
    int required_robots = 3;
    std::array<TeamInfo::role, 3> required_roles = {TeamInfo::goal_keeper, TeamInfo::mid_field, TeamInfo::striker};
};



#endif //PLAY_H
