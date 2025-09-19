//
// Created by caiu on 19/09/25.
//

#ifndef ROLEPENALTIER_H
#define ROLEPENALTIER_H
#include "RoleBase.h"

namespace roles {

class RolePenaltier : public RoleBase {
public:
	void act(RobotController& robot);
};

} // roles

#endif //ROLEPENALTIER_H
