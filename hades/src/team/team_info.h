//
// Created by caiu on 09/04/25.
//

#ifndef TEAM_H
#define TEAM_H



class team_info {
public:
    int goal_keeper_id = 0;
    int left_side = false;

    bool debug = false;

    int play = -1;
    int color = 1; // 0.blue 1.yellow
    int game_state = -1;
    //int roles[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int roles[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int active_robots[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};



#endif //TEAM_H
