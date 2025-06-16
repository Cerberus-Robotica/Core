//
// Created by caiom on 3/11/2025.
//

#include "C_trajectory.h"
#include "geometry/Vetop.h"
#include <deque>
#include <cmath>
#include <iostream>
#include <bits/algorithmfwd.h>


using namespace std;

vector<vector<double>> C_trajectory::path_find(std::vector<double>& start, std::vector<double>& goal,
            std::vector<Circle>& obs_circular, std::vector<Rectangle>& obs_retangular) {

/*Encontra uma trajetoria a partir de 3 estratégias.
Se é encontrada uma trajetoria na primeira estratégia, a função retorna essa trajetoria,
sem calcular com as outras estratégias.

trajetoria = estrategia1()
if trajetoria == []:
    trajetoria = estrategia2()
...
return trajetoria

A funcao tambem adiciona um primeiro ponto a trajetoria caso o ponto inicial (start)
esteja colidindo com um obstaculo.


start : Inicio da trajetoria.
goal : Final da trajetoria.
obscirculares : Obstaculos Circulares.
obsret : Obstaculos Retangulares.
maxiteracoes : Maximo de iteracoes possiveis
menorpasso : Menor distancia entre dois pontos consecutivos
*/

    vector<vector<double>> trajectory = {};

    vector<double> start_new = interference(start, obs_circular, obs_retangular);
    vector<double> goal_new = interference(goal, obs_circular, obs_retangular);

    if (start_new[0] != start[0] || start_new[1] != start[1]) {
        trajectory.push_back(start);
    }


    vector<vector<double>> mid_path = path_connect(start_new, goal_new, obs_circular, obs_retangular);
    if (size(mid_path) == 0) {
        mid_path = path_single(start_new, goal_new, obs_circular, obs_retangular);
        if (size(mid_path) == 0) {
            mid_path = path_single_inverted(start_new, goal_new, obs_circular, obs_retangular);
            if (size(mid_path) == 0) {
                trajectory.push_back(start);
                trajectory.push_back(interference(start, obs_circular, obs_retangular));
            }
        }
    }
    if (size(mid_path) > 0) {
        trajectory.insert(trajectory.end(), mid_path.begin(), mid_path.end());
        trajectory = smoothing(trajectory, obs_circular, obs_retangular);
    }



    return trajectory;
}

vector<vector<double>> C_trajectory::path_connect(std::vector<double>& start, std::vector<double>& goal,
        std::vector<Circle>& obs_circular, std::vector<Rectangle>& obs_retangular) {
    /*
    Cria uma trajetoria a partir de duas trajetorias, uma partindo do ponto inicial e outra do ponto final.
    As trajetorias tentam encontrar uma a outra.
    A busca termina quando um dos pontos de uma consegue alcancar o ultimo ponto da outra.

    start : Inicio da trajetoria.
    goal : Final da trajetoria.
    obscirculares : Obstaculos Circulares.
    obsret : Obstaculos Retangulares.
    maxiteracoes : Maximo de iteracoes possiveis
    menorpasso : Menor distancia entre dois pontos consecutivos
    */

    vector<vector<double>> trajectory_1 = {start};
    vector<vector<double>> trajectory_2 ={goal};
    deque<deque<vector<double>>> alternatives_1 = {};
    deque<deque<vector<double>>> alternatives_2 = {};
    vector<Circle> new_obs_circular = obs_circular;
    vector<double> goal1;
    vector<double> goal2;
    int iteration = 0;
    deque<vector<double>> found;





    while (collision_test(trajectory_1.back(), trajectory_2.back(), new_obs_circular, obs_retangular)) {

        goal1 = trajectory_2.back();
        find_blocking_circles(trajectory_1, trajectory_2.back(), new_obs_circular, obs_circular);
        found = c_point(trajectory_1.back(), trajectory_2.back(), new_obs_circular, obs_retangular);
        alternatives_1.push_back(found);
        remove_invalide_points(alternatives_1.back(), trajectory_1);
        remove_empty_alternatives(alternatives_1, trajectory_1);
        if (size(alternatives_1) == 0) {
            return vector<vector<double>>{};
        }

        angle_sort(trajectory_1.back(),trajectory_2.back(), alternatives_1.back());
        trajectory_1.push_back(alternatives_1.back().front());
        alternatives_1.back().pop_front();

        shorten_path(trajectory_1, trajectory_2, new_obs_circular, obs_retangular);

        if (each_iterations != 0) {
            if (iteration%each_iterations && print_each) {
                std::cout << "Iteration: " << iteration << std::endl;
            }
        }


        if (!collision_test(trajectory_1.back(), trajectory_2.back(), new_obs_circular, obs_retangular)) {
            break;
        }

        goal2 = trajectory_1.back();
        find_blocking_circles(trajectory_2, goal2, new_obs_circular, obs_circular);
        found = c_point(trajectory_2.back(), trajectory_1.back(), new_obs_circular, obs_retangular);
        alternatives_2.push_back(found);
        remove_invalide_points(alternatives_2.back(), trajectory_2);
        remove_empty_alternatives(alternatives_2, trajectory_2);
        if (size(alternatives_2) == 0) {
            return vector<vector<double>> {};
        }

        angle_sort(trajectory_2.back(),trajectory_1.back(), alternatives_2.back());

        trajectory_2.push_back(alternatives_2.back().front());
        alternatives_2.back().pop_front();

        shorten_path(trajectory_2, trajectory_1, new_obs_circular, obs_retangular);
        if (each_iterations != 0) {
            if (iteration%each_iterations && print_each) {
                std::cout << "Iteration: " << iteration << std::endl;
            }
        }
        iteration++;
    }

    invert_trajectory(trajectory_2);
    trajectory_1.insert(trajectory_1.end(), trajectory_2.begin(), trajectory_2.end());

    if (size(trajectory_1) > 2) {
        retrace_path(trajectory_1, obs_circular, obs_retangular);
    }

    if (print_last) {
    }

    //smoothing(trajectory_1, obs_circular, obs_retangular);

    return trajectory_1;

}


