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


void Robot_controller::start(team_info* team) {
    han.new_ia.robots[id].kick_speed_x = 3;
    this->team = team;
    terminate = false;
    std::cout << "iniciado " << id << std::endl;
    team->roles[id] = -1;
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
    double yaw_prev = yaw;
    while (not terminate) {
        if (last_time_stamp == han.new_ia.timestamp) {
            continue;
        }
        auto t1 = std::chrono::steady_clock::now();
        receive_vision();
        //std::cout << id << ", " << team->roles[id] << std::endl;
        check_connection();
        turn_to(world.ball_pos);
        //role_table();
        publish();
        cycles++;
        std::chrono::duration<double> delta = t1 - t0;
        t0 = std::chrono::steady_clock::now();
        delta_time = delta.count();
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
}




std::vector<std::vector<double>> Robot_controller::find_trajectory(double start[2], double goal[2], bool avoid_ball = true) {
    C_trajectory pf(false, false, 0, 1000, 50, 0);
    std::vector<double> double_start = {start[0], start[1]};
    std::vector<double> double_goal = {goal[0], goal[1]};

    std::vector<circle> obs_circular = {};
    std::vector<rectangle> obs_rectangular = {};

    //rectangle r = field.their_defense_area;
    //obs_rectangular.push_back(r);

    if (avoid_ball) {
        circle c({world.ball_pos[0], world.ball_pos[1]}, radius);
        obs_circular.push_back(c);
    }

    for (int i = 0; i < size(world.allies) ; i++) {
        if (!world.allies[i].detected) {
            continue;
        }
        circle c({world.allies[i].pos[0], world.allies[i].pos[1]}, radius);
        obs_circular.push_back(c);
    }
    for (int i = 0; i < size(world.enemies) ; i++) {
        if (!world.enemies[i].detected) {
            continue;
        }
        circle c({world.enemies[i].pos[0], world.enemies[i].pos[1]}, radius);
        obs_circular.push_back(c);
    }

    auto trajectory = pf.path_find(double_start, double_goal, obs_circular, obs_rectangular);
    return trajectory;
}



std::vector<double> Robot_controller::motion_planner(std::vector<std::vector<double>> trajectory) {
    std::vector<double> vect_pos = {pos[0], pos[1]};
    std::vector<double> v_vet = subtract(trajectory[1], vect_pos);
    return v_vet;
}

std::vector<double> Robot_controller::motion_control(std::vector<double> v_vet) {
    const double ang = -yaw;
    double x = v_vet[0];
    double y = v_vet[1];
    v_vet = {x * cos(ang) - y * sin(ang), x * sin(ang) + y * cos(ang)};
    double db_vmax = static_cast<double>(vxy_max);
    v_vet = normalize(db_vmax, v_vet);
    return v_vet;
}

void Robot_controller::move_to(double goal[2], bool avoid_ball = true) {

    if (sqrt(pow(goal[0] - pos[0], 2) + pow(goal[1] - pos[1], 2)) < static_position_tolarance) {
        target_vel[0] = 0;
        target_vel[1] = 0;
        target_vyaw = 0;
        return;
    }
    auto trajectory = find_trajectory(pos, goal, avoid_ball);
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
        return;
    }
    target_vyaw = turn_control(delta);
}

void Robot_controller::kick() {
    if (sqrt(world.ball_speed[0]*world.ball_speed[0] + world.ball_speed[1]*world.ball_speed[1]) >= vxy_min) {
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
        return;
    }

    double next_point[2] = {trajectory[0][0], trajectory[0][1]};
    move_to(next_point);

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
            current_trajectory = {{2000, 2000}, {2000, -2000}, {-2000, -2000}, {-2000, 2000}};
        }
        follow_trajectory(current_trajectory);
    }
}



