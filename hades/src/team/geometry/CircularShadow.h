//
// Created by caiu on 13/09/25.
//

#ifndef CIRCULARSHADOW_H
#define CIRCULARSHADOW_H

#include "Shadow.h"
#include "AreaCircular.h"

class CircularShadow : public Shadow {
private:
	AreaCircular circular;
	LineSegment line1 = {Point(0, 0), Point(0, 0)};
	LineSegment line2 = {Point(0, 0), Point(0, 0)};
public:
	CircularShadow(Point lightCenter, AreaCircular area);

	// Override virtual methods from Shadow
	void setShadow(Point lightCenter, AreaCircular area);
	bool isVisible(Point p) override;
	bool containsPoint(Point p) override;

	// Additional methods specific to circular shadow
	AreaCircular getCircularArea() const;
	bool interceptsLine(LineSegment line) const;
	bool interceptsLine(Point p1, Point p2) const;
	std::vector<Point> getInterceptionPoints(LineSegment line) const;
	bool isPointBetweenLines(Point p) const;
};

#endif //CIRCULARSHADOW_H