std::vector<std::vector<double>> C_trajectory::path_single(std::vector<double>& start, std::vector<double>& goal,
             std::vector<Circle>& obs_circular, std::vector<Rectangle>& obs_retangular) {
    vector<vector<double>> trajectory = {start};
    deque<deque<vector<double>>> alternatives = {};
    vector<Circle> new_obs_circular = obs_circular;
    int iteration = 0;
    deque<vector<double>> found;

    while (collision_test(trajectory.back(), goal, new_obs_circular, obs_retangular)) {
        found = c_point(trajectory.back(), goal, new_obs_circular, obs_retangular);
        alternatives.push_back(found);
        remove_invalide_points(alternatives.back(), trajectory);
        remove_empty_alternatives(alternatives, trajectory);
        if (size(alternatives) == 0) {
            return vector<vector<double>>{};
        }
        angle_sort(trajectory.back(),goal, alternatives.back());
        trajectory.push_back(alternatives.back().front());
        alternatives.back().pop_front();

        iteration++;
        if (each_iterations != 0) {
            if (iteration%each_iterations && print_each) {
                std::cout << "Iteration: " << iteration << std::endl;
            }
        }
        if (iteration >= max_iterations) {
            return {};
        }
    }
    trajectory.push_back(goal);
    if (size(trajectory) > 2) {
        retrace_path(trajectory, obs_circular, obs_retangular);
    }
    return trajectory;
}

std::vector<std::vector<double>> C_trajectory::path_single_inverted(std::vector<double>& start, std::vector<double>& goal,
            std::vector<Circle>& obs_circular, std::vector<Rectangle>& obs_retangular) {
    auto trajectory = path_single(start, goal, obs_circular, obs_retangular);
    invert_trajectory(trajectory);
    return trajectory;
}


bool C_trajectory::collision_test(auto& start, auto& goal,
    auto& obs_circular, auto& obs_retangular) {
    vector<double> vet = {goal[0] - start[0], goal[1] - start[1]};
    for (Circle c: obs_circular) {
        if (c.collision_test(start, vet)) {
            return true;
        }
    }
    for (Rectangle r: obs_retangular) {
        if (r.collision_test(start, vet)) {
            return true;
        }
    }
    return false;
}

