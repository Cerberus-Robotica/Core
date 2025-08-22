//
// Created by caiom on 3/11/2025.
//

#ifndef C_TRAJECTORY_H
#define C_TRAJECTORY_H
#include <deque>
#include <utility>
#include <vector>
#include "geometry/Vetop.h"
#include "geometry/Circle.h"
#include "geometry/Rectangle.h"

class C_trajectory {
    private:
        bool print_each = false;
        bool print_last = false;
        int each_iterations = 1;
        int max_iterations = 1000;
        double small_step = 300;
        double k = 0;
        Rectangle boundaries;


    public:
        C_trajectory(bool print_each, bool print_last, int each_iterations, int max_iterations, double small_step, double k, double boundaryMinor[2], double boundaryMajor[2])
            : boundaries({boundaryMinor[0], boundaryMinor[1]}, {boundaryMajor[0], boundaryMajor[1]}) {
            this->print_each = print_each;
            this->print_last = print_last;
            this->each_iterations = each_iterations;
            this->max_iterations = max_iterations;
            this->small_step = small_step;
            this->k = k;
        }

        std::vector<std::vector<double>> path_find(std::vector<double> start, std::vector<double> goal,
                                                   std::vector<Circle>& obs_circular, std::vector<Rectangle>& obs_retangular);

        std::vector<std::vector<double>> path_connect(std::vector<double>& start, std::vector<double>& goal,
            std::vector<Circle>& obs_circular, std::vector<Rectangle>& obs_retangular);

        std::vector<std::vector<double>> path_single(std::vector<double>& start, std::vector<double>& goal,
             std::vector<Circle>& obs_circular, std::vector<Rectangle>& obs_retangular);

        std::vector<std::vector<double>> path_single_inverted(std::vector<double>& start, std::vector<double>& goal,
            std::vector<Circle>& obs_circular, std::vector<Rectangle>& obs_retangular);

        void retrace_path(auto& trajectory, auto& obs_circular, auto& obs_retangular);

        std::vector<std::vector<double>> smoothing(auto& trajectory, auto& obs_circular, auto& obs_retangular);

        void invert_trajectory(auto& trajectory);

        std::vector<double> interference(auto point, auto& obs_circular, auto& obs_retangular);

        bool collision_test(auto& start, auto& goal, auto& obs_circular, auto& obs_retangular);

        void find_blocking_circles(auto& trajectory, auto& goal, auto& new_obs_circular, auto& obs_circular);
        void remove_invalide_points(auto& points, auto& trajectory);
        void remove_empty_alternatives(auto& alternatives, auto& trajectory);
        void shorten_path(auto& trajectory_1, auto& trajectory_2, auto& obs_circular, auto& obs_retangular);
        std::deque<std::vector<double>> c_point(auto& start, auto& goal, auto& obs_circular, auto& obs_retangular);

};



#endif //C_TRAJECTORY_H
