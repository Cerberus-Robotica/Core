//
// Created by caiu on 13/09/25.
//

#include "CircularShadow.h"
#include <cmath>
#include <iostream>

CircularShadow::CircularShadow(Point lightCenter, AreaCircular area)
    : Shadow(), circular(area.getCenter(), area.getRadius()) {
    setShadow(lightCenter, area);
}

void CircularShadow::setShadow(Point lightCenter, AreaCircular area) {
    circular = area;

    auto points = circular.getNormalPoints(lightCenter);
    Vector2d vet1 = {points[0], lightCenter};
    Vector2d vet2 = {points[1], lightCenter};

    line1 = LineSegment(points[0], vet1.getNormalized(100000));
    line2 = LineSegment(points[1], vet2.getNormalized(100000));
}

bool CircularShadow::isVisible(Point p) {
    return !containsPoint(p);
}

bool CircularShadow::containsPoint(Point p) {
    bool inCircularArea = circular.detectIfContains(p);
    bool betweenLines = isPointBetweenLines(p);
    return inCircularArea || betweenLines;
}

AreaCircular CircularShadow::getCircularArea() const {
    return circular;
}

bool CircularShadow::interceptsLine(LineSegment line) const {
    return circular.detectIfIntercepts(line);
}

bool CircularShadow::interceptsLine(Point p1, Point p2) const {
    return circular.detectIfIntercepts(p1, p2);
}

std::vector<Point> CircularShadow::getInterceptionPoints(LineSegment line) const {
    return circular.getInterceptionPoints(line);
}

bool CircularShadow::isPointBetweenLines(Point p) const {
    // Pega os extremos dos dois segmentos
    Point line1Start = line1.getStart();
    Point line1End   = line1.getEnd();
    Point line2Start = line2.getStart();
    Point line2End   = line2.getEnd();

    // Vetores das retas
    double cross1 = (line1End.getX() - line1Start.getX()) * (p.getY() - line1Start.getY()) -
                    (line1End.getY() - line1Start.getY()) * (p.getX() - line1Start.getX());

    double cross2 = (line2End.getX() - line2Start.getX()) * (p.getY() - line2Start.getY()) -
                    (line2End.getY() - line2Start.getY()) * (p.getX() - line2Start.getX());

    // O ponto precisa estar entre as duas retas (ângulo)
    bool betweenRays = (cross1 >= 0 && cross2 <= 0) || (cross1 <= 0 && cross2 >= 0);

    if (!betweenRays) return false;

    // Agora checamos se o ponto está dentro do retângulo delimitado pelos dois segmentos
    double minX = std::min(std::min(line1Start.getX(), line1End.getX()),
                           std::min(line2Start.getX(), line2End.getX()));
    double maxX = std::max(std::max(line1Start.getX(), line1End.getX()),
                           std::max(line2Start.getX(), line2End.getX()));

    double minY = std::min(std::min(line1Start.getY(), line1End.getY()),
                           std::min(line2Start.getY(), line2End.getY()));
    double maxY = std::max(std::max(line1Start.getY(), line1End.getY()),
                           std::max(line2Start.getY(), line2End.getY()));

    bool insideBox = (p.getX() >= minX && p.getX() <= maxX &&
                      p.getY() >= minY && p.getY() <= maxY);

    return insideBox;
}