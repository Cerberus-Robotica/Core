//
// Created by caiom on 4/7/2025.
//

#ifndef ROBOT_H
#define ROBOT_H



class robot {
public:
    int id = -1;
    float pos[2] = {0, 0};
    float yaw = 0;
    float vel[2] = {0, 0};
    float vyaw = 0;
    bool detected = false;

    robot(int id) {
        this->id = id;
    }
};



#endif //ROBOT_H
