//
// Created by caiu on 21/08/25.
//

#ifndef AREARECTANGULAR_H
#define AREARECTANGULAR_H

#include "Point.h"
#include "LineSegment.h"
#include <array>

class AreaRectangular {
private:
	Point minorPoint;
	Point majorPoint;
public:
  	AreaRectangular(Point minorPoint, Point majorPoint) : minorPoint(minorPoint), majorPoint(majorPoint) {};
    bool detectIfContains(Point);
	bool detectIfIntercepts(Point, Point);
	bool detectIfIntercepts(LineSegment);
	std::array<Point, 2> getInterceptionPoints(Point p1, Point p2);
	std::array<Point, 2> getInterceptionPoints(LineSegment l);
};



#endif //AREARECTANGULAR_H
