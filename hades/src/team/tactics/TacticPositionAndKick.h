//
// Created by caiu on 25/08/25.
//

#ifndef TACTICPOSITIONANDKICK_H
#define TACTICPOSITIONANDKICK_H
#include "TacticBase.h"

class Robot;

namespace tactics {

class TacticPositionAndKick : public TacticBase {
private:
	double distance_to_kick = 300;
public:
	void act(RobotController& robot, Point goal, bool wait = false);
	void act(RobotController& robot, Robot sup, bool wait = false);
	void act(RobotController& robot);
};

} // tactics

#endif //TACTICPOSITIONANDKICK_H
