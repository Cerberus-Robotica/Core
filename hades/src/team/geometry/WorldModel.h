//
// Created by caiu on 11/04/25.
//

#ifndef WORLD_H
#define WORLD_H
#include <vector>

#include "../../c_trajectory/geometry/Rectangle.h"
#include "../TeamInfo.h"
#include "../Robot.h"

class WorldModel {
public:
    double field_size[2][2] = {{-6000, -4500}, {6000, 4500}};

    double their_goal[2][2] = {{6000, 6000}, {1000, -1000}};
    double our_goal[2][2] = {{-6000, -6000}, {1000, -1000}};
    double our_defese_area[2][2] = {{-6000, -1750}, {-4200, 1750}};
    double their_defese_area[2][2] = {{4200, -1750}, {6000, 1750}};

    double back_fisical_left_goal[2][2] = {{-6300, -1000}, {-6200, 1000}}; //{-6300, -1000}, {-6000, 1000}};
    double left_fisical_left_goal[2][2] = {{-6100, -950}, {-6000, -900}}; //{-6300, -1000}, {-6000, 1000}};
    double right_fisical_left_goal[2][2] = {{-6100, 900}, {-6000, 950}}; //{-6300, -1000}, {-6000, 1000}};

    double back_fisical_right_goal[2][2] = {{6200, -1000}, {6300, 1000}}; //{-6300, -1000}, {-6000, 1000}};
    double left_fisical_right_goal[2][2] = {{6000, -950}, {6100, -900}}; //{-6300, -1000}, {-6000, 1000}};
    double right_fisical_right_goal[2][2] = {{6000, 900}, {6100, 950}};

    double outside_field_x_minus[2][2] = {{-8000, -4800}, {-6000, 4800}};
    double outside_field_x_plus[2][2] = {{6000, -4800}, {8000, 4800}};
    double outside_field_y_minus[2][2] = {{-6000, -6000}, {6000, -4700}};
    double outside_field_y_plus[2][2] = {{-6000, 4700}, {6000, 6000}};

    std::vector<Robot> allies = {};
    std::vector<Robot> enemies = {};
    double ball_pos[2] = {0, 0};
    double ball_speed[2] = {0, 0};
    double ball_speed_module = 0;

    std::vector<std::vector<double>> support_areas = {};


    //std::vector<rectangle> field_limits = {};
    //rectangle their_defense_area = rectangle({4200, -1800}, {6000, 1800});

    void generate_support_areas(double goal[2], double kick_distance);
    std::vector<double> kicking_position(std::vector<double> pos_0, std::vector<double> pos_1, double distance);
    bool ball_on_our_side();
    bool ball_on_our_area();
    bool ball_on_their_area();
};



#endif //WORLD_H
