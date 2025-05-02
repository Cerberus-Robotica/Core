//
// Created by caiu on 16/04/25.
//

#include "play_goal_keeper.h"
#include <math.h>

int play_goal_keeper::score(world_model world, team_info team) {
    int score = 0;
    score += 99999;
    return score;
}

std::vector<int> play_goal_keeper::role_assing(world_model& world, team_info team, std::vector<int> roles) {
    int num_active_robots = 0;
    std::vector<int> active_allies_ids = {};
    std::vector<int> active_enemies_ids = {};
    std::vector<double> distances_allies_from_ball = {};
    std::vector<double> distances_enemies_from_ball = {};
    for (int i = 0 ; i < std::size(team.active_robots) ; i++) {
        if (team.active_robots[i] == 1) {
            if (roles[i] != -1) {
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

    for (int i = 0; i < size(world.enemies); i++) {
        if (world.enemies[i].detected) {
            active_enemies_ids.push_back(i);
            distances_enemies_from_ball.push_back(sqrt(pow(world.enemies[i].pos[0] - world.ball_pos[0],2) + pow(world.enemies[i].pos[1] - world.ball_pos[1],2)));
        }
    }

    //role assign
    for (int role : required_roles) {
        if (active_allies_ids.empty()) {
            return roles;
        }
        if (role == 0) {
            roles[team.goal_keeper_id] = 0;
            active_allies_ids.erase(active_allies_ids.begin() + team.goal_keeper_id);
        }
    }
    return roles;
}
