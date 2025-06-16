//
// Created by caiom on 3/11/2025.
//

#ifndef VETOP_H
#define VETOP_H
#include <deque>
#include <vector>




double distance_line_point(std::vector<double>& start, std::vector<double>& goal, std::vector<double>& ponto);
double distance_point(std::vector<double>& ponto_1, std::vector<double>& ponto_2);
double distance_point(double point_1[2], double point_2[2]);
double distance_point_squared(std::vector<double>& ponto_1, std::vector<double>& ponto_2);
std::vector<double> ortogonal_projection(std::vector<double>& u, std::vector<double>& v);
std::vector<double> normalize(double alpha, std::vector<double>& vet);
double angle_vectors_small(std::vector<double>& u, std::vector<double>& v);
double angle_vectors_small(std::vector<double>&& u, std::vector<double>&& v);
double angle_vectors_acw(std::vector<double>& u, std::vector<double>& v);
void rotate_vector(double& ang, std::vector<double>& vet);
void angle_sort(std::vector<double>& start, std::vector<double>& goal, std::deque<std::vector<double>>& pontos);
std::vector<double> subtract(std::vector<double> goal, std::vector<double> start);
double norm(std::vector<double>& point);





#endif //VETOP_H
