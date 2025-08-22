//
// Created by caiu on 21/08/25.
//

#include "AreaCircular.h"
#include <cmath>
#include <algorithm>

bool AreaCircular::detectIfContains(Point p) {
    double dx = p.getX() - center.getX();
    double dy = p.getY() - center.getY();
    return (dx * dx + dy * dy) <= (radius * radius);
}

bool AreaCircular::detectIfIntercepts(Point p1, Point p2) {
    LineSegment seg(p1, p2);
    return detectIfIntercepts(seg);
}

bool AreaCircular::detectIfIntercepts(LineSegment l) {
    auto inters = getInterceptionPoints(l);
    // Se pelo menos um ponto de interseção é válido, retorna true
    return !(inters[0].getX() == 0 && inters[0].getY() == 0 &&
             inters[1].getX() == 0 && inters[1].getY() == 0);
}

std::array<Point, 2> AreaCircular::getInterceptionPoints(Point p1, Point p2) {
    LineSegment seg(p1, p2);
    return getInterceptionPoints(seg);
}

std::array<Point, 2> AreaCircular::getInterceptionPoints(LineSegment l) {
    std::array<Point, 2> intersections = { Point(0,0), Point(0,0) };

    double x1 = l.getStart().getX() - center.getX();
    double y1 = l.getStart().getY() - center.getY();
    double x2 = l.getEnd().getX() - center.getX();
    double y2 = l.getEnd().getY() - center.getY();

    double dx = x2 - x1;
    double dy = y2 - y1;

    double a = dx*dx + dy*dy;
    double b = 2 * (x1*dx + y1*dy);
    double c = x1*x1 + y1*y1 - radius*radius;

    double discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return intersections; // sem interseção
    }

    discriminant = std::sqrt(discriminant);

    double t1 = (-b - discriminant) / (2*a);
    double t2 = (-b + discriminant) / (2*a);

    int found = 0;

    if (t1 >= 0.0 && t1 <= 1.0) {
        intersections[found++] = Point(
            center.getX() + x1 + t1*dx,
            center.getY() + y1 + t1*dy
        );
    }

    if (t2 >= 0.0 && t2 <= 1.0 && found < 2) {
        intersections[found++] = Point(
            center.getX() + x1 + t2*dx,
            center.getY() + y1 + t2*dy
        );
    }

    return intersections;
}
