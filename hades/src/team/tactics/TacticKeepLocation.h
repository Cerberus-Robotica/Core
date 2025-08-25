//
// Created by caiu on 25/08/25.
//

#ifndef TACTICKEEPLOCATION_H
#define TACTICKEEPLOCATION_H
#include "TacticBase.h"

namespace tactics {

class TacticKeepLocation : public TacticBase {
public:
	void act(RobotController& robot);
	void act(RobotController& robot, Point keep);
};

} // tactics

#endif //TACTICKEEPLOCATION_H
