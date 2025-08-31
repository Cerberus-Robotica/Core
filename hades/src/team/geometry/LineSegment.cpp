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

Point LineSegment::getMiddle() const {
    return {(start.getX() + end.getX()) / 2, (start.getY() + end.getY()) / 2};
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


bool LineSegment::isPointAligned(const Point& p, double tolerance_radians) const {
    double segment_length = getLength();

    // Handle degenerate case (zero-length segment)
    if (segment_length < 1e-9) {
        return p.getDistanceTo(start) < 1e-9;
    }

    // Create vectors using your Vector2d class
    Vector2d segment_vector(end, start);  // Vector from start to end
    Vector2d point_vector(p, start);      // Vector from start to point

    // Calculate angle between the vectors
    double angle = segment_vector.getAngleBetween(point_vector);

    // Point is aligned only if angle is close to 0 (same direction)
    // We don't want points in the opposite direction (angle ≈ π)
    return angle <= tolerance_radians;
}