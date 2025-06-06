//
// Created by caiu on 16/04/25.
//

#ifndef PLAY_DEBUG_H
#define PLAY_DEBUG_H
#include "../geometry/WorldModel.h"
#include "../Robot.h"


class PlayDebug {
public:
    int score(WorldModel world, TeamInfo team);
    std::vector<TeamInfo::role> role_assing(WorldModel& world, TeamInfo& team, std::vector<TeamInfo::role> roles);

private:
    int required_robots = 16;
    TeamInfo::role required_roles[16] = {TeamInfo::debug_circular_trajectory, TeamInfo::debug_squared_trajectory,
        TeamInfo::debug_squared_trajectory, TeamInfo::debug_squared_trajectory,
        TeamInfo::debug_squared_trajectory, TeamInfo::debug_squared_trajectory,
        TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory,
        TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory,
        TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory,
        TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory,
        TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory};
};


#endif //PLAY_DEBUG_H
