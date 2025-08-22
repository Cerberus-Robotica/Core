//
// Created by caiu on 21/08/25.
//

#include "Vector2d.h"

void Vector2d::setXY(double x, double y) {
    this->x = x;
    this->y = y;
}

void Vector2d::setX(double x) {
    this->x = x;
}

void Vector2d::setY(double y) {
    this->y = y;
}

double Vector2d::getX() const {
    return x;
}

double Vector2d::getY() const {
    return y;
}

std::vector<double> Vector2d::getVector() const {
    return {x, y};
}

double Vector2d::getNorm() const {
    return std::sqrt(x*x + y*y);
}