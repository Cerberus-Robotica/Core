//
// Created by caiu on 07/08/25.
//

#ifndef PLAYHALT_H
#define PLAYHALT_H

#include "../geometry/WorldModel.h"
#include "../Robot.h"
#include "PlayBase.h"

class PlayHalt : public PlayBase{
public:
    std::array<TeamInfo::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles) override;
    int calc_score(WorldModel world, TeamInfo team) override;

    PlayHalt() {
        name = "halt";
        required_robots = 16;
        required_roles = {TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted
        , TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted
        , TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted};
    }
};



#endif //PLAYHALT_H
