//
// Created by caiu on 25/08/25.
//

#ifndef TACTICKEEPXLINE_H
#define TACTICKEEPXLINE_H
#include "TacticBase.h"
#include "../geometry/LineSegment.h"

namespace tactics {

class TacticKeepXLine : public TacticBase {
public:
	void act(RobotController& robot, LineSegment y_segment, double y_rest);
	void act(RobotController& robot);
};

} // tactics

#endif //TACTICKEEPXLINE_H
