//
// Created by caiu on 19/09/25.
//

#include "PlayOurFreeKick.h"

#include <iostream>
#include <ostream>

#include <team_info.hpp>


#include "../TeamInfo.h"

int PlayOurFreeKick::calc_score(WorldModel world, TeamInfo team) {
    int new_score = 0;
    if (team.event == TeamInfo::ourFreeKick) {
        new_score += 999992;
    }
    this->score = new_score;
    return score;
}

std::array<Robot::role, 16> PlayOurFreeKick::role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) {
    std::vector<Robot*> avaiable_robots = {};
    for (int i = 0 ; i < std::size(team.active_robots) ; i++) {
        if (team.active_robots[i] == 1) {
            if (roles[i] != Robot::unknown) {
                continue;
            }
            avaiable_robots.push_back(&world.allies[i]);
        }
    }

    if (avaiable_robots.empty()) {
        return roles;
    }

    //role assign
    for (Robot::role selected_role : required_roles) {
        if (avaiable_robots.empty()) {
            return roles;
        }

        if (selected_role == Robot::goal_keeper) {
            if (!world.allies[team.goal_keeper_id].isDetected()) continue;
            avaiable_robots[team.goal_keeper_id]->setRole(Robot::goal_keeper);
            roles[team.goal_keeper_id] = Robot::goal_keeper;
            avaiable_robots.erase(avaiable_robots.begin() + team.goal_keeper_id);
        }

        if (selected_role == Robot::freeKicker || selected_role == Robot::support) {
            int closest_idx = 0;
            for (int idx = 0; idx < avaiable_robots.size(); idx++) {
                if (avaiable_robots[idx]->getPosition().getDistanceTo(world.ball.getPosition()) < avaiable_robots[closest_idx]->getPosition().getDistanceTo(world.ball.getPosition())) {
                    closest_idx = idx;
                }
            }
            int closest_id = avaiable_robots[closest_idx]->getId();
            avaiable_robots[closest_idx]->setRole(selected_role);
            roles[closest_id] = selected_role;
            avaiable_robots.erase(avaiable_robots.begin() + closest_idx);
        }
    }

    return roles;
}