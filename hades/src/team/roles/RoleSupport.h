//
// Created by caiu on 18/08/25.
//

#ifndef ROLEMIDFIELD_H
#define ROLEMIDFIELD_H
#include "RoleBase.h"


namespace roles {

class RoleSupport : public RoleBase {
private:
    Point getSupportPosition(RobotController robot);
public:
    void act(RobotController& robot);
};

} // roles

#endif //ROLEMIDFIELD_H
