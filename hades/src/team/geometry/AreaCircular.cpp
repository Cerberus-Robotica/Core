//
// Created by caiu on 21/08/25.
//

#include "AreaCircular.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>


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

std::array<Point, 2> AreaCircular::getInterceptionPoints(const AreaCircular& other) {
    // Distance between centers
    double dx = other.center.getX() - center.getX();
    double dy = other.center.getY() - center.getY();
    double d = std::sqrt(dx*dx + dy*dy);

    // Check if circles are too far apart or one is contained in the other
    if (d > (radius + other.radius)) {
        throw std::runtime_error("Circles are too far apart - no intersection");
    }
    if (d < std::abs(radius - other.radius)) {
        throw std::runtime_error("One circle is contained within the other - no intersection");
    }
    if (d == 0 && radius == other.radius) {
        throw std::runtime_error("Circles are identical - infinite intersections");
    }
    if (d == 0) {
        throw std::runtime_error("Circles are concentric - no intersection");
    }

    // Distance from this circle's center to the line connecting intersection points
    double a = (radius*radius - other.radius*other.radius + d*d) / (2*d);

    // Height of the triangle formed by centers and intersection point
    double h = std::sqrt(radius*radius - a*a);

    // Point on the line between centers
    double px = center.getX() + a * (dx / d);
    double py = center.getY() + a * (dy / d);

    // Calculate the two intersection points
    std::array<Point, 2> intersections = {Point(0, 0), Point(0, 0)};
    intersections[0] = Point(
        px + h * (-dy / d),
        py + h * (dx / d)
    );

    intersections[1] = Point(
        px - h * (-dy / d),
        py - h * (dx / d)
    );

    return intersections;
}

std::vector<Point> AreaCircular::getInterceptionPoints(Point p1, Point p2) {
    LineSegment seg(p1, p2);
    return getInterceptionPoints(seg);
}

std::vector<Point> AreaCircular::getInterceptionPoints(LineSegment l) {

    std::vector<Point> intersections = { Point(0,0), Point(0,0) };

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
        intersections.reserve(1);
        intersections[found++] = Point(
            center.getX() + x1 + t1*dx,
            center.getY() + y1 + t1*dy
        );
    }

    if (t2 >= 0.0 && t2 <= 1.0 && found < 2) {
        intersections.reserve(1);
        intersections[found++] = Point(
            center.getX() + x1 + t2*dx,
            center.getY() + y1 + t2*dy
        );
    }

    return intersections;
}

std::array<Point, 2> AreaCircular::getNormalPoints(Point& p) {
    double diversion = radius*1.05;

    double k_norm = sqrt(fabs(pow(p.getDistanceTo(center), 2) - diversion*diversion));

    double theta = atan2((center.getY() - p.getY()), (center.getX() - p.getX()));
    double dif = atan2(diversion, k_norm);
    double theta_1 = theta + dif;
    double theta_2 = theta - dif;

    if (k_norm < radius) {
        k_norm = radius;
    }

    double distancia_do_centro = p.getDistanceTo(center);
    if (distancia_do_centro < diversion) {
        double correction = atan2(distancia_do_centro - radius, k_norm);
        theta_1 += correction;
        theta_2 -= correction;
    }
    Point p1 = {k_norm * cos(theta_1) + p.getX(), k_norm * sin(theta_1) + p.getY()};
    Point p2 = {k_norm * cos(theta_2) + p.getX(), k_norm * sin(theta_2) + p.getY()};
    return {p1, p2};
}