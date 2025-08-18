//
// Created by caiu on 18/08/25.
//

#ifndef PLAYBASE_H
#define PLAYBASE_H
#include <string>

#include "../geometry/WorldModel.h"
#include "../TeamInfo.h"

class PlayBase {
public:
    virtual int calc_score(WorldModel world, TeamInfo team) = 0;
    virtual std::array<TeamInfo::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles) = 0;
    int get_score();

protected:
    std::string name;
    int score = 0;
    int required_robots;
    std::vector<TeamInfo::role> required_roles;
};



#endif //PLAYBASE_H
