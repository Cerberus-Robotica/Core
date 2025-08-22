//
// Created by caiu on 21/08/25.
//

#include "Vector2d.h"
#include <algorithm>

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

Vector2d Vector2d::getNormalized(double alpha = 1) const {
	double norm = getNorm();
    if (norm == 0) {
        return {0, 0};
    }
    return {alpha * x / norm, alpha * y / norm};
}

double Vector2d::getDotProduct(Vector2d v) const {
	return x*v.x + y*v.y;
}

double Vector2d::getAngleBetween(Vector2d v) const {
	return acos(std::clamp(getDotProduct(v) / (v.getNorm() * getNorm() + 1e-6), -1.0, 1.0));
}

Vector2d Vector2d::getRotated(double ang) const {
	return {x * cos(ang) - y * sin(ang), x * sin(ang) + y * cos(ang)};
}