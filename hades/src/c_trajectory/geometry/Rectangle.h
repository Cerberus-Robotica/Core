//
// Created by caiom on 3/11/2025.
//

#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <vector>
#include "Vetop.h"


class Rectangle {
public:
    std::vector<double> minor = {0, 0};
    std::vector<double> major = {0, 0};

    Rectangle(const std::vector<double>& minor, const std::vector<double>& major) {
        this->minor = minor;
        this->major = major;
    }

    Rectangle(std::vector<double>&& minor, std::vector<double>&& major) {
        this->minor = minor;
        this->major = major;
    }

    bool collision_test(std::vector<double>& start, std::vector<double>& vet);
    std::vector<std::vector<double>> avoid(std::vector<double>& start);
};



#endif //RECTANGLE_H
