//
// Created by caiu on 21/08/25.
//

#ifndef AREACIRCULAR_H
#define AREACIRCULAR_H

#include "Point.h"
#include "LineSegment.h"
#include <array>

class AreaCircular {
private:
	Point center;
	double radius;
public:
  	AreaCircular(Point center, double radius) : center(center), radius(radius) {};
	bool detectIfContains(Point);
	bool detectIfIntercepts(Point, Point);
	bool detectIfIntercepts(LineSegment);
	std::array<Point, 2> getInterceptionPoints(Point p1, Point p2);
	std::array<Point, 2> getInterceptionPoints(LineSegment l);
};



#endif //AREACIRCULAR_H
