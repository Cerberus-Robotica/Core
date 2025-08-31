//
// Created by caiu on 26/08/25.
//

#ifndef TACTICINTERCEPT_H
#define TACTICINTERCEPT_H
#include "TacticBase.h"
namespace tactics {

class TacticIntercept : public TacticBase {
private:
	double distanceThreshold = 200;
    double angle_tolerance = 3.1415/8;
public:
	void act(RobotController& robot) override;
};

} // tactics

#endif //TACTICINTERCEPT_H
