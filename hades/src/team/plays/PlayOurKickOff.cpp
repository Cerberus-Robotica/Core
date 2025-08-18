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
    int score = 0;
    std::cout << team.current_command << std::endl;
    if ((team.current_command == TeamInfo::PREPARE_KICKOFF_BLUE)&&(team.color == TeamInfo::blue) || (team.current_command == TeamInfo::PREPARE_KICKOFF_YELLOW)&&(team.color == TeamInfo::yellow)) {
        score += 999992;
    }
    this->score = score;
    return score;
}

std::array<TeamInfo::role, 16> PlayOurKickOff::role_assign(WorldModel& world, TeamInfo& team, std::array<TeamInfo::role, 16> roles) {
    int num_active_robots = 0;
    std::vector<int> active_allies_ids = {};
    std::vector<double> distances_allies_from_ball = {};
    for (int i = 0 ; i < std::size(team.active_robots) ; i++) {
        if (team.active_robots[i] == 1) {
            if (roles[i] != TeamInfo::unknown) {
                continue;
            }
            num_active_robots++;
            active_allies_ids.push_back(i);
            distances_allies_from_ball.push_back(sqrt(pow(world.allies[i].pos[0] - world.ball_pos[0],2) + pow(world.allies[i].pos[1] - world.ball_pos[1],2)));
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
        if (selected_role == TeamInfo::goal_keeper) {
            if (!world.allies[team.goal_keeper_id].detected) continue;
            roles[team.goal_keeper_id] = TeamInfo::kickoff_goal_keeper;
            active_allies_ids.erase(active_allies_ids.begin() + team.goal_keeper_id);
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + team.goal_keeper_id);
        }
        if (selected_role == TeamInfo::kickoff_kicker || selected_role == TeamInfo::kickoff_support) {
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