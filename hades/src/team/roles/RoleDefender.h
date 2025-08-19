//
// Created by caiu on 18/08/25.
//

#ifndef ROLEDEFENDER_H
#define ROLEDEFENDER_H
#include "RoleBase.h"

namespace roles {

class RoleDefender : public RoleBase {
public:
    void act(RobotController& robot);
};

} // roles

#endif //ROLEDEFENDER_H
