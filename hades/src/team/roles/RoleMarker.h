//
// Created by caiu on 15/09/25.
//

#ifndef ROLEMARKER_H
#define ROLEMARKER_H

#include "RoleBase.h"

namespace roles {

class RoleMarker : public RoleBase {
public:
	void act(RobotController& robot);
};

} // roles

#endif //ROLEMARKER_H
