//
// Created by caiom on 4/4/2025.
//

#include "Robot_controller.h"
#include <iostream>
#include <thread>
#include "../c_trajectory/C_trajectory.h"
#include <math.h>
#include "../include/handlers.hpp"
#include <chrono>
#include <numeric>
#include <set>
#include <unordered_set>


void Robot_controller::start(team_info* team) {
    han.new_ia.robots[id].kick_speed_x = 3;
    this->team = team;
    terminate = false;
    offline_counter = 0;
    std::cout << "iniciado " << id << std::endl;
    team->roles[id] = 0;
    std::thread t(&Robot_controller::loop, this);
    t.detach();
}

void Robot_controller::stop() {
    team->active_robots[id] = false;
    team->roles[id] = -1;
    terminate = true;
}

void Robot_controller::loop() {
    auto t0 = std::chrono::steady_clock::now();
    int cycles = 0;
    while (not terminate) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (last_time_stamp == han.new_ia.timestamp) {
            continue;
        }
        auto t1 = std::chrono::steady_clock::now();
        receive_vision();
        //std::cout << id << ", " << team->roles[id] << std::endl;
        //std::cout << world.enemies[0].detected<< std::endl;
        check_connection();
        role_table();
        publish();
        cycles++;
        std::chrono::duration<double> delta = t1 - t0;
        t0 = std::chrono::steady_clock::now();
        delta_time = delta.count();
        if (delta_time == 0 or delta_time < 0) {
            std::cout << "??" << std::endl;
            delta_time = 1/60;
        }
        if (delta_time != 0) {
            continue;
            double frequency = cycles/delta_time;
            if (cycles > 1000) {
                cycles = 0;
                t0 = std::chrono::steady_clock::now();
            }
            if (frequency < 30) {
                std::cout << id << std::endl;
                std::cout << "Frequencia: " << frequency << std::endl;
                std::cout << cycles << std::endl;
                std::cout << delta_time << std::endl;
            }
        }
    }
    std::cout << "Encerrado " << id << std::endl;
}




