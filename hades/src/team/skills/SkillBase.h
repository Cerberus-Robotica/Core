//
// Created by caiu on 25/08/25.
//

#ifndef SKILLBASE_H
#define SKILLBASE_H

class RobotController;

namespace skills {

class SkillBase {
public:
	virtual void act(RobotController& robot) = 0;
};

} // skills

#endif //SKILLBASE_H
