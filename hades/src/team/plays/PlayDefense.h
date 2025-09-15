//
// Created by caiu on 15/04/25.
//

#ifndef PLAY_DEFENSE_H
#define PLAY_DEFENSE_H
#include "../geometry/WorldModel.h"
#include "../Robot.h"
#include "PlayBase.h"


class PlayDefense : public PlayBase{
    std::array<Robot::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) override;
    int calc_score(WorldModel world, TeamInfo team) override;
public:
    PlayDefense() {
        name = "defense";
        required_robots = 3;
        required_roles = {Robot::goal_keeper, Robot::striker, Robot::defender};
    }
};



#endif //PLAY_DEFENSE_H
