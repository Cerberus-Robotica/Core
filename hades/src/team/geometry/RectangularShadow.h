//
// Created by caiu on 15/09/25.
//

#ifndef RECTANGULARSHADOW_H
#define RECTANGULARSHADOW_H

#include "Shadow.h"
#include "AreaRectangular.h"

class RectangularShadow : public Shadow{
private:
	AreaRectangular rectangle;
	LineSegment line1 = {Point(0, 0), Point(0, 0)};
	LineSegment line2 = {Point(0, 0), Point(0, 0)};
public:
	RectangularShadow(Point lightCenter, AreaRectangular area);

	// Override virtual methods from Shadow
	void setShadow(Point lightCenter, AreaRectangular area);
	bool isVisible(Point p) override;
	bool containsPoint(Point p) override;

	// Additional methods specific to circular shadow
	AreaRectangular getRectangularArea() const;
	bool interceptsLine(LineSegment line) const;
	bool interceptsLine(Point p1, Point p2) const;
	std::vector<Point> getInterceptionPoints(LineSegment line) const;
	bool isPointBetweenLines(Point p) const;
};



#endif //RECTANGULARSHADOW_H