std::vector<std::vector<double>> Robot_controller::find_trajectory(double start[2], double goal[2], bool avoid_ball = true, bool is_ball = false) {
    C_trajectory pf(false, false, 0, 1000, 50, 0);
    std::vector<double> double_start = {start[0], start[1]};
    std::vector<double> double_goal = {goal[0], goal[1]};

    std::vector<circle> obs_circular = {};
    std::vector<rectangle> obs_rectangular = {};

    //rectangle r = field.their_defense_area;
    //obs_rectangular.push_back(r);

    //add static ball to obstacles according to avoidance radius
    if (avoid_ball) {
        circle c({world.ball_pos[0], world.ball_pos[1]}, ball_avoidance_radius + radius);
        obs_circular.push_back(c);
    }

    //add static allies to obstacles
    for (int i = 0; i < size(world.allies) ; i++) {
        if (!world.allies[i].detected || i == id || is_ball) {
            continue;
        }
        circle c({world.allies[i].pos[0], world.allies[i].pos[1]}, radius);
        obs_circular.push_back(c);
    }

    //add static enemies to obstacles
    for (int i = 0; i < size(world.enemies) ; i++) {
        if (!world.enemies[i].detected) {
            continue;
        }
        circle c({world.enemies[i].pos[0], world.enemies[i].pos[1]}, radius);
        obs_circular.push_back(c);
    }

    if (team->roles[id] != 0 && !is_ball) {
        rectangle r({world.our_defese_area[0][0] - radius, world.our_defese_area[0][1] - radius}, {world.our_defese_area[1][0] + radius, world.our_defese_area[1][1] + radius});
        obs_rectangular.push_back(r);
    }

    if (!is_ball) {
        rectangle r({world.their_defese_area[0][0] - radius, world.their_defese_area[0][1] - radius}, {world.their_defese_area[1][0] + radius, world.their_defese_area[1][1] + radius});
        obs_rectangular.push_back(r);
    }

    //goal fisical barrier
    if (!is_ball) {
        rectangle r({world.back_fisical_left_goal[0][0] - radius, world.back_fisical_left_goal[0][1] - radius}, {world.back_fisical_left_goal[1][0] - radius, world.back_fisical_left_goal[1][1] + radius});
        obs_rectangular.push_back(r);
        r.minor = {world.left_fisical_left_goal[0][0] - radius, world.left_fisical_left_goal[0][1] - radius};
        r.major = {world.left_fisical_left_goal[1][0] - radius, world.left_fisical_left_goal[1][1] - radius};
        obs_rectangular.push_back(r);
        r.minor = {world.right_fisical_left_goal[0][0] - radius, world.right_fisical_left_goal[0][1] + radius};
        r.major = {world.right_fisical_left_goal[1][0] - radius, world.right_fisical_left_goal[1][1] + radius};
        obs_rectangular.push_back(r);


        r.minor = {world.back_fisical_right_goal[0][0] + radius, world.back_fisical_right_goal[0][1] - radius};
        r.major = {world.back_fisical_right_goal[1][0] + radius, world.back_fisical_right_goal[1][1] + radius};
        obs_rectangular.push_back(r);
        r.minor = {world.left_fisical_right_goal[0][0] + radius, world.left_fisical_right_goal[0][1] - radius};
        r.major = {world.left_fisical_right_goal[1][0] + radius, world.left_fisical_right_goal[1][1] - radius};
        obs_rectangular.push_back(r);
        r.minor = {world.right_fisical_right_goal[0][0] + radius, world.right_fisical_right_goal[0][1] + radius};
        r.major = {world.right_fisical_right_goal[1][0] + radius, world.right_fisical_right_goal[1][1] + radius};
        obs_rectangular.push_back(r);
    }


    auto trajectory = pf.path_find(double_start, double_goal, obs_circular, obs_rectangular);
    return trajectory;
}



