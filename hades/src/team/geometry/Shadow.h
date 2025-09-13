//
// Created by caiu on 13/09/25.
//

#ifndef SHADOW_H
#define SHADOW_H
#include "LineSegment.h"
#include "Point.h"

class Shadow {
public:
	Shadow() = default;
	virtual ~Shadow() = default;
	virtual bool isVisible(Point p) = 0;
	virtual bool containsPoint(Point p) = 0;  // Check if point is in shadow

	// Helper method to check if point is between the two line segments
	bool isPointBetweenLines(Point p) const;
};

#endif //SHADOW_H