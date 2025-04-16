//
// Created by caiu on 16/04/25.
//

#include "play_debug.h"

int play_debug::score(team_info team, robot allies[16]) {
    int score = 0;
    if (team.debug) {
        score += 999991;
    }
    return score;
}

std::vector<int> play_debug::role_assing(int (&active_robots)[16], team_info& team, robot allies[16], std::vector<int> roles) {
    int num_active_robots = 0;
    std::vector<int> active_allies_ids = {};

    for (int i = 0 ; i < 16 ; i++) {
        if (active_robots[i] == 1) {
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
    for (int role : required_roles) {
        if (active_allies_ids.empty()) {
            return roles;
        }
        if (role == 990) {
            roles[active_allies_ids[0]] = 990;
            active_allies_ids.erase(active_allies_ids.begin() + team.goal_keeper_id);
        }
        if (role == 991) {
            roles[active_allies_ids[0]] = 991;
            active_allies_ids.erase(active_allies_ids.begin() + team.goal_keeper_id);
        }
    }
    return roles;
}
