//
// Created by caiu on 13/09/25.
//

#ifndef TACTICMARK_H
#define TACTICMARK_H
#include "TacticBase.h"
#include "../Robot.h"

namespace tactics {

class TacticMark : public TacticBase {
private:
	double distanceToMark = 400;
public:
	void act(RobotController& robot, Robot toMark);
	void act(RobotController& robot);
};

} // tactics

#endif //TACTICMARK_H
