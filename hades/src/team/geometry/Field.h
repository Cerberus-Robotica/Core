//
// Created by caiu on 21/08/25.
//

#ifndef FIELD_H
#define FIELD_H

#include "AreaRectangular.h"
#include "LineSegment.h"

class Field {
public:
	Field() : dimensions({0, 0}, {0, 0}),
				theirGoal({0, 0}, {0, 0}),
				ourGoal({0, 0}, {0, 0}),
				theirDefenseArea({0, 0}, {0, 0}),
				ourDefenseArea({0, 0}, {0, 0}),
				theirFisicalBarrier({0, 0}, {0, 0}),
				ourFisicalBarrier({0, 0}, {0, 0})
	{}

  	AreaRectangular dimensions;
	LineSegment theirGoal;
	LineSegment ourGoal;
	AreaRectangular theirDefenseArea;
	AreaRectangular ourDefenseArea;
	AreaRectangular theirFisicalBarrier;
	AreaRectangular ourFisicalBarrier;
};



#endif //FIELD_H
