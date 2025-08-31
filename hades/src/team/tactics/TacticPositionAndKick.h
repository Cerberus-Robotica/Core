//
// Created by caiu on 25/08/25.
//

#ifndef TACTICPOSITIONANDKICK_H
#define TACTICPOSITIONANDKICK_H
#include "TacticBase.h"

class Robot;

namespace tactics {

class TacticPositionAndKick : public TacticBase {
public:
	void act(RobotController& robot, Point goal);
	void act(RobotController& robot, Robot sup);
	void act(RobotController& robot);
};

} // tactics

#endif //TACTICPOSITIONANDKICK_H
