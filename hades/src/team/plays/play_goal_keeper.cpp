//
// Created by caiu on 16/04/25.
//

#include "play_goal_keeper.h"
#include <math.h>

int play_goal_keeper::score(world field, robot allies[16], robot enemies[16], double ball_pos[2], team_info team) {
    int score = 0;
    score += 99999;


    return score;
}

std::vector<int> play_goal_keeper::role_assing(int (&active_robots)[16], double ball_pos[2], team_info& team, robot allies[16], robot enemies[16], std::vector<int> roles) {
    int num_active_robots = 0;
    std::vector<int> active_allies_ids = {};
    std::vector<int> active_enemies_ids = {};
    std::vector<double> distances_allies_from_ball = {};
    std::vector<double> distances_enemies_from_ball = {};
    for (int i = 0 ; i < 16 ; i++) {
        if (active_robots[i] == 1) {
            if (roles[i] != -1) {
                continue;
            }
            num_active_robots++;
            active_allies_ids.push_back(i);
            distances_allies_from_ball.push_back(sqrt(pow(allies[i].pos[0] - ball_pos[0],2) + pow(allies[i].pos[1] - ball_pos[1],2)));
        }
    }
    if (active_allies_ids.empty()) {
        return roles;  // ou faça algo apropriado, como um log de erro
    }

    for (int i = 0; i < 16; i++) {
        if (enemies[i].detected) {
            active_enemies_ids.push_back(i);
            distances_enemies_from_ball.push_back(sqrt(pow(enemies[i].pos[0] - ball_pos[0],2) + pow(enemies[i].pos[1] - ball_pos[1],2)));
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
