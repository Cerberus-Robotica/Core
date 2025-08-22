//
// Created by caiu on 21/08/25.
//

#ifndef FIELD_H
#define FIELD_H

#include "AreaRectangular.h"
#include "LineSegment.h"

class Field {
public:
  	AreaRectangular dimensions;
	LineSegment theirGoal;
	LineSegment ourGoal;
	AreaRectangular theirDefenseArea;
	AreaRectangular ourDefenseArea;
	AreaRectangular theirFisicalBarrier;
	AreaRectangular ourFisicalBarrier;
};



#endif //FIELD_H
