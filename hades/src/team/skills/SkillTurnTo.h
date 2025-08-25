//
// Created by caiu on 25/08/25.
//

#ifndef SKILLTURNTO_H
#define SKILLTURNTO_H
#include "SkillBase.h"
#include "../geometry/Point.h"

namespace skills {

class SkillTurnTo {
public:
	void act(RobotController& robot, Point goal);
private:
  	double find_angle_error(RobotController robot, Point goal);
    double turn_control(RobotController robot, double delta);
};

} // skills

#endif //SKILLTURNTO_H
