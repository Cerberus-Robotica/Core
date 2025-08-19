//
// Created by caiu on 18/08/25.
//
#pragma once
#ifndef ROLEBASE_H
#define ROLEBASE_H
#include "../skills/skills.h"
#include "../tactics/tactics.h"

class RobotController;

namespace roles {

class RoleBase {
public:
    virtual void act(RobotController& robot) = 0;
};

}

#endif //ROLEBASE_H
