//
// Created by caiu on 07/08/25.
//

#ifndef PLAYHALT_H
#define PLAYHALT_H

#include "../geometry/WorldModel.h"
#include "../Robot.h"


class PlayHalt {
public:
    int score(WorldModel world, TeamInfo team);
    std::vector<TeamInfo::role> role_assing(WorldModel& world, TeamInfo& team, std::vector<TeamInfo::role> roles);

private:
    int required_robots = 16;
    TeamInfo::role required_roles[16] = {TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted
    , TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted
    , TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted};
};



#endif //PLAYHALT_H
