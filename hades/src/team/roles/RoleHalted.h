//
// Created by caiu on 18/08/25.
//

#ifndef ROLEHALTED_H
#define ROLEHALTED_H
#include "RoleBase.h"

namespace roles {

class RoleHalted : public RoleBase {
public:
    void act(RobotController& robot);
};

} // roles

#endif //ROLEHALTED_H
