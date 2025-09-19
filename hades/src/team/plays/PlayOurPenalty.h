//
// Created by caiu on 19/09/25.
//

#ifndef PLAYOURPENALTY_H
#define PLAYOURPENALTY_H
#include "PlayBase.h"


class PlayOurPenalty : public PlayBase {
	std::array<Robot::role, 16> role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) override;
	int calc_score(WorldModel world, TeamInfo team) override;
public:
	PlayOurPenalty() {
		name = "ourPenalty";
		required_robots = 16;
		required_roles = {Robot::penaltier, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher, Robot::watcher};
		}
};



#endif //PLAYOURPENALTY_H