std::vector<double> Robot_controller::motion_planner(std::vector<std::vector<double>> trajectory) {
    for (auto i : trajectory) {
        break;
        std::cout << i[0] << ", " << i[1] << std::endl;
    }
    std::vector<double> delta = {trajectory[1][0] - pos[0], trajectory[1][1] - pos[1]};
    double dist = norm(delta) / 1000.0; // metros
    std::vector<double> direction = normalize(1, delta);

    double curve_safe_speed = vxy_min;
    double v_target_magnitude = vxy_max;
    if (trajectory.size() > 2) {
        std::vector<double> p0 = {pos[0], pos[1]};
        std::vector<double> p1 = {trajectory[1][0], trajectory[1][1]};
        std::vector<double> p2 = {trajectory[2][0], trajectory[2][1]};

        std::vector<double> v1 = {p1[0] - p0[0], p1[1] - p0[1]};
        std::vector<double> v2 = {p2[0] - p1[0], p2[1] - p1[1]};

        double dot_product = v1[0] * v2[0] + v1[1] * v2[1];
        double norm_v1 = sqrt(v1[0]*v1[0] + v1[1]*v1[1]);
        double norm_v2 = sqrt(v2[0]*v2[0] + v2[1]*v2[1]);
        double angle_between = acos(std::clamp(dot_product / (norm_v1 * norm_v2 + 1e-6), -1.0, 1.0));

        double chord_length = sqrt(pow(p2[0] - p0[0], 2) + pow(p2[1] - p0[1], 2)); // mm
        chord_length /= 1000.0; // Para metros

        double radius = chord_length / (2.0 * sin(std::max(angle_between/2, 1e-3)));
        radius = std::max(radius, 0.05);
        curve_safe_speed = sqrt(a_xy_max * radius)/4;
        curve_safe_speed = std::clamp(curve_safe_speed, vxy_min, vxy_max);

        if (angle_between > 70.0 * M_PI / 180.0) {
            curve_safe_speed = vxy_min;
        }

    }

    double current_speed = sqrt(vel[0]*vel[0] + vel[1]*vel[1]);

    double brake_distance = (vxy_max*vxy_max - curve_safe_speed * curve_safe_speed) / (2.0 * a_xy_brake);
    brake_distance = std::max(brake_distance, 0.0);
    if (dist <= brake_distance) {
        v_target_magnitude = curve_safe_speed;
    } else {
        v_target_magnitude = vxy_max;
    }

    v_target_magnitude = std::clamp(v_target_magnitude, -vxy_max, vxy_max);
    std::vector<double> v_target = {v_target_magnitude * direction[0], v_target_magnitude * direction[1]};
    std::vector<double> error = {v_target[0] - last_target_vel[0], v_target[1] - last_target_vel[1]};
    std::vector<double> acceleration = {0, 0};

    if (fabs(v_target[0]) > fabs(last_target_vel[0])) {
        acceleration[0] = std::clamp(error[0] / delta_time, -a_xy_max, a_xy_max);
    } else {
        acceleration[0] = std::clamp(error[0] / delta_time, -a_xy_brake, a_xy_brake);
    }

    if (fabs(v_target[1]) > fabs(last_target_vel[1])) {
        acceleration[1] = std::clamp(error[1] / delta_time, -a_xy_max, a_xy_max);
    } else {
        acceleration[1] = std::clamp(error[1] / delta_time, -a_xy_brake, a_xy_brake);
    }


    std::vector<double> vel_cmd = {last_target_vel[0] + acceleration[0]*delta_time, last_target_vel[1] + acceleration[1]*delta_time};
    if (norm(vel_cmd) > vxy_max) {
        vel_cmd = normalize(vxy_max, vel_cmd);
    }

    if (isnan(vel_cmd[0])) vel_cmd[0] = 0;
    if (isnan(vel_cmd[1])) vel_cmd[1] = 0;

    last_target_vel[0] = vel_cmd[0];
    last_target_vel[1] = vel_cmd[1];
    return vel_cmd;
}



std::vector<double> Robot_controller::motion_control(std::vector<double> v_vet) {
    const double ang = -yaw;
    double x = v_vet[0];
    double y = v_vet[1];
    v_vet = {x * cos(ang) - y * sin(ang), x * sin(ang) + y * cos(ang)};
    return v_vet;
}

void Robot_controller::move_to(double goal[2], bool avoid_ball = true) {

    if (sqrt(pow(goal[0] - pos[0], 2) + pow(goal[1] - pos[1], 2)) < static_position_tolarance) {
        target_vel[0] = 0;
        target_vel[1] = 0;
        target_vyaw = 0;
        positioned = true;
        return;
    }
    positioned = false;

    auto trajectory = find_trajectory(pos, goal, avoid_ball, false);
    auto v_vet = motion_planner(trajectory);

    v_vet = motion_control(v_vet);

    target_vel[0] = v_vet[0];
    target_vel[1] = v_vet[1];
}

double Robot_controller::find_angle_error(double goal[2]) {
    double theta_final = atan2(goal[1] - pos[1], goal[0] - pos[0]);
    double orientation = yaw;
    double delta = theta_final - orientation;
    if (delta > M_PI) delta -= 2 * M_PI;
    if (delta < -M_PI) delta += 2 * M_PI;
    return delta;
}

double Robot_controller::turn_control(double delta) {
    double P = KP_ang * delta;
    I_ang = I_ang + delta*delta_time*KI_ang;
    double D = (delta-last_delta)*KD_ang;
    //std::cout << P << ", " << I_ang << ", " << D << std::endl;
    double PID_vyaw = P + I_ang + D;

    last_delta = delta;

    if (fabs(PID_vyaw) > vyaw_max) {
        PID_vyaw = vyaw_max*PID_vyaw/fabs(PID_vyaw);
    };
    if (fabs(PID_vyaw) < vyaw_min && fabs(PID_vyaw) != 0) {
        PID_vyaw = vyaw_min*PID_vyaw/fabs(PID_vyaw);
    }
    return PID_vyaw;
}


