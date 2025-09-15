//
// Created by caiu on 18/08/25.
//
#pragma once
#ifndef ROLEBASE_H
#define ROLEBASE_H
#include "../tactics/tactics.h"
#include "../skills/skills.h"

class RobotController;

namespace roles {

class RoleBase {
protected:
    skills::SkillMoveTo moveTo;
    skills::SkillStop stop;
    skills::SkillTurnTo turnTo;
    skills::SkillKick kick;
    tactics::TacticFollowTrajectory followTrajectory;
    tactics::TacticPositionAndKick positionAndKick;
    tactics::TacticKeepLocation keepLocation;
    tactics::TacticKeepXLine keepXLine;
    tactics::TacticIntercept intercept;
    tactics::TacticMark mark;
public:
    virtual void act(RobotController& robot) = 0;
};

}

#endif //ROLEBASE_H
