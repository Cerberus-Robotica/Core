//
// Created by caiu on 08/08/25.
//

#ifndef PLAYKICKOFF_H
#define PLAYKICKOFF_H

#include "../geometry/WorldModel.h"
#include "../Robot.h"


class PlayOurKickOff {
public:
    int score(WorldModel world, TeamInfo team);
    std::array<TeamInfo::role, 16> role_assing(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles);

private:
    int required_robots = 3;
    std::array<TeamInfo::role, 3> required_roles = {TeamInfo::kickoff_goal_keeper, TeamInfo::kickoff_kicker, TeamInfo::kickoff_support};

};



#endif //PLAYKICKOFF_H
