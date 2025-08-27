//
// Created by caiu on 21/08/25.
//

#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "Point.h"
#include "Vector2d.h"

class LineSegment {
private:
    Point start = {0, 0};
    Point end = {0, 0};

public:
    LineSegment(const Point start, const Point end) : start(start), end(end) {};

    LineSegment(const Point pt, const Vector2d vect) {
        start = pt;
        end.setXY(pt.getX() + vect.getX(), pt.getY() + vect.getY());
    };

    Point getStart() const;
    Point getEnd() const;
    void setStart(const Point& start);
    void setEnd(const Point& end);

    double getLength() const;
    Point getPointProjection(const Point& p) const;
    bool isPointAligned(const Point& p, double tolerance = 1e-9) const;
};



#endif //LINESEGMENT_H
