//
// Created by caiu on 21/08/25.
//

#ifndef FIELD_H
#define FIELD_H

#include "AreaRectangular.h"
#include "LineSegment.h"

class Field {
public:
	Field() :	full_dimensions({{-6300, -4800}, {6300, 4800}}),
				inside_dimensions({-6000, -4500}, {6000, 4500}),
				theirGoal(Point(6000, -850), Point(6000, 850)),
				ourGoal(Point(-6000, -850), Point(-6000, 850)),
				theirDefenseArea({4200, -1750}, {6000, 1750}),
				ourDefenseArea({-6000, -1750}, {-4200, 1750}),
				theirFisicalBarrier({6000, -900}, {6300, 900}),
				ourFisicalBarrier({-6300, -900}, {-6000, 900})
	{}

	AreaRectangular full_dimensions;
	AreaRectangular inside_dimensions;
	LineSegment theirGoal;
	LineSegment ourGoal;
	AreaRectangular theirDefenseArea;
	AreaRectangular ourDefenseArea;
	AreaRectangular theirFisicalBarrier;
	AreaRectangular ourFisicalBarrier;
};



#endif //FIELD_H
