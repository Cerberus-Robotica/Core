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