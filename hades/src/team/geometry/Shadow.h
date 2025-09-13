//
// Created by caiu on 13/09/25.
//

#ifndef SHADOW_H
#define SHADOW_H
#include "LineSegment.h"


class Shadow {
protected:
	LineSegment line1;
    LineSegment line2;
public:
	virtual void setShadow();
    virtual void isVisible(Point p);
};



#endif //SHADOW_H
