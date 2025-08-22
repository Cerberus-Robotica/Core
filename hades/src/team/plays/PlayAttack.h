//
// Created by caiu on 15/04/25.
//

#ifndef PLAY_H
#define PLAY_H
#include "../geometry/WorldModel.h"
#include "../Robot.h"
#include "PlayBase.h"


class PlayAttack : public PlayBase{
    std::array<Robot::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) override;
    int calc_score(WorldModel world, TeamInfo team) override;
public:
    PlayAttack() {
        name = "attack";
        required_robots = 3;
        required_roles = {Robot::goal_keeper, Robot::mid_field, Robot::striker};
    }
};



#endif //PLAY_H
