//
// Created by caiu on 07/08/25.
//

#include "PlayHalt.h"

#include <team_info.hpp>


#include "../TeamInfo.h"

int PlayHalt::calc_score(WorldModel world, TeamInfo team) {
    int score = 0;
    if (team.current_command == TeamInfo::halt) {
        score += 999992;
    }
    this->score = score;
    return score;
}

std::array<TeamInfo::role, 16> PlayHalt::role_assign(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles) {
    int num_active_robots = 0;
    std::vector<int> active_allies_ids = {};

    for (int i = 0 ; i < std::size(team.active_robots) ; i++) {
        if (team.active_robots[i] == 1) {
            if (roles[i] != -1) {
                continue;
            }
            num_active_robots++;
            active_allies_ids.push_back(i);
        }
    }
    if (active_allies_ids.empty()) {
        return roles;  // ou faça algo apropriado, como um log de erro
    }

    //role assign
    for (TeamInfo::role selected_role : required_roles) {
        if (active_allies_ids.empty()) {
            return roles;
        }
        roles[active_allies_ids[0]] = selected_role;
        active_allies_ids.erase(active_allies_ids.begin() + team.goal_keeper_id);
    }

    return roles;
}