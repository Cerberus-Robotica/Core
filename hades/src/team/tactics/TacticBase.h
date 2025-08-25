//
// Created by caiu on 25/08/25.
//

#ifndef TACTICBASE_H
#define TACTICBASE_H
#include "../skills/skills.h"

class RobotController;

namespace tactics {

class TacticBase {
protected:
	skills::SkillMoveTo moveTo;
	skills::SkillStop stop;
	skills::SkillTurnTo turnTo;
	skills::SkillKick kick;
public:
	virtual void act(RobotController& robot) = 0;
};

} // tactics

#endif //TACTICBASE_H
