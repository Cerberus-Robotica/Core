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

#include "LineSegment.h"
#include <algorithm> // for std::min, std::max

// Helper function: orientation of ordered triplet (a, b, c)
// 0 -> collinear, 1 -> clockwise, 2 -> counterclockwise
static int orientation(const Point& a, const Point& b, const Point& c) {
    double val = (b.getY() - a.getY()) * (c.getX() - b.getX()) -
                 (b.getX() - a.getX()) * (c.getY() - b.getY());

    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

// Check if q lies on segment pr
static bool onSegment(const Point& p, const Point& q, const Point& r) {
    return (q.getX() <= std::max(p.getX(), r.getX()) && q.getX() >= std::min(p.getX(), r.getX()) &&
            q.getY() <= std::max(p.getY(), r.getY()) && q.getY() >= std::min(p.getY(), r.getY()));
}

bool LineSegment::intersects(const LineSegment& other) const {
    Point p1 = this->getStart();
    Point q1 = this->getEnd();
    Point p2 = other.getStart();
    Point q2 = other.getEnd();

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4) return true;

    // Special cases (collinear and overlapping)
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

LineSegment LineSegment::getResized(int new_size) {
    Vector2d vet = {end, start};
    vet = vet.getNormalized(new_size);
    return {start, Point(vet.getX() + start.getX(), vet.getY() + start.getY())};
}
