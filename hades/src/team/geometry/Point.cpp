//
// Created by caiu on 21/08/25.
//

#include "Point.h"
#include <math.h>

void Point::setXY(double x, double y) {
    this->x = x;
    this->y = y;
}

void Point::setX(double x) {
    this->x = x;
}

void Point::setY(double y) {
    this->y = y;
}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

std::vector<double> Point::getVector() const {
    return {x, y};
}

double Point::getDistanceTo(const Point& p) const {
  return sqrt(pow(p.getX() - x, 2) + pow(p.getY() - y, 2));
}