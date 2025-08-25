//
// Created by caiu on 25/08/25.
//

#ifndef SKILLMOVETO_H
#define SKILLMOVETO_H
#include "SkillBase.h"
#include "../geometry/Vector2d.h"
#include "../geometry/Point.h"

namespace skills {

class SkillMoveTo : public SkillBase {
public:
	void act(RobotController& robot, Point goal, bool avoid_ball);
	void act(RobotController& robot);
private:
	Vector2d motion_planner(RobotController& robot, std::vector<Point> trajectory);
	Vector2d motion_control(Vector2d v_vet, double yaw);
	std::vector<Point> find_trajectory(RobotController& robot, Point start, Point goal, bool avoid_ball);
};

} // skills

#endif //SKILLMOVETO_H
