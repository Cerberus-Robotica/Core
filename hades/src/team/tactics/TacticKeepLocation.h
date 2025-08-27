//
// Created by caiu on 25/08/25.
//

#ifndef TACTICKEEPLOCATION_H
#define TACTICKEEPLOCATION_H
#include "TacticBase.h"

namespace tactics {

class TacticKeepLocation : public TacticBase {
private:
	double distanceThreshold = 400;
	double angle_tolerance = 3.1415/8;
public:
	void act(RobotController& robot);
	void act(RobotController& robot, Point keep);
};

} // tactics

#endif //TACTICKEEPLOCATION_H
