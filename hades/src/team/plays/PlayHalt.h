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
    std::array<TeamInfo::role, 16> role_assing(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles);

private:
    int required_robots = 16;
    std::array<TeamInfo::role, 16> required_roles = {TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted
    , TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted
    , TeamInfo::halted, TeamInfo::halted, TeamInfo::halted, TeamInfo::halted};
};



#endif //PLAYHALT_H
