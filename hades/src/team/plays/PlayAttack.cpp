//
// Created by caiu on 15/04/25.
//

#include "PlayAttack.h"

#include <iostream>
#include <math.h>

int PlayAttack::calc_score(WorldModel world, TeamInfo team) {
    int score = 0;
    if (world.ball.getPosition().getX() > 0 && team.our_side == TeamInfo::left) {
        score += 100;
    }
    if (world.ball.getPosition().getX() < 0 && team.our_side == TeamInfo::right) {
        score += 100;
    }
    this->score = score;
    return score;
}

std::array<Robot::role, 16> PlayAttack::role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) {
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
        return roles;
    }

    //role assign
    for (Robot::role selected_role : required_roles) {
        if (active_allies_ids.empty()) {
            return roles;
        }
        if (selected_role == Robot::goal_keeper) {
            if (!world.allies[team.goal_keeper_id].isDetected()) continue;
            roles[team.goal_keeper_id] = Robot::goal_keeper;
            active_allies_ids.erase(active_allies_ids.begin() + team.goal_keeper_id);
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + team.goal_keeper_id);

        }
        if (selected_role == Robot::striker) {
            int closest_idx = 0;
            for (int idx = 0; idx < active_allies_ids.size(); idx++) {
                if (distances_allies_from_ball[idx] < distances_allies_from_ball[closest_idx]) {
                    closest_idx = idx;
                }
            }
            int closest_id = active_allies_ids[closest_idx];
            roles[closest_id] = Robot::striker;
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + closest_idx);
            active_allies_ids.erase(active_allies_ids.begin() + closest_idx);
            //std::cout << "striker : " << closest_id << std::endl;
        }
        if (selected_role == Robot::mid_field) {
            int closest_idx = 0;
            if (world.ball.getVelocity().getNorm() < 0.1) {
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
            roles[closest_id] = Robot::mid_field;
            distances_allies_from_ball.erase(distances_allies_from_ball.begin() + closest_idx);
            active_allies_ids.erase(active_allies_ids.begin() + closest_idx);
            //std::cout << "midfield : " << closest_id << std::endl;
        }
    }

    return roles;
}