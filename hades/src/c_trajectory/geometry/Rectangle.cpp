//
// Created by caiom on 3/11/2025.
//

#include "Rectangle.h"

#include "Vetop.h"
#include <cmath>

bool Rectangle::collision_test(std::vector<double>& start, std::vector<double>& vet) {
    std::vector<double> goal = {vet[0] + start[0], vet[1] + start[1]};

    double a = 10000000.0;
    if (goal[0] - start[0] != 0) {
        a = (goal[1] - start[1]) / (goal[0] - start[0]);
    }
    if (a == 0) {
        a = 0.0000001;
    }
    std::vector<double> u;
    std::vector<double> v;


    double dist_goal_start = distance_point_squared(start, goal);

    if (a * major[0] + start[1] - a * start[0] <= major[1] && a * major[0] + start[1] - a * start[0] >= minor[1]) {
        u = {major[0] - start[0], a * major[0] + start[1] - a * start[0] - start[1]};
        v = {goal[0] - start[0], goal[1] - start[1]};
        if (angle_vectors_small(u, v) <= M_PI / 2) {
            if (std::pow(major[0] - start[0], 2) +
                std::pow(a * major[0] + start[1] - a * start[0] - start[1], 2) <= dist_goal_start) {
                return true;
            }
        }
    }

    if (a * minor[0] + start[1] - a * start[0] <= major[1] && a * minor[0] + start[1] - a * start[0] >= minor[1]) {
        u = {minor[0] - start[0], a * minor[0] + start[1] - a * start[0] - start[1]};
        v = {goal[0] - start[0], goal[1] - start[1]};
        if (angle_vectors_small(u, v) <= M_PI / 2) {
            if (std::pow(minor[0] - start[0], 2) +
                std::pow(a * minor[0] + start[1] - a * start[0] - start[1], 2) <= dist_goal_start) {
                return true;
            }
        }
    }

    if ((major[1] - start[1] + a * start[0]) / a <= major[0] && (major[1] - start[1] + a * start[0]) / a >= minor[0]) {
        u = {(major[1] - start[1] + a * start[0]) / a - start[0], major[1] - start[1]};
        v = {goal[0] - start[0], goal[1] - start[1]};
        if (angle_vectors_small(u, v) <= M_PI / 2) {
            if (std::pow((major[1] - start[1] + a * start[0]) / a - start[0], 2) +
                std::pow(major[1] - start[1], 2) <= dist_goal_start) {
                return true;
            }
        }
    }

    if ((minor[1] - start[1] + a * start[0]) / a <= major[0] && (minor[1] - start[1] + a * start[0]) / a >= minor[0]) {
        u = {(minor[1] - start[1] + a * start[0]) / a - start[0], minor[1] - start[1]};
        v = {goal[0] - start[0], goal[1] - start[1]};
        if (angle_vectors_small(u, v) <= M_PI / 2) {
            if (std::pow((minor[1] - start[1] + a * start[0]) / a - start[0], 2) +
                std::pow(minor[1] - start[1], 2) <= dist_goal_start) {
                return true;
            }
        }
    }

    return false;
}


std::vector<std::vector<double>> Rectangle::avoid(std::vector<double>& start) {

    std::vector<std::vector<double>> alternatives;
    double adc = 1.0;

    if (start[0] > major[0]) {
        if (start[1] > major[1]) {
            alternatives.push_back({minor[0] - adc - start[0], major[1] + adc - start[1]});
            alternatives.push_back({major[0] + adc - start[0], minor[1] - adc - start[1]});
        } else if (start[1] < minor[1]) {
            alternatives.push_back({minor[0] - adc - start[0], minor[1] - adc - start[1]});
            alternatives.push_back({major[0] + adc - start[0], major[1] + adc - start[1]});
        } else {
            alternatives.push_back({major[0] + adc - start[0], minor[1] - adc - start[1]});
            alternatives.push_back({major[0] + adc - start[0], major[1] + adc - start[1]});
        }
    } else if (start[0] < minor[0]) {
        if (start[1] > major[1]) {
            alternatives.push_back({major[0] + adc - start[0], major[1] + adc - start[1]});
            alternatives.push_back({minor[0] - adc - start[0], minor[1] - adc - start[1]});
        } else if (start[1] < minor[1]) {
            alternatives.push_back({minor[0] - adc - start[0], major[1] + adc - start[1]});
            alternatives.push_back({major[0] + adc - start[0], minor[1] - adc - start[1]});
        } else {
            alternatives.push_back({minor[0] - adc - start[0], minor[1] - adc - start[1]});
            alternatives.push_back({minor[0] - adc - start[0], major[1] + adc - start[1]});
        }
    } else {
        if (start[1] > major[1]) {
            alternatives.push_back({major[0] + adc - start[0], major[1] + adc - start[1]});
            alternatives.push_back({minor[0] - adc - start[0], major[1] + adc - start[1]});
        } else {
            alternatives.push_back({minor[0] - adc - start[0], minor[1] - adc - start[1]});
            alternatives.push_back({major[0] + adc - start[0], minor[1] - adc - start[1]});
        }
    }
    return alternatives;

}