//
// Created by caiu on 18/09/25.
//

#ifndef TACTICBLOCKBALL_H
#define TACTICBLOCKBALL_H
#include <iostream>
#include <ostream>

#include "TacticBase.h"

namespace tactics {

class TacticBlockBall : public TacticBase {
public:
	void act(RobotController& robot, double block_distance);
	void act(RobotController& robot) {std::cout << "This is a dummy method" << std::endl;}
};

} // tactics

#endif //TACTICBLOCKBALL_H
