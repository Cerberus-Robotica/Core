#include "AreaRectangular.h"
#include <algorithm>

// Função auxiliar para checar se dois segmentos se interceptam
static bool segmentsIntersect(Point p1, Point p2, Point q1, Point q2, Point& intersection) {
    // Use parametric form: p1 + t*(p2-p1) and q1 + s*(q2-q1)
    double dx1 = p2.getX() - p1.getX();
    double dy1 = p2.getY() - p1.getY();
    double dx2 = q2.getX() - q1.getX();
    double dy2 = q2.getY() - q1.getY();

    double det = dx1 * dy2 - dy1 * dx2;

    // Check for parallel lines with tolerance
    if (std::abs(det) < 1e-10) {
        return false; // Lines are parallel
    }

    double dx3 = q1.getX() - p1.getX();
    double dy3 = q1.getY() - p1.getY();

    double t = (dx3 * dy2 - dy3 * dx2) / det;
    double s = (dx3 * dy1 - dy3 * dx1) / det;

    // Check if intersection is within both segments (with small tolerance for floating point)
    const double epsilon = 1e-10;
    if (t >= -epsilon && t <= 1.0 + epsilon && s >= -epsilon && s <= 1.0 + epsilon) {
        intersection = Point(p1.getX() + t * dx1, p1.getY() + t * dy1);
        return true;
    }

    return false;
}


bool AreaRectangular::detectIfContains(Point p) {
    return (p.getX() >= minorPoint.getX() && p.getX() <= majorPoint.getX() &&
            p.getY() >= minorPoint.getY() && p.getY() <= majorPoint.getY());
}

bool AreaRectangular::detectIfIntercepts(Point p1, Point p2) const {
    LineSegment seg(p1, p2);
    return detectIfIntercepts(seg);
}

bool AreaRectangular::detectIfIntercepts(LineSegment l) const {
    Point rectPts[4] = {
        minorPoint,
        Point(majorPoint.getX(), minorPoint.getY()),
        majorPoint,
        Point(minorPoint.getX(), majorPoint.getY())
    };

    // Checar interseção com cada lado do retângulo
    for (int i = 0; i < 4; i++) {
        Point inter = {0, 0};
        if (segmentsIntersect(l.getStart(), l.getEnd(), rectPts[i], rectPts[(i + 1) % 4], inter)) {
            return true;
        }
    }
    return false;
}

std::vector<Point> AreaRectangular::getInterceptionPoints(Point p1, Point p2) const {
    LineSegment seg(p1, p2);
    return getInterceptionPoints(seg);
}

void AreaRectangular::grow(double size) {
    Point& p = majorPoint;
    if (p.getX() > 0) p.setX(p.getX() + size); else p.setX(p.getX() - size);
    if (p.getY() > 0) p.setY(p.getY() + size); else p.setY(p.getY() - size);
    Point& p2 = minorPoint;
    if (p2.getX() > 0) p2.setX(p2.getX() + size); else p2.setX(p2.getX() - size);
    if (p2.getY() > 0) p2.setY(p2.getY() + size); else p2.setY(p2.getY() - size);
}

AreaRectangular AreaRectangular::getResized(double size) {
    Point p = majorPoint;
    if (p.getX() > 0) p.setX(p.getX() + size); else p.setX(p.getX() - size);
    if (p.getY() > 0) p.setY(p.getY() + size); else p.setY(p.getY() - size);
    Point p2 = minorPoint;
    if (p2.getX() > 0) p2.setX(p2.getX() + size); else p2.setX(p2.getX() - size);
    if (p2.getY() > 0) p2.setY(p2.getY() + size); else p2.setY(p2.getY() - size);
    return AreaRectangular(p2, p);
}

std::vector<Point> AreaRectangular::getInterceptionPoints(LineSegment l) const {
    std::vector<Point> intersections;
    intersections.reserve(2); // A line can intersect a rectangle at most at 2 points

    Point rectPts[4] = {
        minorPoint,
        Point(majorPoint.getX(), minorPoint.getY()),
        majorPoint,
        Point(minorPoint.getX(), majorPoint.getY())
    };

    for (int i = 0; i < 4; i++) {
        Point inter = {0, 0};
        if (segmentsIntersect(l.getStart(), l.getEnd(), rectPts[i], rectPts[(i + 1) % 4], inter)) {
            // Check if this intersection point is already found (avoid duplicates)
            bool isDuplicate = false;
            for (const Point& existingPoint : intersections) {
                if (std::abs(existingPoint.getX() - inter.getX()) < 1e-9 &&
                    std::abs(existingPoint.getY() - inter.getY()) < 1e-9) {
                    isDuplicate = true;
                    break;
                    }
            }

            if (!isDuplicate && intersections.size() < 2) {
                intersections.push_back(inter);
            }
        }
    }
    return intersections;
}

std::array<Point, 2> AreaRectangular::getNormalPoints(Point& p) {
    double adc = 1.0;

    if (p.getX() > majorPoint.getX()) {
        if (p.getY() > majorPoint.getY()) {
            Point p1 = {minorPoint.getX() - adc, majorPoint.getY() + adc};
            Point p2 = {majorPoint.getX() + adc, minorPoint.getY() - adc};
            return {p1, p2};
        } else if (p.getY() < minorPoint.getY()) {
            Point p1 = {minorPoint.getX() - adc, minorPoint.getY() - adc};
            Point p2 = {majorPoint.getX() + adc, majorPoint.getY() + adc};
            return {p1, p2};
        } else {
            Point p1 = {majorPoint.getX() + adc, minorPoint.getY() - adc};
            Point p2 = {majorPoint.getX() + adc, majorPoint.getY() + adc};
            return {p1, p2};
        }
    } else if (p.getX() < minorPoint.getX()) {
        if (p.getY() > majorPoint.getY()) {
            Point p1 = {majorPoint.getX() + adc, majorPoint.getY() + adc};
            Point p2 = {minorPoint.getX() - adc, minorPoint.getY() - adc};
            return {p1, p2};
        } else if (p.getY() < minorPoint.getY()) {
            Point p1 = {minorPoint.getX() - adc, majorPoint.getY() + adc};
            Point p2 = {majorPoint.getX() + adc, minorPoint.getY() - adc};
            return {p1, p2};
        } else {
            Point p1 = {minorPoint.getX() - adc, minorPoint.getY() - adc};
            Point p2 = {minorPoint.getX() - adc, majorPoint.getY() + adc};
            return {p1, p2};
        }
    } else {
        if (p.getY() > majorPoint.getY()) {
            Point p1 = {majorPoint.getX() + adc, majorPoint.getY() + adc};
            Point p2 = {minorPoint.getX() - adc, majorPoint.getY() + adc};
            return {p1, p2};
        } else {
            Point p1 = {minorPoint.getX() - adc, minorPoint.getY() - adc};
            Point p2 = {majorPoint.getX() + adc, minorPoint.getY() - adc};
            return {p1, p2};
        }
    }
}

Point AreaRectangular::getMajorPoint() {
    return majorPoint;
}

Point AreaRectangular::getMinorPoint() {
    return minorPoint;
}

void AreaRectangular::setMajorPoint(Point p) {
    majorPoint = p;
}
void AreaRectangular::setMinorPoint(Point p) {
    minorPoint = p;
}