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



std::vector<std::vector<double>> world_model::support_areas() {
    double kick_distance = 500;
    int num_of_support_areas = 16;
    std::vector<std::vector<double>> support_areas = {};

    for (int i = 0; i < num_of_support_areas; i++) {
        support_areas.push_back({ball_pos[0] + kick_distance*their_goal[0][0]/fabs(their_goal[0][0]), field_size[1][1]/2 - field_size[1][1]*i/num_of_support_areas});
    }
    //TODO organizar por prioridade com funcao heuristica

    return support_areas;
}