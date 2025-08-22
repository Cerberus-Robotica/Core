//
// Created by caiu on 21/08/25.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <vector>
#include <cmath>
#include "Point.h"

class Vector2d {
private:
	double x;
	double y;

public:
	Vector2d(double x = 0.0, double y = 0.0) : x(x), y(y) {}
	Vector2d(Point p1, Point p0) : x(p1.getX() - p0.getX()), y(p1.getY() - p0.getY()) {}

	void setXY(double x, double y);
	void setX(double x);
	void setY(double y);

	double getX() const;
	double getY() const;
	std::vector<double> getVector() const;
	double getNorm() const;
	Vector2d getNormalized(double alpha) const;
	double getDotProduct(Vector2d v) const;
	double getAngleBetween(Vector2d v) const;
	Vector2d getRotated(double ang) const;
};

#endif // VECTOR2D_H