void C_trajectory::find_blocking_circles(auto& trajectory, auto& goal, auto& new_obs_circular, auto& obs_circular) {
    if (k == 0 || size(trajectory) < 2) {
        return;
    }
    if (size(new_obs_circular) > size(obs_circular) + 3) {
        new_obs_circular.erase(new_obs_circular.end() - 3);
        new_obs_circular.erase(new_obs_circular.end() - 3);
    }
    auto n = subtract(trajectory[size(trajectory) - 1], trajectory[size(trajectory) - 2]);
    vector<double> c_vet = {};
    c_vet.push_back(1.2 *-n[1]/norm(n)/k);
    c_vet.push_back(1.2 *n[0]/norm(n)/k);

    vector<double> ponto_1 = {-1*c_vet[0] + trajectory.back()[0], -1*c_vet[1] + trajectory.back()[1]};
    vector<double> ponto_2 = {c_vet[0] + trajectory.back()[0], c_vet[1] + trajectory.back()[1]};
    if (distance_point_squared(ponto_1, trajectory[0]) < pow(1/k, 2) || distance_point_squared(ponto_1, goal) < pow(1/k, 2)) {
        return;
    }
    if (distance_point_squared(ponto_2, trajectory[0]) < pow(1/k, 2) || distance_point_squared(ponto_2, goal) < pow(1/k, 2)) {
        return;
    }
    Circle circle_1(ponto_1, 1/k);
    new_obs_circular.push_back(circle_1);
    Circle circle_2(ponto_2, 1/k);
    new_obs_circular.push_back(circle_2);
}


std::deque<std::vector<double>> C_trajectory::c_point(auto& start, auto& goal, auto& obs_circular, auto& obs_rectangular) {
    deque<vector<double>> unchecked = {subtract(goal, start)};
    deque<vector<double>> approved;
    vector<bool> collided_circle;
    vector<bool> collided_rectangular;
    bool collided = false;

    double boundarieMinorVector[2] = {boundaries.minor[0] - start[0], boundaries.minor[1] - start[1]};
    double boundarieMajorVector[2] = {boundaries.major[0] - start[0], boundaries.major[1] - start[1]};

    for (int i = 0; i < size(obs_circular); i++) {
        collided_circle.push_back(false);
    }
    for (int i = 0; i < size(obs_rectangular); i++) {
        collided_rectangular.push_back(false);
    }


    while (size(unchecked) != 0) {
        collided = false;
        for (int i = 0; i < size(obs_circular); i++) {
            if (obs_circular[i].collision_test(start, unchecked[0])) {
                collided = true;
                if (!collided_circle[i]) {
                    auto avoided = obs_circular[i].avoid(start);
                    unchecked.insert(unchecked.end(), avoided.begin(), avoided.end());
                    collided_circle[i] = true;
                }
            }
        }

        for (int i = 0; i < size(obs_rectangular); i++) {
            if (obs_rectangular[i].collision_test(start, unchecked[0])) {
                collided = true;
                if (!collided_rectangular[i]) {
                    auto avoided = obs_rectangular[i].avoid(start);
                    unchecked.insert(unchecked.end(), avoided.begin(), avoided.end());
                    collided_rectangular[i] = true;
                }
            }
        }
        if (unchecked[0][0] < boundarieMinorVector[0] || unchecked[0][0] > boundarieMajorVector[0]){
            collided = true;
        }
        if (unchecked[0][1] < boundarieMinorVector[1] || unchecked[0][1] > boundarieMajorVector[1]){
            collided = true;
        }
        if (!collided) {
            approved.push_back(unchecked[0]);
        }
        unchecked.pop_front();
    }

    for (auto& vet :approved) {
        vet[0] = vet[0] + start[0];
        vet[1] = vet[1] + start[1];
    }
    return approved;
}



