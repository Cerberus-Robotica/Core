//
// Created by caiu on 09/04/25.
//

#ifndef TEAM_H
#define TEAM_H



class team_info {
public:
    int play = -1;
    int color = 0; // 0.blue 1.yellow
    int game_state = -1;
    int roles[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int active_robots[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};



#endif //TEAM_H
