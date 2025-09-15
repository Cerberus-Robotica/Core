//
// Created by caiu on 15/09/25.
//

#include "RectangularShadow.h"

#include "RectangularShadow.h"
#include <algorithm> // Para std::min e std::max

RectangularShadow::RectangularShadow(Point lightCenter, AreaRectangular area)
    : Shadow(), rectangle(area.getMinorPoint(), area.getMajorPoint())
{
    setShadow(lightCenter, area);
}

void RectangularShadow::setShadow(Point lightCenter, AreaRectangular area)
{
    rectangle = area;
    auto points = rectangle.getNormalPoints(lightCenter);
    // Cria vetores que apontam dos pontos externos do retângulo para a fonte de luz
    Vector2d vet1 = {points[0], lightCenter};
    Vector2d vet2 = {points[1], lightCenter};
    // Normaliza esses vetores para criar linhas longas de sombra (extensão arbitrária)
    line1 = LineSegment(points[0], vet1.getNormalized(100000));
    line2 = LineSegment(points[1], vet2.getNormalized(100000));
}

bool RectangularShadow::isVisible(Point p)
{
    return !containsPoint(p);
}

bool RectangularShadow::containsPoint(Point p)
{
    bool inRectArea = rectangle.detectIfContains(p);
    bool betweenLines = isPointBetweenLines(p);
    return inRectArea || betweenLines;
}

AreaRectangular RectangularShadow::getRectangularArea() const
{
    return rectangle;
}

bool RectangularShadow::interceptsLine(LineSegment line) const
{
    return rectangle.detectIfIntercepts(line);
}

bool RectangularShadow::interceptsLine(Point p1, Point p2) const
{
    return rectangle.detectIfIntercepts(p1, p2);
}

std::vector<Point> RectangularShadow::getInterceptionPoints(LineSegment line) const
{
    return rectangle.getInterceptionPoints(line);
}

// Determina se um ponto está entre as duas linhas de sombra projetadas pelo retângulo
bool RectangularShadow::isPointBetweenLines(Point p) const
{
    // Calcular o cruzamento como no CircularShadow
    Point line1Start = line1.getStart();
    Point line1End = line1.getEnd();
    Point line2Start = line2.getStart();
    Point line2End = line2.getEnd();

    double cross1 = (line1End.getX() - line1Start.getX()) * (p.getY() - line1Start.getY()) -
                    (line1End.getY() - line1Start.getY()) * (p.getX() - line1Start.getX());
    double cross2 = (line2End.getX() - line2Start.getX()) * (p.getY() - line2Start.getY()) -
                    (line2End.getY() - line2Start.getY()) * (p.getX() - line2Start.getX());

    bool betweenRays = (cross1 >= 0 && cross2 <= 0) || (cross1 <= 0 && cross2 >= 0);
    if (!betweenRays) return false;

    // Verifica se o ponto está dentro da “caixa” delimitada pelos dois segmentos
    double minX = std::min({line1Start.getX(), line1End.getX(), line2Start.getX(), line2End.getX()});
    double maxX = std::max({line1Start.getX(), line1End.getX(), line2Start.getX(), line2End.getX()});
    double minY = std::min({line1Start.getY(), line1End.getY(), line2Start.getY(), line2End.getY()});
    double maxY = std::max({line1Start.getY(), line1End.getY(), line2Start.getY(), line2End.getY()});

    return (p.getX() >= minX && p.getX() <= maxX && p.getY() >= minY && p.getY() <= maxY);
}
