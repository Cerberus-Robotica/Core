#include "AreaRectangular.h"
#include <algorithm>

// Função auxiliar para checar se dois segmentos se interceptam
static bool segmentsIntersect(Point p1, Point p2, Point q1, Point q2, Point& intersection) {
    double A1 = p2.getY() - p1.getY();
    double B1 = p1.getX() - p2.getX();
    double C1 = A1 * p1.getX() + B1 * p1.getY();

    double A2 = q2.getY() - q1.getY();
    double B2 = q1.getX() - q2.getX();
    double C2 = A2 * q1.getX() + B2 * q1.getY();

    double det = A1 * B2 - A2 * B1;
    if (det == 0) {
        return false; // paralelas
    }

    double x = (B2 * C1 - B1 * C2) / det;
    double y = (A1 * C2 - A2 * C1) / det;

    // Verificar se (x,y) está dentro dos dois segmentos
    if (x >= std::min(p1.getX(), p2.getX()) && x <= std::max(p1.getX(), p2.getX()) &&
        y >= std::min(p1.getY(), p2.getY()) && y <= std::max(p1.getY(), p2.getY()) &&
        x >= std::min(q1.getX(), q2.getX()) && x <= std::max(q1.getX(), q2.getX()) &&
        y >= std::min(q1.getY(), q2.getY()) && y <= std::max(q1.getY(), q2.getY())) {
        intersection = Point(x, y);
        return true;
    }

    return false;
}

bool AreaRectangular::detectIfContains(Point p) {
    return (p.getX() >= minorPoint.getX() && p.getX() <= majorPoint.getX() &&
            p.getY() >= minorPoint.getY() && p.getY() <= majorPoint.getY());
}

bool AreaRectangular::detectIfIntercepts(Point p1, Point p2) {
    LineSegment seg(p1, p2);
    return detectIfIntercepts(seg);
}

bool AreaRectangular::detectIfIntercepts(LineSegment l) {
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

std::array<Point, 2> AreaRectangular::getInterceptionPoints(Point p1, Point p2) {
    LineSegment seg(p1, p2);
    return getInterceptionPoints(seg);
}

std::array<Point, 2> AreaRectangular::getInterceptionPoints(LineSegment l) {
    std::array<Point, 2> intersections = { Point(0, 0), Point(0, 0) };
    int found = 0;

    Point rectPts[4] = {
        minorPoint,
        Point(majorPoint.getX(), minorPoint.getY()),
        majorPoint,
        Point(minorPoint.getX(), majorPoint.getY())
    };

    for (int i = 0; i < 4; i++) {
        Point inter = {0 ,0};
        if (segmentsIntersect(l.getStart(), l.getEnd(), rectPts[i], rectPts[(i + 1) % 4], inter)) {
            if (found < 2) {
                intersections[found] = inter;
                found++;
            }
        }
    }

    return intersections;
}
