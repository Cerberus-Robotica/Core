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
    std::array<Robot::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) override;
    int calc_score(WorldModel world, TeamInfo team) override;
    PlayDebug(){
        name = "debug";
        required_robots = 16;
        required_roles = {Robot::debug_circular_trajectory, Robot::debug_squared_trajectory, Robot::debug_squared_trajectory,
        Robot::debug_squared_trajectory,Robot::debug_squared_trajectory,Robot::debug_squared_trajectory,Robot::debug_squared_trajectory,
        Robot::debug_squared_trajectory,Robot::debug_squared_trajectory,Robot::debug_squared_trajectory,Robot::debug_squared_trajectory,
        Robot::debug_squared_trajectory,Robot::debug_squared_trajectory,Robot::debug_squared_trajectory,Robot::debug_squared_trajectory,
        Robot::debug_squared_trajectory,};}
};


#endif //PLAY_DEBUG_H