void Robot_controller::turn_to(double goal[2]) {
    double delta = find_angle_error(goal);
    if (fabs(delta) < static_angle_tolarance) {
        target_vyaw = 0;
        oriented = true;
        return;
    }
    oriented = false;
    target_vyaw = turn_control(delta);
}

void Robot_controller::kick() {
    if (world.ball_speed_module >= vxy_min) {
        kicker_x = 0;
        state += 1;
    }

    std::vector<double> v_vet = {world.ball_pos[0] - pos[0], world.ball_pos[1] - pos[1]};
    v_vet = normalize(vxy_min,v_vet);
    target_vel[0] = v_vet[0]*cos(-yaw) - v_vet[1]*sin(-yaw);
    target_vel[1] = v_vet[0]*sin(-yaw) + v_vet[1]*cos(-yaw);
    kicker_x = 1000;

}


void Robot_controller::follow_trajectory(std::vector<std::vector<double>>& trajectory) {

    int i = 0;
    while (size(trajectory) > 0) {
        double distance = sqrt(pow(trajectory[0][0] - pos[0], 2) + pow(trajectory[0][1] - pos[1], 2));
        if (size(trajectory) == 1 && distance < static_position_tolarance) {
            trajectory.erase(trajectory.begin());
            break;
        }

        if (distance < dynamic_position_tolarance) {
            trajectory.erase(trajectory.begin());
            continue;
        }
        break;
    }
    if (size(trajectory) == 0) {
        target_vel[0] = 0;
        target_vel[1] = 0;
        positioned = true;
        return;
    }

    double next_point[2] = {trajectory[0][0], trajectory[0][1]};
    auto mid_trajectory = find_trajectory(pos, next_point, true);
    mid_trajectory.insert(mid_trajectory.end(), trajectory.begin() + 1, trajectory.end());
    auto v_vet = motion_planner(mid_trajectory);
    v_vet = motion_control(v_vet);

    target_vel[0] = v_vet[0];
    target_vel[1] = v_vet[1];
    positioned = false;

}








void Robot_controller::role_table() {
    //TODO roles

    if (team->roles[id] == -1) {
        target_vel[0] = 0;
        target_vel[1] = 0;
        target_vyaw = 0;
    }

    if (team->roles[id] == 0) {
        goal_keeper_role();
    }
    else if (team->roles[id] == 1) {
        stricker_role();
    } else if (team->roles[id] == 2) {
        std::cout << id << std::endl;
        attack_support_role();
    }


    if (team->roles[id] == 990) {
        if (size(current_trajectory) == 0) {
            int resolution = 120;
            for (int i = 0; i < resolution; i++) {
                current_trajectory.push_back({1000*sin(2*i*M_PI/resolution + id*2*M_PI/size(world.allies)), 1000*cos(2*i*M_PI/resolution + id*2*M_PI/size(world.allies))});
            }
        }
        follow_trajectory(current_trajectory);
    }
    if (team->roles[id] == 991) {
        if (size(current_trajectory) == 0) {
            current_trajectory = {{4000, 4000}, {4000, -4000}, {4000, 4000}, {-4000, 4000}};
        }
        follow_trajectory(current_trajectory);
    }
}



