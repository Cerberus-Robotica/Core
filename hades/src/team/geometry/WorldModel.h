//
// Created by caiu on 11/04/25.
//

#ifndef WORLD_H
#define WORLD_H
#include <cmath>
#include <map>
#include <vector>

#include "../../c_trajectory/geometry/Rectangle.h"
#include "../Robot.h"
#include "Ball.h"
#include "Field.h"

class WorldModel {
public:
    WorldModel() : field() {}
    
    Field field;

    std::vector<Robot> allies = {};
    std::vector<Robot> enemies = {};
    Ball ball = {false, {0, 0}, {0, 0}};


    Point getKickingPosition(Point pos_0, Point pos_1, double distance);
    bool isBallOnOurSide();
    bool isBallOnOurArea();
    bool isBallOnTheirArea();
    std::vector<int> getAlliesIdsAccordingToDistanceToBall();
    int findNearestAllyThatIsntTheGoalKeeper(int id, int goalkeeper_id);
    void generateBallStopPosition();
    int getIdOfTheBallInterceptor();
    bool isBallMovingIdDirection(int id);
};



#endif //WORLD_H