void Robot_controller::stricker_role() {
    //TODO melhorar stricker_role
    //std::cout << state << std::endl;
    if (state == 0) {

        //double goal[2] = {(field.their_goal[0][1] + field.their_goal[0][0])/2, (field.their_goal[1][1] + field.their_goal[1][0])/2};

        //TODO REMOVER
        double goal[2] = {(world.our_goal[0][1] + world.our_goal[0][0])/2, (world.our_goal[1][1] + world.our_goal[1][0])/2};

        auto traj = find_trajectory(world.ball_pos, goal, false);

        std::vector<double> kick_pos = world.kicking_position(traj[0], traj[1], radius);
        double db_kick_pos[2] = {kick_pos[0], kick_pos[1]};;
        move_to(db_kick_pos);
        if (sqrt(pow(pos[0] - db_kick_pos[0], 2) + pow(pos[1] - db_kick_pos[1], 2)) < radius/2) {
            target_vel[0] = 0;
            target_vel[1] = 0;
            state = 1;
            return;
        }
        turn_to(world.ball_pos);

    } else if (state == 1) {
        target_vel[0] = 0;
        target_vel[1] = 0;
        turn_to(world.ball_pos);
        if (sqrt(pow(pos[0] - world.ball_pos[0], 2) + pow(pos[1] - world.ball_pos[1], 2)) > 2*radius) {
            state = 0;
            return;
        }
        if (target_vyaw == 0) {
            state += 1;
        }
    } else if (state == 2) {
        kick();
    } else if (state == 3) {
        state = 0;
    }
}

