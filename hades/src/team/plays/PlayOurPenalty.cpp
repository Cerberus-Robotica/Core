//
// Created by caiu on 19/09/25.
//

#include "PlayOurPenalty.h"

#include <iostream>
#include <ostream>

#include <team_info.hpp>


#include "../TeamInfo.h"

int PlayOurPenalty::calc_score(WorldModel world, TeamInfo team) {
    int new_score = 0;
    if (team.event == TeamInfo::ourPenalty || team.event == TeamInfo::runningOurPenalty) {
        new_score += 999992;
    }
    this->score = new_score;
    return score;
}

std::array<Robot::role, 16> PlayOurPenalty::role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) {
    int num_active_robots = 0;
    std::vector<int> active_allies_ids = {};
    std::vector<double> distances_allies_from_ball = {};
    for (int i = 0 ; i < std::size(team.active_robots) ; i++) {
        if (team.active_robots[i] == 1) {
            if (roles[i] != Robot::unknown) {
                continue;
            }
            num_active_robots++;
            active_allies_ids.push_back(i);
            distances_allies_from_ball.push_back(world.allies[i].getPosition().getDistanceTo(world.ball.getPosition()));
        }
    }

    if (active_allies_ids.empty()) {
        return roles;  // ou faça algo apropriado, como um log de erro
    }

    //role assign
    for (Robot::role selected_role : required_roles) {
        if (active_allies_ids.empty()) {
            return roles;
        }
        if (selected_role == Robot::halted) {
            if (roles[active_allies_ids[0]] == Robot::unknown) {
                roles[active_allies_ids[0]] = selected_role;
                active_allies_ids.erase(active_allies_ids.begin());
            }
        }
        if (selected_role == Robot::watcher) {
            if (roles[active_allies_ids[0]] == Robot::unknown) {
                roles[active_allies_ids[0]] = selected_role;
                active_allies_ids.erase(active_allies_ids.begin());
            }
        }
        if (selected_role == Robot::penaltier) { //Mais perto da bola
            int closest_idx = 0;
            for (int idx = 0; idx < active_allies_ids.size(); idx++) {
                if (distances_allies_from_ball[idx] < distances_allies_from_ball[closest_idx]) {
                    closest_idx = idx;
                }
            }
            int closest_id = active_allies_ids[closest_idx];
            roles[closest_id] = selected_role;
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + closest_idx);
            active_allies_ids.erase(active_allies_ids.begin() + closest_idx);
        }
    }
    return roles;
}