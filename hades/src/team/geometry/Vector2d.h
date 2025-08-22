//
// Created by caiu on 21/08/25.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <vector>
#include <cmath>

class Vector2d {
private:
	double x;
	double y;

public:
	Vector2d(double x = 0.0, double y = 0.0) : x(x), y(y) {}

	void setXY(double x, double y);
	void setX(double x);
	void setY(double y);

	double getX() const;
	double getY() const;
	std::vector<double> getVector() const;
	double getNorm() const;
};

#endif // VECTOR2D_H