std::vector<double> C_trajectory::interference(auto point, auto& obs_circular, auto& obs_retangular) {
    bool collided = false;
    auto backup = point;
    vector<double> a;
    double ang = 0;
    double adcret = 0;
    int tries = 0;

    while(true) {
        collided = false;

        for (Circle c : obs_circular) {
            if (distance_point(point, c.center) <= c.radius*1.05){
                if (point[0] - c.center[0] == 0 || point[1] - c.center[1] == 0) {
                    point[0] = 1;
                }
                a = subtract(point, c.center);
                rotate_vector(ang, a);
                point = {a[0] / norm(a) * (c.radius*1.1) + c.center[0], a[1]/norm(a)*(c.radius*1.1) + c.center[1]};
                ang += M_PI/100;
                collided = true;

            }
        }
        for (Rectangle ret : obs_retangular){
            if (point[0] >= ret.minor[0] && point[0] <= ret.major[0] && point[1] >= ret.minor[1] && point[1] <= ret.major[1]) {
                double distx = 0;

                if (fabs(point[0] - ret.minor[0]) < fabs(point[0] - ret.major[0])) {
                    distx = abs(point[0] - ret.minor[0]);
                }
                else {
                    distx = fabs(point[0] - ret.major[0]);
                }
                if (distx < fabs(point[1] - ret.minor[1]) && distx < fabs(point[1] - ret.major[1])) {
                    if (fabs(point[0] - ret.minor[0]) < fabs(point[0] - ret.major[0])) {
                        distx = fabs(point[0] - ret.minor[0]);
                        point[0] = ret.minor[0] - adcret;
                    }
                    else {
                        distx = fabs(point[0] - ret.major[0]);
                        point[0] = ret.major[0] + adcret;
                    }
                }

                double disty = 0;

                if (fabs(point[1] - ret.minor[1]) < fabs(point[1] - ret.major[1])) {
                    disty = abs(point[1] - ret.minor[1]);
                }
                else {
                    disty = fabs(point[1] - ret.major[1]);
                }
                if (disty < fabs(point[0] - ret.minor[0]) && disty < fabs(point[0] - ret.major[0])) {
                    if (fabs(point[1] - ret.minor[1]) < fabs(point[1] - ret.major[1])) {
                        disty = fabs(point[1] - ret.minor[1]);
                        point[1] = ret.minor[1] - adcret;
                    }
                    else {
                        disty = fabs(point[1] - ret.major[1]);
                        point[1] = ret.major[1] + adcret;
                    }
                }
                adcret += small_step;
                collided = true;
            }
        }
        if (point[0] < boundaries.minor[0] || point[0] > boundaries.major[0]){
            std::clamp(point[0], boundaries.minor[0], boundaries.major[0]);
            collided = true;
        }
        if (point[1] < boundaries.minor[1] || point[1] > boundaries.major[1]){
            std::clamp(point[1], boundaries.minor[1], boundaries.major[1]);
            collided = true;
        }

        if (!collided) {
            break;
        }
        tries += 1;
        if (tries >= 100) {
            return backup;
        }
    }
    return point;
}

void C_trajectory::remove_invalide_points(auto& points, auto& trajectory) {
    int i = 0;
    while (i < size(points)) {
        if (size(trajectory) < 2) {
            break;
        }
        auto u = subtract(points[i], trajectory.back());
        auto v = subtract(trajectory.back(), trajectory[size(trajectory) - 2]);
        auto m = angle_vectors_small(u, v);
        if (angle_vectors_small(u, v) > 2.0944) {
            points.erase(points.begin() + i);
            continue;
        }
        for (auto j :trajectory) {
            if (distance_point(points[i], j) < small_step / 2){
                points.erase(points.begin() + i);
                i += -1;
                break;
            }
        }
        i += 1;
    }
}



void C_trajectory::remove_empty_alternatives(auto& alternatives, auto& trajectory) {
    if (size(alternatives) == 0) {
        alternatives = {};
        return;
    }

    while (size(alternatives.back()) == 0) {
        alternatives.pop_back();
        trajectory.pop_back();
        if (size(alternatives) == 0) {
            alternatives = {};
            return;
        }
    }
}

void C_trajectory::invert_trajectory(auto& trajectory) {
    vector<vector<double>> caminho;
    for (int i = size(trajectory) - 1; i >= 0; i = i - 1) {
        caminho.push_back(trajectory[i]);
    }
    trajectory = caminho;
}


void C_trajectory::shorten_path(auto& trajectory_1, auto& trajectory_2, auto& obs_circular, auto& obs_retangular) {
    for (int i = 0; i < size(trajectory_2); i++) {
        if (!collision_test(trajectory_1.back(), trajectory_2[i], obs_circular, obs_retangular)) {
            trajectory_2.resize(i + 1);
            break;
        }
    }
}


