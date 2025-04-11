//
// Created by caiu on 11/04/25.
//

#ifndef WORLD_H
#define WORLD_H
#include <vector>

#include "../../c_trajectory/geometry/rectangle.h"

class world {
public:
    double their_goal[2][2] = {{6000, 6000}, {1000, -1000}};
    std::vector<rectangle> field_limits = {};
    rectangle their_defense_area = rectangle({4200, -1800}, {6000, 1800});
    std::vector<double> kicking_position(std::vector<double> pos_0, std::vector<double> pos_1, double distance);
};



#endif //WORLD_H
