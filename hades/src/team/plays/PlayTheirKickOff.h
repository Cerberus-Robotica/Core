//
// Created by caiu on 18/09/25.
//

#ifndef PLAYTHEIRKICKOFF_H
#define PLAYTHEIRKICKOFF_H
#include "../geometry/WorldModel.h"
#include "../Robot.h"
#include "PlayBase.h"


class PlayTheirKickOff : public PlayBase {
	std::array<Robot::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) override;
	int calc_score(WorldModel world, TeamInfo team) override;
public:
	PlayTheirKickOff() {
		name = "theirKickOff";
		required_robots = 3;
		required_roles = {Robot::goal_keeper, Robot::defender, Robot::kickoff_support};
	}
};



#endif //PLAYTHEIRKICKOFF_H
