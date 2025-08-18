//
// Created by caiu on 18/08/25.
//

#ifndef PLAYBASE_H
#define PLAYBASE_H
#include "../geometry/WorldModel.h"
#include "../TeamInfo.h"

class PlayBase {
public:
    virtual int score(WorldModel world, TeamInfo team) = 0;
    virtual std::array<TeamInfo::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles) = 0;

protected:
    int required_robots;
    std::vector<TeamInfo::role> required_roles;
};



#endif //PLAYBASE_H
