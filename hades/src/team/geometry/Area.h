//
// Created by caiu on 21/08/25.
//

#ifndef AREA_H
#define AREA_H

#include <array>
#include "Point.h"
#include "LineSegment.h"

class Area {
public:
	virtual bool detectIfContains(Point p) = 0;
	virtual bool detectIfIntercepts(Point p1, Point p2) = 0;
	virtual bool detectIfIntercepts(LineSegment l) = 0;
	virtual std::array<Point, 2> getInterceptionPoints(Point p1, Point p2) = 0;
	virtual std::array<Point, 2> getInterceptionPoints(LineSegment l) = 0;
};

#endif //AREA_H
