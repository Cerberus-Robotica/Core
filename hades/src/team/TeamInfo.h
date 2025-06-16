//
// Created by caiu on 09/04/25.
//

#ifndef TEAM_H
#define TEAM_H
#include <map>


class TeamInfo {
public:

    enum role {
        unknown = -1,
        goal_keeper,
        stricker,
        mid_field,
        defender,
        debug_circular_trajectory,
        debug_squared_trajectory
    };

    enum sides {
        left,
        right
    };

    enum color {
        blue,
        yellow
    };


    int goal_keeper_id = 0;
    sides our_side = right;
    int our_side_sign = +1;

    bool debug = false;

    double central_line_x = 0;
    int play = -1;
    color color = yellow; // 0.blue 1.yellow
    int game_state = -1;
    role roles[16] = {unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown};
    role enemy_roles[16] = {unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown};
    //std::map<role, int> enemy_roles;

    double striker_max_dislocation = 2000;
    double mid_field_max_dislocation = 0;
    double defender_min_dislocation = 2000;

    //int roles[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int active_robots[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int num_of_active_robots = 0;
    bool positioned[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
};



#endif //TEAM_H
