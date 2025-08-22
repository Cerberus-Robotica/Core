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
    std::array<Robot::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) override;
    int calc_score(WorldModel world, TeamInfo team) override;

    PlayOurKickOff(){
        name = "OurKickOff";
        required_robots = 3;
        required_roles = {Robot::kickoff_goal_keeper, Robot::kickoff_kicker, Robot::kickoff_support};
    };
};



#endif //PLAYKICKOFF_H