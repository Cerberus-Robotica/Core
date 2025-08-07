//
// Created by caiu on 15/04/25.
//

#include "PlayAttack.h"

#include <iostream>
#include <math.h>

int PlayAttack::score(WorldModel world, TeamInfo team) {
    int score = 0;
    if (world.ball_pos[0] > 0 && team.our_side == TeamInfo::left) {
        score += 100;
    }
    if (world.ball_pos[0] < 0 && team.our_side == TeamInfo::right) {
        score += 100;
    }
    return score;
}

std::vector<TeamInfo::role> PlayAttack::role_assing(WorldModel& world, TeamInfo& team, std::vector<TeamInfo::role> roles) {
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
        return roles;
    }

    //role assign
    for (TeamInfo::role selected_role : required_roles) {
        if (active_allies_ids.empty()) {
            return roles;
        }
        if (selected_role == TeamInfo::goal_keeper) {
            if (!world.allies[team.goal_keeper_id].detected) continue;
            roles[team.goal_keeper_id] = TeamInfo::goal_keeper;
            active_allies_ids.erase(active_allies_ids.begin() + team.goal_keeper_id);
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + team.goal_keeper_id);

        }
        if (selected_role == TeamInfo::striker) {
            int closest_idx = 0;
            for (int idx = 0; idx < active_allies_ids.size(); idx++) {
                if (distances_allies_from_ball[idx] < distances_allies_from_ball[closest_idx]) {
                    closest_idx = idx;
                }
            }
            int closest_id = active_allies_ids[closest_idx];
            roles[closest_id] = TeamInfo::striker;
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + closest_idx);
            active_allies_ids.erase(active_allies_ids.begin() + closest_idx);
            //std::cout << "striker : " << closest_id << std::endl;
        }
        if (selected_role == TeamInfo::mid_field) {
            int closest_idx = 0;
            if (world.ball_speed_module < 0.1) {
                for (int idx = 0; idx < active_allies_ids.size(); idx++) {
                    if (distances_allies_from_ball[idx] < distances_allies_from_ball[closest_idx]) {
                        closest_idx = idx;
                    }
                }
            }
            else {
                int interceptor = world.getIdOfTheBallInterceptor();
                if (interceptor < 20) {
                    for (int idx = 0; idx < active_allies_ids.size(); idx++) {
                        if (active_allies_ids[idx] == interceptor) {
                            closest_idx = idx;
                            break;
                        }
                    }
                }
            }
            int closest_id = active_allies_ids[closest_idx];
            roles[closest_id] = TeamInfo::mid_field;
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + closest_idx);
            active_allies_ids.erase(active_allies_ids.begin() + closest_idx);
            //std::cout << "midfield : " << closest_id << std::endl;
        }
    }

    return roles;
}