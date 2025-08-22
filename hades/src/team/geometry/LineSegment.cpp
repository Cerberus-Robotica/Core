//
// Created by caiu on 21/08/25.
//

#include "LineSegment.h"

// LineSegment.cpp
#include "LineSegment.h"
#include <cmath>
#include <algorithm> // Para std::max e std::min

Point LineSegment::getStart() const {
    return start;
}

Point LineSegment::getEnd() const {
    return end;
}

double LineSegment::getLength() const {
    return start.getDistanceTo(end);
}


Point LineSegment::getPointProjection(const Point& p) const {
    const double l2 = std::pow(getLength(), 2);
    if (l2 == 0.0) {
        return start;
    }

    const double dot_product = (p.getX() - start.getX()) * (end.getX() - start.getX()) +
                               (p.getY() - start.getY()) * (end.getY() - start.getY());

    const double t = std::max(0.0, std::min(1.0, dot_product / l2));

    const double projX = start.getX() + t * (end.getX() - start.getX());
    const double projY = start.getY() + t * (end.getY() - start.getY());

    return Point(projX, projY);
}


bool LineSegment::isPointAligned(const Point& p, double tolerance) const {
    double dist_p_start = p.getDistanceTo(start);
    double dist_p_end = p.getDistanceTo(end);
    double segment_length = getLength();

    // A soma das distâncias deve ser igual ao comprimento total.
    return std::abs((dist_p_start + dist_p_end) - segment_length) < tolerance;
}