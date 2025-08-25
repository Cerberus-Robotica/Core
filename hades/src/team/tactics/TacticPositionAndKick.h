//
// Created by caiu on 25/08/25.
//

#ifndef TACTICPOSITIONANDKICK_H
#define TACTICPOSITIONANDKICK_H
#include "TacticBase.h"

namespace tactics {

class TacticPositionAndKick : public TacticBase {
public:
	void act(RobotController& robot, Point goal);
	void act(RobotController& robot, int id);
	void act(RobotController& robot);
	std::vector<Point> find_ball_trajectory(RobotController& robot, Point start, Point goal);
};

} // tactics

#endif //TACTICPOSITIONANDKICK_H
