//
// Created by caiom on 3/11/2025.
//

#include "Vetop.h"
#include <cmath>
#include <deque>
#include <vector>
#include <algorithm>


double distance_line_point(std::vector<double>& start, std::vector<double>& goal, std::vector<double>& ponto) {
    double a = 10000000;
    if (goal[0] != start[0]) {
        a = (goal[1] - start[1]) / (goal[0] - start[0]);
    }
    double c = start[1] - start[0] * a;
    return (fabs(a * ponto[0] - ponto[1] + c) / sqrt(pow(a, 2) + 1));
}

double distance_point(std::vector<double>& ponto_1, std::vector<double>& ponto_2) {
    return sqrt(pow(ponto_1[0] - ponto_2[0], 2) + pow(ponto_1[1] - ponto_2[1], 2));
}

double distance_point_squared(std::vector<double>& ponto_1, std::vector<double>& ponto_2) {
    return (pow(ponto_1[0] - ponto_2[0], 2) + pow(ponto_1[1] - ponto_2[1], 2));
}

std::vector<double> ortogonal_projection(std::vector<double>& u, std::vector<double>& v) {
    double norm_u_sq = u[0] * u[0] + u[1] * u[1];
    if (norm_u_sq == 0) {
        return {0, 0};
    }
    double mod = (u[0] * v[0] + u[1] * v[1])/norm_u_sq;
    return {u[0]*mod, u[1]*mod};
}

std::vector<double> normalize(double& alpha, std::vector<double>& vet) {
    double norm_vet = norm(vet);
    if (norm_vet == 0) {
        return {0, 0};
    }
    return {alpha * vet[0] / norm_vet, alpha * vet[1] / norm_vet};
}

double angle_vectors_small(std::vector<double>& u, std::vector<double>& v) {
    double norm_u = norm(u);
    double norm_v = norm(v);
    if (norm_u == 0 or norm_v == 0) {
        return 0;
    }
    double cosine = (u[0] * v[0] + u[1] * v[1]) / (norm_u * norm_v);
    cosine = std::clamp(cosine, -1.0, 1.0);
    auto m = acos(cosine);
    return acos(cosine);
}

double angle_vectors_acw(std::vector<double>& u, std::vector<double>& v) {
    double theta = atan2(u[1], u[0]) - atan2(v[1], v[0]);
    if (theta > M_PI) {
        theta = theta - 2 * M_PI;
    }
    if (theta < -M_PI) {
        theta = theta + 2 * M_PI;
    }
    return theta;
    }

void rotate_vector(double& ang, std::vector<double>& vet) {
    double c = cos(ang);
    double s = sin(ang);
    vet = {vet[0] * c + vet[1] * s, -vet[0] * s + vet[1] * c};
}

void angle_sort(std::vector<double>& start, std::vector<double>& goal, std::deque<std::vector<double>>& pontos) {
    std::vector<double> angulos;
    auto vetor_direto = subtract(goal, start);
    for (auto ponto : pontos) {
        auto vetor = subtract(ponto, start);
        angulos.push_back(fabs(angle_vectors_small(vetor, vetor_direto)));
    }
    for (int i = 0 ; i < size(angulos) ; i++) {
        for (int j = 0; j < size(angulos) - i - 1; j++) {
            if (angulos[j] > angulos[j + 1]) {
                std::swap(angulos[j], angulos[j + 1]);
                std::swap(pontos[j], pontos[j + 1]);
            }
        }
    }
}

std::vector<double> subtract(std::vector<double> goal, std::vector<double> start) {
    return {goal[0] - start[0], goal[1] - start[1]};
}

double norm(std::vector<double>& point) {
    return sqrt(pow(point[0], 2) + pow(point[1], 2));
}