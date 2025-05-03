//
// Created by caiu on 11/04/25.
//

#include "world_model.h"

#include <iostream>
#include <ostream>
#include <cmath>

#include "../c_trajectory/geometry/Vetop.h"

std::vector<double> world_model::kicking_position(std::vector<double> pos_0, std::vector<double> pos_1, double distance) {
    std::vector<double> pos_f = {0, 0};
    pos_f[0] = -pos_1[0] + pos_0[0];
    pos_f[1] = -pos_1[1] + pos_0[1];
    pos_f = normalize(distance, pos_f);
    pos_f[0] += pos_0[0];
    pos_f[1] += pos_0[1];
    return pos_f;
}

bool world_model::ball_on_our_side() {
    if (ball_pos[0] == 0) {
        return false;
    }
    else if (ball_pos[0]/fabs(ball_pos[0]) == our_goal[0][0]/fabs(our_goal[0][0])) {
        return true;
    }
    return false;
}

bool world_model::ball_on_our_area() {
    if (ball_pos[0] > our_defese_area[0][0] && ball_pos[0] < our_defese_area[1][0]) {
        if (ball_pos[1] > our_defese_area[0][1] && ball_pos[1] < our_defese_area[1][1]) {
            return true;
        }
    }
    return false;
}

bool world_model::ball_on_their_area() {
    if (ball_pos[0] > their_defese_area[0][0] && ball_pos[0] < their_defese_area[1][0]) {
        if (ball_pos[1] > their_defese_area[0][1] && ball_pos[1] < their_defese_area[1][1]) {
            return true;
        }
    }
    return false;
}



void world_model::generate_support_areas(double goal[2], double kick_distance) {
    kick_distance = std::min(kick_distance,  sqrt(pow(ball_pos[0] - goal[0], 2) + pow(ball_pos[1] - goal[1], 2)/2));
    int num_of_support_areas = 16;
    std::vector<std::vector<double>> support_areas = {};
    std::vector<double> heuristics = {};
    double heuristic = 0;

    for (int i = 0; i < num_of_support_areas; i++) {
        support_areas.push_back({ball_pos[0] + kick_distance*their_goal[0][0]/fabs(their_goal[0][0]), field_size[1][1]/2 - field_size[1][1]*i/num_of_support_areas});
        heuristic = 0;
        heuristic += sqrt(pow(ball_pos[0] - support_areas[i][0], 2) + pow(ball_pos[1] - support_areas[i][1], 2)) +
            sqrt(pow(goal[0] - support_areas[i][0], 2) + pow(goal[1] - support_areas[i][1], 2));
        heuristics.push_back(heuristic);
    }

    for (int i = 0 ; i < size(heuristics) ; i++) {
        for (int j = 0; j < size(heuristics) - i - 1; j++) {
            if (heuristics[j] > heuristics[j + 1]) {
                std::swap(heuristics[j], heuristics[j + 1]);
                std::swap(support_areas[j], support_areas[j + 1]);
            }
        }
    }
    //TODO organizar por prioridade com funcao heuristica
    this->support_areas = support_areas;
}