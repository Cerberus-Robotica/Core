//
// Created by caiu on 19/09/25.
//

#ifndef PLAYOURFREEKICK_H
#define PLAYOURFREEKICK_H

#include "PlayBase.h"

class PlayOurFreeKick : public PlayBase {
	std::array<Robot::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) override;
	int calc_score(WorldModel world, TeamInfo team) override;
public:
	PlayOurFreeKick() {
		name = "ourFreeKick";
		required_robots = 3;
		required_roles = {Robot::goal_keeper, Robot::freeKicker, Robot::support};
        }
};



#endif //PLAYOURFREEKICK_H
