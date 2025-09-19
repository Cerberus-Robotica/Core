//
// Created by caiu on 19/09/25.
//

#ifndef ROLEWATCHER_H
#define ROLEWATCHER_H
#include "RoleBase.h"

namespace roles {

class RoleWatcher : public RoleBase {
public:
	void act(RobotController& robot);
};

} // roles

#endif //ROLEWATCHER_H
