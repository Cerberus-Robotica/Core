//
// Created by caiu on 11/04/25.
//

#ifndef WORLD_H
#define WORLD_H
#include <vector>


class world {
public:
    double their_goal[2][2] = {{4000, 4000}, {1250, -1250}};
    std::vector<double> kicking_position(std::vector<double> pos_0, std::vector<double> pos_1, double distance);
};



#endif //WORLD_H
