//
// Created by caiu on 16/04/25.
//

#ifndef PLAY_DEBUG_H
#define PLAY_DEBUG_H
#include "PlayBase.h"
#include "../geometry/WorldModel.h"
#include "../Robot.h"


class PlayDebug : public PlayBase {
public:
    std::array<TeamInfo::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles) override;
    int score(WorldModel world, TeamInfo team) override;
    PlayDebug(){
        required_robots = 16;
        required_roles = {TeamInfo::debug_circular_trajectory, TeamInfo::debug_squared_trajectory,
            TeamInfo::debug_squared_trajectory, TeamInfo::debug_squared_trajectory,
            TeamInfo::debug_squared_trajectory, TeamInfo::debug_squared_trajectory,
            TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory,
            TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory,
            TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory,
            TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory,
            TeamInfo::debug_squared_trajectory,TeamInfo::debug_squared_trajectory};}
};


#endif //PLAY_DEBUG_H
