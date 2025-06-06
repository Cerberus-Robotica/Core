//
// Created by caiu on 09/04/25.
//

#ifndef TEAM_H
#define TEAM_H



class TeamInfo {
public:

    enum role {
        unknown = -1,
        goal_keeper,
        stricker,
        mid_field_support,
        debug_circular_trajectory,
        debug_squared_trajectory
    };

    enum sides {
        left,
        right
    };


    int goal_keeper_id = 0;
    sides our_side = right;

    bool debug = false;

    int play = -1;
    int color = 1; // 0.blue 1.yellow
    int game_state = -1;
    role roles[16] = {unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown};
    //int roles[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int active_robots[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int waiting_for_ball[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int positioned[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};



#endif //TEAM_H
