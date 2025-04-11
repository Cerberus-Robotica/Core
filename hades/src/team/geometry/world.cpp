//
// Created by caiu on 11/04/25.
//

#include "world.h"
#include "../c_trajectory/geometry/Vetop.h"

std::vector<double> kicking_position(std::vector<double> pos_0, std::vector<double> pos_1, double distance) {
    std::vector<double> pos_f;
    pos_f[0] = -pos_1[0] + pos_0[0];
    pos_f[1] = -pos_1[1] + pos_0[1];
    normalize(distance, pos_f);
    return pos_f;
}