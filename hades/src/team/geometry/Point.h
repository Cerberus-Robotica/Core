//
// Created by caiu on 21/08/25.
//

#ifndef POINT_H
#define POINT_H
#include <vector>

class Point {
private:
  double x;
  double y;
public:
  Point(double x, double y){
    this->x = x;
    this->y = y;
  }
  Point(int x, int y){
    this->x = x;
    this->y = y;
  }
  Point(double x, int y){
    this->x = x;
    this->y = y;
  }
  Point(int x, double y){
    this->x = x;
    this->y = y;
  }

  void setXY(double x, double y);
  void setX(double x);
  void setY(double y);
  double getX() const;
  double getY() const;
  std::vector<double> getVector() const;
  double getDistanceTo(const Point& p) const;
};



#endif //POINT_H