void Robot_controller::stricker_role() {
    //TODO melhorar stricker_role para chutar para o passe
    //std::cout << state << std::endl;

    //double goal[2] = {(world.their_goal[0][1] + world.their_goal[0][0])/2, (world.their_goal[1][1] + world.their_goal[1][0])/2};
    double goal[2] = {(world.our_goal[0][1] + world.our_goal[0][0])/2, (world.our_goal[1][1] + world.our_goal[1][0])/2};
    /*if (kick_distance < sqrt(pow(world.ball_pos[0] - world.their_goal[0][0], 2) + pow(world.ball_pos[1], 2))) {
        world.generate_support_areas(goal, kick_distance);
        goal[0] = world.support_areas[0][0];
        goal[1] = world.support_areas[0][1];
    }*/


    auto traj = find_trajectory(world.ball_pos, goal, false, true);
    std::vector<double> kick_pos = world.kicking_position(traj[0], traj[1], ball_avoidance_radius + radius);
    double next_point[2] = {traj[1][0], traj[1][1]};

    if (state == 0) {
        double db_kick_pos[2] = {kick_pos[0], kick_pos[1]};;
        move_to(db_kick_pos);
        turn_to(next_point);
        if (positioned and oriented) {
            state = 1;
            return;
        }

    } else if (state == 1) {
        kick();
        turn_to(next_point);
    } else if (state == 2) {
        state = 0;
    }
}

void Robot_controller::goal_keeper_role() {
    //TODO melhorar goal_keeper_role
    if (!world.ball_on_our_area() || world.ball_speed_module > 0.2) {
        state = 0;
        //posicionando para defesa
        std::vector<int> enemies_ids = {};
        std::vector<double> enemies_distances = {};

        for (robot i : world.enemies) {
            if (i.detected) {
                enemies_ids.push_back(i.id);
                enemies_distances.push_back(sqrt(pow(i.pos[0] - world.ball_pos[0], 2) + pow(i.pos[1] - world.ball_pos[1], 2)));
            }
        }
        int closest_idx = 0;
        for (int idx = 0; idx < enemies_ids.size(); idx++) {
            if (enemies_distances[idx] < enemies_distances[closest_idx]) {
                closest_idx = idx;
            }
        }

        double a = 1000;
        if (size(enemies_ids) != 0) {
            int closest_id = enemies_ids[closest_idx];
            if (sqrt(pow(world.ball_speed[0], 2) + pow(world.ball_speed[1], 2)) > vxy_min) {
                if (world.ball_speed[0] != 0) {
                    a = (world.ball_speed[1]) / (world.ball_speed[0]);
                }
            }
            else if (world.ball_pos[0] - world.enemies[closest_id].pos[0] != 0) {    //para bola parada
                a = (world.ball_pos[1] - world.enemies[closest_id].pos[1]) / (world.ball_pos[0] - world.enemies[closest_id].pos[0]);
            }
        }

        double y_meet = a*(world.our_goal[0][0]) + world.ball_pos[1] -a*world.ball_pos[0];
        double y_max = world.our_goal[1][0] - radius;
        double y_min = world.our_goal[1][1] + radius;

        y_meet = std::clamp(y_meet, y_min, y_max);

        double x_meet = world.our_goal[0][0] - radius*world.our_goal[0][0]/fabs(world.our_goal[0][0]);


        double meet[2] = {x_meet, y_meet};

        if (!world.ball_on_our_side()) {
            meet[1] = 0;
        }


        move_to(meet, false);
        kicker_x = 0;
        turn_to(world.ball_pos);
    }
    else {
        double goal[2] = {(world.their_goal[0][1] + world.their_goal[0][0])/2, (world.their_goal[1][1] + world.their_goal[1][0])/2};
        auto traj = find_trajectory(world.ball_pos, goal, false, true);
        std::vector<double> kick_pos = world.kicking_position(traj[0], traj[1], ball_avoidance_radius + radius);
        double next_point[2] = {traj[1][0], traj[1][1]};

        if (state == 0) {
            double db_kick_pos[2] = {kick_pos[0], kick_pos[1]};;
            move_to(db_kick_pos);
            turn_to(next_point);
            if (positioned and oriented) {
                state = 1;
                return;
            }

        } else if (state == 1) {
            kick();
        } else if (state == 2) {
            state = 0;
        }
    }
}

