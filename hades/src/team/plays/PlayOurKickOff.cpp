//
// Created by caiu on 08/08/25.
//

#include "PlayOurKickOff.h"

#include <iostream>
#include <ostream>

#include "PlayHalt.h"

#include <team_info.hpp>


#include "../TeamInfo.h"

int PlayOurKickOff::calc_score(WorldModel world, TeamInfo team) {
    int new_score = 0;
    if (team.event == TeamInfo::ourKickOff || team.event == TeamInfo::prepareOurKickOff) {
        new_score += 999992;
    }
    this->score = new_score;
    return score;
}

std::array<Robot::role, 16> PlayOurKickOff::role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) {
    int num_active_robots = 0;
    std::vector<int> active_allies_ids = {};
    std::vector<double> distances_allies_from_center = {};
    for (int i = 0 ; i < std::size(team.active_robots) ; i++) {
        if (team.active_robots[i] == 1) {
            if (roles[i] != Robot::unknown) {
                continue;
            }
            num_active_robots++;
            active_allies_ids.push_back(i);
            distances_allies_from_center.push_back(world.allies[i].getPosition().getDistanceTo({0, 0}));
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
        if (selected_role == Robot::goal_keeper) {
            if (!world.allies[team.goal_keeper_id].isDetected()) continue;
            roles[team.goal_keeper_id] = Robot::kickoff_goal_keeper;
            active_allies_ids.erase(active_allies_ids.begin() + team.goal_keeper_id);
            distances_allies_from_center.erase(distances_allies_from_center.begin() + team.goal_keeper_id);
        }
        if (selected_role == Robot::kickoff_kicker || selected_role == Robot::kickoff_support) {
            int closest_idx = 0;
            for (int idx = 0; idx < active_allies_ids.size(); idx++) {
                if (distances_allies_from_center[idx] < distances_allies_from_center[closest_idx]) {
                    closest_idx = idx;
                }
            }
            int closest_id = active_allies_ids[closest_idx];
            roles[closest_id] = selected_role;
            distances_allies_from_center.erase(distances_allies_from_center.begin() + closest_idx);
            active_allies_ids.erase(active_allies_ids.begin() + closest_idx);
        }
    }

    return roles;
}