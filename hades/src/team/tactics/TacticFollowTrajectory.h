//
// Created by caiu on 25/08/25.
//

#ifndef TACTICFOLLOWTRAJECTORY_H
#define TACTICFOLLOWTRAJECTORY_H
#include "TacticBase.h"

namespace tactics {

class TacticFollowTrajectory : public TacticBase {
public:
	void act(RobotController& robot);
	void act(RobotController& robot, std::vector<Point>& trajectory);
};

} // tactics

#endif //TACTICFOLLOWTRAJECTORY_H
