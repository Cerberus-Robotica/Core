//
// Created by caiu on 18/09/25.
//

#include "PlayTheirKickOff.h"

#include <iostream>
#include <ostream>

#include "PlayHalt.h"

#include <team_info.hpp>


#include "../TeamInfo.h"

int PlayTheirKickOff::calc_score(WorldModel world, TeamInfo team) {
    int new_score = 0;
    if (team.event == TeamInfo::theirKickOff || team.event == TeamInfo::prepareTheirKickOff) {
        new_score += 999992;
    }
    this->score = new_score;
    return score;
}

std::array<Robot::role, 16> PlayTheirKickOff::role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) {
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
        if (selected_role == Robot::goal_keeper) {
            if (!world.allies[team.goal_keeper_id].isDetected()) continue;
            roles[team.goal_keeper_id] = Robot::kickoff_goal_keeper;
            active_allies_ids.erase(active_allies_ids.begin() + team.goal_keeper_id);
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + team.goal_keeper_id);
        }
        if (selected_role == Robot::kickoff_kicker || selected_role == Robot::kickoff_support) { //Mais perto da bola
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
        if (selected_role == Robot::defender) { //Mais longe da bola
            int furthest_idx = 0;
            for (int idx = 0; idx < active_allies_ids.size(); idx++) {
                if (distances_allies_from_ball[idx] > distances_allies_from_ball[furthest_idx]) {
                    furthest_idx = idx;
                }
            }
            int furthest_id = active_allies_ids[furthest_idx];
            roles[furthest_id] = selected_role;
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + furthest_idx);
            active_allies_ids.erase(active_allies_ids.begin() + furthest_idx);
        }

    }

    return roles;
}