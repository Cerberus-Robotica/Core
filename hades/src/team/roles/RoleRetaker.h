//
// Created by caiu on 18/09/25.
//

#ifndef ROLERETAKER_H
#define ROLERETAKER_H
#include "RoleBase.h"

namespace roles {

class RoleRetaker : public RoleBase {
public:
	void act(RobotController& robot);
};

} // roles

#endif //ROLERETAKER_H
