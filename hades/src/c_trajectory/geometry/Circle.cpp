//
// Created by caiom on 3/11/2025.
//

#include "Circle.h"
#include "Vetop.h"
#include <cmath>
#include <vector>

bool Circle::collision_test(std::vector<double>& start, std::vector<double>& vet) {
    std::vector<double> goal = {vet[0] + start[0], vet[1] + start[1]};
    auto a = distance_line_point(start, goal, center);
    if (distance_line_point(start, goal, center) > radius) {
        return false;
    }

    auto v = subtract(center, start);

    double theta = angle_vectors_small(vet, v);

    if (theta < 0) {
        theta = 2*M_PI/2 + theta;
    }
    if (theta > M_PI/2) {
        return false;
    }

    auto v1 = ortogonal_projection(vet, v);
    auto R = subtract(v, v1);
    double r = norm(R);
    double w = sqrt(fabs(radius*radius - r*r));
    if (norm(v1) > norm(vet) + w) {
        return false;
    }

    return true;
}

std::vector<std::vector<double>> Circle::avoid(std::vector<double>& start) {
    double diversion = radius*1.05;

    double k_norm = sqrt(fabs(pow(distance_point(start, center), 2) - diversion*diversion));

    double theta = atan2((center[1] - start[1]), (center[0] - start[0]));
    double dif = atan2(diversion, k_norm);
    double theta_1 = theta + dif;
    double theta_2 = theta - dif;

    if (k_norm < radius) {
        k_norm = radius;
    }

    double distancia_do_centro = distance_point(start, center);
    if (distancia_do_centro < diversion) {
        double correction = atan2(distancia_do_centro - radius, k_norm);
        theta_1 += correction;
        theta_2 -= correction;
    }
    std::vector<double> vet_1 = {k_norm * cos(theta_1), k_norm * sin(theta_1)};
    std::vector<double> vet_2 = {k_norm * cos(theta_2), k_norm * sin(theta_2)};
    return {vet_1, vet_2};


}