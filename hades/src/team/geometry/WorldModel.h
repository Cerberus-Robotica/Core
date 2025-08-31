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

    std::array<Robot, 16> allies = {
        Robot(0), Robot(1), Robot(2), Robot(3),
        Robot(4), Robot(5), Robot(6), Robot(7),
        Robot(8), Robot(9), Robot(10), Robot(11),
        Robot(12), Robot(13), Robot(14), Robot(15)
    };

    std::array<Robot, 16> enemies = {
        Robot(0), Robot(1), Robot(2), Robot(3),
        Robot(4), Robot(5), Robot(6), Robot(7),
        Robot(8), Robot(9), Robot(10), Robot(11),
        Robot(12), Robot(13), Robot(14), Robot(15)
    };

    Ball ball = {false, {0, 0}, {0, 0}};


    Point getKickingPosition(Point pos_0, Point pos_1, double distance);
    bool isPointOnOurSide(Point p);
    bool isPointOnOurArea(Point p);
    bool isPointOnTheirArea(Point p);
    std::vector<int> getAlliesIdsAccordingToDistanceToBall();
    int findNearestAllyThatIsntTheGoalKeeper(int id, int goalkeeper_id);
    int getIdOfTheBallInterceptor();
    bool isBallMovingIdDirection(int id);
    bool isBallMovingRobotDirection(Robot robot);
    bool isBallReachable(); //TODO IMPLEMENTAR PARA TACTICPOSITIONANDKICK() PARA NAO CHUTAR COM BOLA DENTRO DA AREA TMJ
    bool doInterceptAnyRobot(LineSegment l);
};



#endif //WORLD_H
