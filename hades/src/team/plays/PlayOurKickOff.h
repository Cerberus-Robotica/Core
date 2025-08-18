//
// Created by caiu on 08/08/25.
//

#ifndef PLAYKICKOFF_H
#define PLAYKICKOFF_H

#include "PlayBase.h"
#include "../geometry/WorldModel.h"
#include "../Robot.h"


class PlayOurKickOff : public PlayBase{
public:
    std::array<TeamInfo::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles) override;
    int score(WorldModel world, TeamInfo team) override;

    PlayOurKickOff(){
        required_robots = 3;
        required_roles = {TeamInfo::kickoff_goal_keeper, TeamInfo::kickoff_kicker, TeamInfo::kickoff_support};
    };
};



#endif //PLAYKICKOFF_H