void Robot_controller::attack_support_role() {
    //TODO continuar
    //TODO melhorar esquema, importar informacoes do team
    if (world.ball_speed_module < 0.1 || size(world.support_areas) == 0) {
        double goal[2] = {world.their_goal[0][0], 0};
        world.generate_support_areas(goal, kick_distance);
    }
    int priority = 0;
    for (int i = 0 ; i < id ; i++ ) {
        if (team->roles[i] == 2) priority++;
    }
    auto support_areas = world.support_areas;


    if (state == 0) {
        double goal[2] = {support_areas[priority][0], support_areas[priority][1]};
        move_to(goal);
        turn_to(world.ball_pos);
    }
}




void Robot_controller::check_connection() {
    if (!detected) {
        offline_counter++;
    } else {
        offline_counter = 0;
    }

    if (offline_counter >= max_offline_counter) {
        stop();
    }
}


void Robot_controller::receive_vision() {
    std::unordered_set<int> allies_detected = {};
    std::unordered_set<int> enemies_detected = {};

    for (auto blue_robot : han.new_vision.robots_blue) {
        if (team->color == 0) {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(world.allies)) {
                for (int i = size(world.allies); i <= rb_id; i++) {
                    world.allies.push_back(robot(i));
                }
            }

            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.allies[rb_id].stored_speed_x.push_back((blue_robot.position_x - world.allies[rb_id].pos[0])/(delta_time*1000));
                world.allies[rb_id].stored_speed_y.push_back((blue_robot.position_y - world.allies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.allies[rb_id].stored_speed_x) > 10) {
                    world.allies[rb_id].stored_speed_x.pop_front();
                    world.allies[rb_id].stored_speed_y.pop_front();
                }
                world.allies[rb_id].vel[0] = std::accumulate(world.allies[rb_id].stored_speed_x.begin(), world.allies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.allies[rb_id].vel[1] = std::accumulate(world.allies[rb_id].stored_speed_y.begin(), world.allies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.allies[rb_id].yaw = new_yaw;
            world.allies[rb_id].pos[0] = blue_robot.position_x;
            world.allies[rb_id].pos[1] = blue_robot.position_y;
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(world.enemies)) {
                for (int i = size(world.enemies); i <= rb_id; i++) {
                    world.enemies.push_back(robot(i));
                }
            }

            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.enemies[rb_id].stored_speed_x.push_back((blue_robot.position_x - world.enemies[rb_id].pos[0])/(delta_time*1000));
                world.enemies[rb_id].stored_speed_y.push_back((blue_robot.position_y - world.enemies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.enemies[rb_id].stored_speed_x) > 10) {
                    world.enemies[rb_id].stored_speed_x.pop_front();
                    world.enemies[rb_id].stored_speed_y.pop_front();
                }
                world.enemies[rb_id].vel[0] = std::accumulate(world.enemies[rb_id].stored_speed_x.begin(), world.enemies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.enemies[rb_id].vel[1] = std::accumulate(world.enemies[rb_id].stored_speed_y.begin(), world.enemies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.enemies[rb_id].yaw = new_yaw;
            world.enemies[rb_id].pos[0] = blue_robot.position_x;
            world.enemies[rb_id].pos[1] = blue_robot.position_y;
            enemies_detected.insert(rb_id);
        }
    }


    for (auto yellow_robot : han.new_vision.robots_yellow) {
        if (team->color == 1) {
            int rb_id = yellow_robot.robot_id;
            if (rb_id >= size(world.allies)) {
                for (int i = size(world.allies); i <= rb_id; i++) {
                    world.allies.push_back(robot(i));
                }
            }
            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.allies[rb_id].stored_speed_x.push_back((yellow_robot.position_x - world.allies[rb_id].pos[0])/(delta_time*1000));
                world.allies[rb_id].stored_speed_y.push_back((yellow_robot.position_y - world.allies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.allies[rb_id].stored_speed_x) > 10) {
                    world.allies[rb_id].stored_speed_x.pop_front();
                    world.allies[rb_id].stored_speed_y.pop_front();
                }
                world.allies[rb_id].vel[0] = std::accumulate(world.allies[rb_id].stored_speed_x.begin(), world.allies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.allies[rb_id].vel[1] = std::accumulate(world.allies[rb_id].stored_speed_y.begin(), world.allies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.allies[rb_id].yaw = new_yaw;
            world.allies[rb_id].pos[0] = yellow_robot.position_x;
            world.allies[rb_id].pos[1] = yellow_robot.position_y;
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = yellow_robot.robot_id;
            if (rb_id >= size(world.enemies)) {
                for (int i = size(world.enemies); i <= rb_id; i++) {
                    world.enemies.push_back(robot(i));
                }
            }
            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (delta_time > 0) {
                world.enemies[rb_id].stored_speed_x.push_back((yellow_robot.position_x - world.enemies[rb_id].pos[0])/(delta_time*1000));
                world.enemies[rb_id].stored_speed_y.push_back((yellow_robot.position_y - world.enemies[rb_id].pos[1])/(delta_time*1000));
                if (size(world.enemies[rb_id].stored_speed_x) > 10) {
                    world.enemies[rb_id].stored_speed_x.pop_front();
                    world.enemies[rb_id].stored_speed_y.pop_front();
                }
                world.enemies[rb_id].vel[0] = std::accumulate(world.enemies[rb_id].stored_speed_x.begin(), world.enemies[rb_id].stored_speed_x.end(), 0.0)/10;
                world.enemies[rb_id].vel[1] = std::accumulate(world.enemies[rb_id].stored_speed_y.begin(), world.enemies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            world.enemies[rb_id].yaw = new_yaw;
            world.enemies[rb_id].pos[0] = yellow_robot.position_x;
            world.enemies[rb_id].pos[1] = yellow_robot.position_y;
            enemies_detected.insert(rb_id);
        }
    }

    for (int i = 0; i < size(world.allies); i++) {
        if (allies_detected.find(i) != allies_detected.end()) world.allies[i].detected = true;
        else world.allies[i].detected = false;
        }

    for (int i = 0; i < size(world.enemies); i++) {
        if (enemies_detected.find(i) != enemies_detected.end()) world.enemies[i].detected = true;
        else world.enemies[i].detected = false;
    }

    if (delta_time != 0) {
        world.ball_speed[0] = (han.new_vision.balls.position_x - world.ball_pos[0])/(delta_time*1000);
        world.ball_speed[1] = (han.new_vision.balls.position_y - world.ball_pos[1])/(delta_time*1000);
    }

    world.ball_pos[0] = han.new_vision.balls.position_x;
    world.ball_pos[1] = han.new_vision.balls.position_y;
    world.ball_speed_module = sqrt(world.ball_speed[0]*world.ball_speed[0] + world.ball_speed[1]*world.ball_speed[1]);

    last_time_stamp = han.new_vision.timestamp;

    pos[0] = world.allies[id].pos[0];
    pos[1] = world.allies[id].pos[1];
    yaw = world.allies[id].yaw;
    vel[0] = world.allies[id].vel[0];
    vel[1] = world.allies[id].vel[1];
    vyaw = world.allies[id].vyaw;
    detected = world.allies[id].detected;
}




void Robot_controller::publish() {
    /*
    std::cout << id << std::endl;
    std::cout << target_vel[1] << std::endl;
    std::cout << target_vel[0] << std::endl;
    std::cout << target_vyaw << std::endl;
    */



    han.new_ia.robots[id].id = id;
    han.new_ia.robots[id].vel_normal = target_vel[1];
    han.new_ia.robots[id].vel_tang = target_vel[0];
    han.new_ia.robots[id].vel_ang = static_cast<float>(target_vyaw);
    if (kicker_x != 0) {
        han.new_ia.robots[id].kick_speed_x = kicker_x_max;
    } else {
        han.new_ia.robots[id].kick_speed_x = 0;
    }
    han.lc->publish("IA", &han.new_ia);
    han.lc->publish("tartarus", &han.new_tartarus);

}
