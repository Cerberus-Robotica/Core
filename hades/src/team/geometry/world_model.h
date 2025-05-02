//
// Created by caiu on 11/04/25.
//

#ifndef WORLD_H
#define WORLD_H
#include <vector>

#include "../../c_trajectory/geometry/rectangle.h"
#include "../team_info.h"
#include "../robot.h"

class world_model {
public:
    double field_size[2][2] = {{-6000, -4500}, {6000, 4500}};

    double their_goal[2][2] = {{6000, 6000}, {1000, -1000}};
    double our_goal[2][2] = {{-6000, -6000}, {1000, -1000}};
    double our_defese_area[2][2] = {{-6000, -1750}, {-4200, 1750}};
    double their_defese_area[2][2] = {{4200, -1750}, {6000, 1750}};

    double inside_our_goal[2][2] = {{-6300, -1000}, {-6000, 1000}};


    std::vector<robot> allies = {};
    std::vector<robot> enemies = {};
    double ball_pos[2] = {0, 0};
    double ball_speed[2] = {0, 0};
    double ball_speed_module = 0;



    //std::vector<rectangle> field_limits = {};
    //rectangle their_defense_area = rectangle({4200, -1800}, {6000, 1800});

    std::vector<std::vector<double>> support_areas();
    std::vector<double> kicking_position(std::vector<double> pos_0, std::vector<double> pos_1, double distance);
    bool ball_on_our_side();
    bool ball_on_our_area();
    bool ball_on_their_area();
};



#endif //WORLD_H