void Robot_controller::goal_keeper_role() {
    //TODO melhorar goal_keeper_role
    if (state == 0) {
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
        int closest_id = enemies_ids[closest_idx];

        double a = 10000;
        if (sqrt(pow(world.ball_speed[0], 2) + pow(world.ball_speed[1], 2)) > vxy_min) {
            if (world.ball_speed[0] != 0) {
                a = (world.ball_speed[1]) / (world.ball_speed[0]);
            }
        }
        else if (world.ball_pos[0] - world.enemies[closest_id].pos[0] != 0) {    //para bola parada
            a = (world.ball_pos[1] - world.enemies[closest_id].pos[1]) / (world.ball_pos[0] - world.enemies[closest_id].pos[0]);
        }



        double y_meet = a*(world.our_goal[0][0]) + world.ball_pos[1] -a*world.ball_pos[0];
        double y_max = world.our_goal[1][0] - radius;
        double y_min = world.our_goal[1][1] + radius;

        if (y_meet > y_max) {
            y_meet = y_max;
        }
        if (y_meet < y_min) {
            y_meet = y_min;
        }

        double meet[2] = {world.our_goal[0][0], y_meet};


        move_to(meet);
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
    detected = false;
    for (int i = 0; i < size(world.allies); i++) {
        world.allies[i].detected = false;
    }
    for (int i = 0; i < size(world.enemies); i++) {
        world.enemies[i].detected = false;
    }
    for (auto blue_robot : han.new_vision.robots_blue) {
        if (team->color == 0) {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(world.allies)) {
                for (int i = size(world.allies); i <= rb_id; i++) {
                    world.allies.push_back(robot(i));
                }
            }
            if (rb_id == id) {
                double new_yaw = blue_robot.orientation;
                if (new_yaw < 0) new_yaw += 2*M_PI;
                if (delta_time > 0) {
                    vel[0] = (pos[0] - blue_robot.position_x)/delta_time;
                    vel[1] = (pos[1] - blue_robot.position_y)/delta_time;
                    vyaw = (yaw - new_yaw)/delta_time;
                }
                yaw = new_yaw;
                pos[0] = blue_robot.position_x;
                pos[1] = blue_robot.position_y;
                detected = true;
                continue;
            }
            world.allies[rb_id].vel[0] = (blue_robot.position_x - world.allies[rb_id].pos[0])/delta_time;
            world.allies[rb_id].vel[1] = (blue_robot.position_y - world.allies[rb_id].pos[1])/delta_time;
            world.allies[rb_id].pos[0] = blue_robot.position_x;
            world.allies[rb_id].pos[1] = blue_robot.position_y;
            world.allies[rb_id].yaw = blue_robot.orientation;
            if (world.allies[rb_id].yaw < 0) world.allies[rb_id].yaw += 2*M_PI;
            world.allies[rb_id].detected = true;
        }
        else {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(world.enemies)) {
                for (int i = size(world.enemies); i <= rb_id; i++) {
                    world.enemies.emplace_back(i);
                }
            }
            world.enemies[rb_id].vel[0] = (blue_robot.position_x - world.enemies[rb_id].pos[0])/delta_time;
            world.enemies[rb_id].vel[1] = (blue_robot.position_y - world.enemies[rb_id].pos[1])/delta_time;
            world.enemies[rb_id].pos[0] = blue_robot.position_x;
            world.enemies[rb_id].pos[1] = blue_robot.position_y;
            world.enemies[rb_id].yaw = blue_robot.orientation;
            if (world.enemies[rb_id].yaw < 0) world.enemies[rb_id].yaw += 2*M_PI;
            world.enemies[rb_id].detected = true;
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
            if (rb_id == id) {
                double new_yaw = yellow_robot.orientation;
                if (new_yaw < 0) new_yaw += 2*M_PI;
                if (delta_time > 0) {
                    vel[0] = (pos[0] - yellow_robot.position_x)/delta_time;
                    vel[1] = (pos[1] - yellow_robot.position_y)/delta_time;
                    vyaw = (yaw - new_yaw)/delta_time;
                }
                yaw = new_yaw;
                pos[0] = yellow_robot.position_x;
                pos[1] = yellow_robot.position_y;
                detected = true;
                continue;
            }
            world.allies[rb_id].vel[0] = (yellow_robot.position_x - world.allies[rb_id].pos[0])/delta_time;
            world.allies[rb_id].vel[1] = (yellow_robot.position_y - world.allies[rb_id].pos[1])/delta_time;
            world.allies[rb_id].pos[0] = yellow_robot.position_x;
            world.allies[rb_id].pos[1] = yellow_robot.position_y;
            world.allies[rb_id].yaw = yellow_robot.orientation;
            if (world.allies[rb_id].yaw < 0) world.allies[rb_id].yaw += 2*M_PI;
            world.allies[rb_id].detected = true;
        }
        else {
            int rb_id = yellow_robot.robot_id;
            if (rb_id >= size(world.enemies)) {
                for (int i = size(world.enemies); i <= rb_id; i++) {
                    world.enemies.emplace_back(i);
                }
            }
            world.enemies[rb_id].vel[0] = (yellow_robot.position_x - world.enemies[rb_id].pos[0])/delta_time;
            world.enemies[rb_id].vel[1] = (yellow_robot.position_y - world.enemies[rb_id].pos[1])/delta_time;
            world.enemies[rb_id].pos[0] = yellow_robot.position_x;
            world.enemies[rb_id].pos[1] = yellow_robot.position_y;
            world.enemies[rb_id].yaw = yellow_robot.orientation;
            if (world.enemies[rb_id].yaw < 0) world.enemies[rb_id].yaw += 2*M_PI;
            world.enemies[rb_id].detected = true;
        }
    }
    if (delta_time != 0) {
        world.ball_speed[0] = (han.new_vision.balls.position_x - world.ball_pos[0])/delta_time;
        world.ball_speed[1] = (han.new_vision.balls.position_y - world.ball_pos[1])/delta_time;
    }
    world.ball_pos[0] = han.new_vision.balls.position_x;
    world.ball_pos[1] = han.new_vision.balls.position_y;

    last_time_stamp = han.new_vision.timestamp;
}





void Robot_controller::publish() {
    han.new_ia.robots[id].id = id;
    han.new_ia.robots[id].vel_normal = target_vel[1];
    han.new_ia.robots[id].vel_tang = target_vel[0];
    han.new_ia.robots[id].vel_ang = static_cast<float>(target_vyaw);
    if (kicker_x) {
        han.new_ia.robots[id].kick_speed_x = kicker_x_max;
    } else {
        han.new_ia.robots[id].kick_speed_x = 0;
    }
    han.lc->publish("IA", &han.new_ia);
    han.lc->publish("tartarus", &han.new_tartarus);

}
