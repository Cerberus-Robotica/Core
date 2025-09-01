//
// Created by caiu on 15/04/25.
//

#include "PlayAttack.h"

#include <iostream>
#include <math.h>

int PlayAttack::calc_score(WorldModel world, TeamInfo team) {
    int score = 50;
    if (team.event == TeamInfo::run && world.ball.getPosition().getX() > 0 && team.our_side == TeamInfo::left) {
        score += 100;
    }
    if (team.event == TeamInfo::run && world.ball.getPosition().getX() < 0 && team.our_side == TeamInfo::right) {
        score += 100;
    }
    this->score = score;
    return score;
}

std::array<Robot::role, 16> PlayAttack::role_assign(WorldModel& world, TeamInfo& team, std::array<Robot::role, 16> roles) {
    std::vector<Robot*> avaiable_robots = {};
    for (int i = 0 ; i < std::size(team.active_robots) ; i++) {
        if (team.active_robots[i] == 1) {
            if (roles[i] != Robot::unknown) {
                continue;
            }
            avaiable_robots.push_back(&world.allies[i]);
        }
    }

    if (avaiable_robots.empty()) {
        return roles;
    }

    //role assign
    for (Robot::role selected_role : required_roles) {
        if (avaiable_robots.empty()) {
            return roles;
        }

        if (selected_role == Robot::goal_keeper) {
            if (!world.allies[team.goal_keeper_id].isDetected()) continue;
            avaiable_robots[team.goal_keeper_id]->setRole(Robot::goal_keeper);
            roles[team.goal_keeper_id] = Robot::goal_keeper;
            avaiable_robots.erase(avaiable_robots.begin() + team.goal_keeper_id);
        }

        if (selected_role == Robot::striker) {
            int closest_idx = 0;
            for (int idx = 0; idx < avaiable_robots.size(); idx++) {
                if (avaiable_robots[idx]->getPosition().getDistanceTo(world.ball.getPosition()) < avaiable_robots[closest_idx]->getPosition().getDistanceTo(world.ball.getPosition())) {
                    closest_idx = idx;
                }
            }
            int closest_id = avaiable_robots[closest_idx]->getId();
            avaiable_robots[closest_idx]->setRole(Robot::striker);
            roles[closest_id] = Robot::striker;
            avaiable_robots.erase(avaiable_robots.begin() + closest_idx);
        }

        if (selected_role == Robot::support) {
            int closest_idx = 0;
            if (world.ball.isStopped()) {
                for (int idx = 0; idx < avaiable_robots.size(); idx++) {
                    if (avaiable_robots[idx]->getPosition().getDistanceTo(world.ball.getPosition()) < avaiable_robots[closest_idx]->getPosition().getDistanceTo(world.ball.getPosition())) {
                        closest_idx = idx;
                    }
                }
            }
            else {
                Robot interceptor = world.getClosestAllyToPoint(world.ball.getStopPosition());
                for (int idx = 0; idx < avaiable_robots.size(); idx++) {
                    if (avaiable_robots[idx]->getId() == interceptor.getId()) {
                        closest_idx = idx;
                        break;
                    }
                }
            }
            int closest_id = avaiable_robots[closest_idx]->getId();
            avaiable_robots[closest_idx]->setRole(Robot::support);
            roles[closest_id] = Robot::support;
            avaiable_robots.erase(avaiable_robots.begin() + closest_idx);
        }
    }

    return roles;
}