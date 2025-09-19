//
// Created by caiu on 19/09/25.
//

#ifndef PLAYTHEIRPENALTY_H
#define PLAYTHEIRPENALTY_H

#include "PlayBase.h"


class PlayTheirPenalty : public PlayBase {
	std::array<Robot::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) override;
	int calc_score(WorldModel world, TeamInfo team) override;
public:
	PlayTheirPenalty() {
		name = "theirPenalty";
		required_robots = 16;
		required_roles = {Robot::goal_keeper, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher};
	}
};



#endif //PLAYTHEIRPENALTY_H
