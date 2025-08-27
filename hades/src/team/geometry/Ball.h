//
// Created by caiu on 21/08/25.
//

#ifndef BALL_H
#define BALL_H

#include "Point.h"
#include "Vector2d.h"

class Ball {
private:
	bool detected;
	double deceleration = 0.2;
	Point position;
	Vector2d velocity;
	Point stopPosition = Point(0, 0);
public:
  	Ball(bool detected, Point position, Vector2d velocity) : detected(detected), position(position), velocity(velocity) {stopPosition = getStopPosition();};
  	void setVelocity(const Vector2d& v);
  	Vector2d getVelocity() const;
  	void setDetected(bool detected);
    bool getDetected();
  	void setPosition(Point position);
  	Point getPosition();
	Point getStopPosition() const;
};



#endif //BALL_H
