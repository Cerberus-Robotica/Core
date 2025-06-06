//
// Created by caiom on 3/11/2025.
//

#ifndef CIRCLE_H
#define CIRCLE_H
#include <vector>


class Circle {

public:
    std::vector<double> center = {0, 0};
    double radius = 0;

    Circle(std::vector<double> center, double radius) {
        this->center = center;
        this->radius = radius;
    }

    bool collision_test(std::vector<double>& start, std::vector<double>& vet);
    std::vector<std::vector<double>> avoid(std::vector<double>& start);
};



#endif //CIRCLE_H