void C_trajectory::retrace_path(auto& trajectory, auto& obs_circular, auto& obs_retangular) {
    /*
    Retraca uma trajetoria retirando pontos desnecessarios.
    Sendo um trecho da trajetoria [A, B, C], se AC nao ha colisao, retira-se B.

    trajetoria : Trajetoria a ser retracada.
    obscirculares : Obstaculos circulares.
    obsret : Obstaculos Retangulares.
    */


    if (size(trajectory) < 4) {
        return;
    }
    bool retraced = false;
    vector<double> vet;
    //Id do ponto sendo testado
    int i = 0;
    //Complementar do Id do segundo ponto que forma o par
    int j = 0;

    //Itera por todos os pontos da trajetoria
    for (i = 0 ; i < size(trajectory) - 1; i++) {
        j = 0;
        //Para cada ponto da trajetoria, forma um par com todos os outros
        while (size(trajectory) - 1 - j > i) {
            //Se a trajetoria for retracada (modificada), refaz a verificacao
            if (retraced) {
                j = 0;
            }
            retraced = false;

            //Se nao colidir A com C, retira todos os pontos entre A e C. Sendo A = trajetoria[i] e C = trajetoria[len(trajetoria) - 1 - j]
            vet = subtract(trajectory[size(trajectory) - 1 - j], trajectory[i]);
            if (!collision_test(trajectory[i], trajectory[size(trajectory) - 1 - j], obs_circular, obs_retangular)) {
                for (int a = 0; a < size(trajectory) - j - 1 - 1 - i; a++) {
                    trajectory.erase(trajectory.begin() + i + 1);
                }
                retraced = true;
            }
            j += 1;
        }
    }
}

vector<vector<double>> C_trajectory::smoothing(auto& trajectory, auto& obs_circular, auto& obs_retangular) {
    if (k == 0) {
        return trajectory;
    }
    Circle new_obs_circular({0, 0}, 0);
    double radius = 1/k;
    auto new_trajectory = trajectory;
    vector<double> x = {1, 0};
    int i = 0;
    while (i < size(new_trajectory) - 2) {
        auto AC = subtract(new_trajectory[i + 2], new_trajectory[i]);
        auto BC = subtract(new_trajectory[i + 2], new_trajectory[i + 1]);
        auto BA = subtract(new_trajectory[i], new_trajectory[i + 1]);

        auto ang_ba = angle_vectors_acw(BA, x);
        if (ang_ba < 0) {
            ang_ba = ang_ba + 2*M_PI;
        }
        auto ang_bc = angle_vectors_acw(BC, x);
        if (ang_bc < 0) {
            ang_bc = ang_bc + 2*M_PI;
        }

        auto ang = angle_vectors_acw(BA, BC)/2 + angle_vectors_acw(BC, x);
        ang = angle_vectors_acw(BC, BA)/2 + ang_ba;
        if (ang_ba > ang_bc) {
            ang = angle_vectors_acw(BA, BC)/2 + ang_bc;
        }

        vector<double> n = {cos(ang), sin(ang)};
        auto length = radius*1.1;
        n = normalize(length, n);
        vector<double> c = {n[0] + new_trajectory[i + 1][0], n[1] + new_trajectory[i + 1][1]};
        new_obs_circular.center = c;
        new_obs_circular.radius = radius;


        if (distance_point_squared(c, new_trajectory[i]) < pow(radius, 2)
            || distance_point_squared(c, new_trajectory[i + 1]) < pow(radius, 2)){
            i += 1;
            continue;
            }

        auto total_obs_circular = obs_circular;
        total_obs_circular.push_back(new_obs_circular);

        double boundariesMinor[2] = {boundaries.minor[0], boundaries.minor[1]};
        double boundariesMajor[2] = {boundaries.major[0], boundaries.major[1]};
        C_trajectory temp(false, false, 0, int(max_iterations/10), radius/2, 0, boundariesMinor, boundariesMajor);
        auto between = temp.path_connect(new_trajectory[i], new_trajectory[i + 1], total_obs_circular, obs_retangular);
        if (size(between) > 2) {
            between.erase(between.begin());
            between.pop_back();
            new_trajectory.insert(new_trajectory.begin() + i + 1, between.begin(), between.end());
        }
        i += 1;

        if (i > max_iterations) {
            return trajectory;
        }
    }
    return new_trajectory;
}