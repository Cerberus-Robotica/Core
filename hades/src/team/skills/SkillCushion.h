//
// Created by caiu on 26/08/25.
//

#ifndef SKILLCUSHION_H
#define SKILLCUSHION_H
#include "SkillBase.h"

namespace skills {

class SkillCushion : public SkillBase {
public:
	void act(RobotController& robot);
};

} // skills

#endif //SKILLCUSHION_H
