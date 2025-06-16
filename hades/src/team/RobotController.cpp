//
// Created by caiom on 4/4/2025.
//

#include "RobotController.h"
#include <iostream>
#include <thread>
#include "../c_trajectory/C_trajectory.h"
#include <math.h>
#include "../include/handlers.hpp"
#include <chrono>
#include <numeric>
#include <set>
#include <unordered_set>


void RobotController::start(TeamInfo* team) {
    han.new_ia.robots[mId].kick_speed_x = 3;
    this->mTeam = team;
    mTeam->num_of_active_robots++;
    mTerminate = false;
    mOffline_counter = 0;
    loadCalibration();
    std::cout << "iniciado " << mId << std::endl;
    std::thread t(&RobotController::loop, this);
    t.detach();
}

void RobotController::stop() {
    mTeam->active_robots[mId] = false;
    mTeam->roles[mId] = TeamInfo::unknown;
    mTerminate = true;
    mTeam->num_of_active_robots--;
}

void RobotController::loop() {
    auto t0 = std::chrono::steady_clock::now();

    while (not mTerminate) {
        if (mLast_time_stamp == han.new_ia.timestamp) {
            continue;
        }
        auto t1 = std::chrono::steady_clock::now();

        //if (mId == 1) std::cout << mWorld.getIdOfTheBallInterceptor() << std::endl;
        if (mId == 1) std::cout << mWorld.ball_speed_module << std::endl;
        receive_vision();
        check_connection();
        select_behavior();
        publish();

        std::chrono::duration<double> delta = t1 - t0;
        t0 = std::chrono::steady_clock::now();
        mDelta_time = delta.count();
    }
    std::cout << "Encerrado " << mId << std::endl;
}




std::vector<std::vector<double>> RobotController::find_trajectory(double start[2], double goal[2], bool avoid_ball = true) {
    //goal[0] = 7261.12;
    //goal[1] = -2280.79;
    C_trajectory pf(false, false, 0, 100, 50, 0, mWorld.boundariesMinor, mWorld.boundariesMajor);
    std::vector<double> double_start = {start[0], start[1]};
    std::vector<double> double_goal = {goal[0], goal[1]};

    std::vector<Circle> obs_circular = {};
    std::vector<Rectangle> obs_rectangular = {};
    Rectangle r({0, 0}, {0, 0});
    //rectangle r = field.their_defense_area;
    //obs_rectangular.push_back(r);

    //add static ball to obstacles according to avoidance radius
    if (avoid_ball) {
        Circle c({mWorld.ball_pos[0], mWorld.ball_pos[1]}, mBall_avoidance_radius + mRadius);
        obs_circular.push_back(c);
    }

    //add static allies to obstacles
    for (int i = 0; i < size(mWorld.allies) ; i++) {
        if (!mWorld.allies[i].detected || i == mId) {
            continue;
        }
        Circle c({mWorld.allies[i].pos[0], mWorld.allies[i].pos[1]}, mRadius);
        obs_circular.push_back(c);
    }

    //add static enemies to obstacles
    for (int i = 0; i < size(mWorld.enemies) ; i++) {
        if (!mWorld.enemies[i].detected) {
            continue;
        }
        Circle c({mWorld.enemies[i].pos[0], mWorld.enemies[i].pos[1]}, mRadius);
        obs_circular.push_back(c);
    }

    if (mTeam->roles[mId] != TeamInfo::goal_keeper) {
        Rectangle r({mWorld.our_defese_area[0][0] - mRadius, mWorld.our_defese_area[0][1] - mRadius}, {mWorld.our_defese_area[1][0] + mRadius, mWorld.our_defese_area[1][1] + mRadius});
        obs_rectangular.push_back(r);
    }


    r.minor = {mWorld.their_defese_area[0][0] - mRadius, mWorld.their_defese_area[0][1] - mRadius};
    r.major = {mWorld.their_defese_area[1][0] + mRadius, mWorld.their_defese_area[1][1] + mRadius};
    obs_rectangular.push_back(r);


    //goal fisical barrier

    r.minor = {mWorld.back_fisical_left_goal[0][0] - mRadius, mWorld.back_fisical_left_goal[0][1] - mRadius};
    r.major = {mWorld.back_fisical_left_goal[1][0] - mRadius, mWorld.back_fisical_left_goal[1][1] + mRadius};
    obs_rectangular.push_back(r);
    r.minor = {mWorld.left_fisical_left_goal[0][0] - mRadius, mWorld.left_fisical_left_goal[0][1] - mRadius};
    r.major = {mWorld.left_fisical_left_goal[1][0] - mRadius, mWorld.left_fisical_left_goal[1][1] - mRadius};
    obs_rectangular.push_back(r);
    r.minor = {mWorld.right_fisical_left_goal[0][0] - mRadius, mWorld.right_fisical_left_goal[0][1] + mRadius};
    r.major = {mWorld.right_fisical_left_goal[1][0] - mRadius, mWorld.right_fisical_left_goal[1][1] + mRadius};
    obs_rectangular.push_back(r);


    r.minor = {mWorld.back_fisical_right_goal[0][0] + mRadius, mWorld.back_fisical_right_goal[0][1] - mRadius};
    r.major = {mWorld.back_fisical_right_goal[1][0] + mRadius, mWorld.back_fisical_right_goal[1][1] + mRadius};
    obs_rectangular.push_back(r);
    r.minor = {mWorld.left_fisical_right_goal[0][0] + mRadius, mWorld.left_fisical_right_goal[0][1] - mRadius};
    r.major = {mWorld.left_fisical_right_goal[1][0] + mRadius, mWorld.left_fisical_right_goal[1][1] - mRadius};
    obs_rectangular.push_back(r);
    r.minor = {mWorld.right_fisical_right_goal[0][0] + mRadius, mWorld.right_fisical_right_goal[0][1] + mRadius};
    r.major = {mWorld.right_fisical_right_goal[1][0] + mRadius, mWorld.right_fisical_right_goal[1][1] + mRadius};
    obs_rectangular.push_back(r);

    r.minor = {mWorld.outside_field_x_minus[0][0], mWorld.outside_field_x_minus[0][1]};
    r.major = {mWorld.outside_field_x_minus[1][0], mWorld.outside_field_x_minus[1][1]};
    obs_rectangular.push_back(r);
    r.minor = {mWorld.outside_field_x_plus[0][0], mWorld.outside_field_x_plus[0][1]};
    r.major = {mWorld.outside_field_x_plus[1][0], mWorld.outside_field_x_plus[1][1]};
    obs_rectangular.push_back(r);
    r.minor = {mWorld.outside_field_y_minus[0][0], mWorld.outside_field_y_minus[0][1]};
    r.major = {mWorld.outside_field_y_minus[1][0], mWorld.outside_field_y_minus[1][1]};
    obs_rectangular.push_back(r);
    r.minor = {mWorld.outside_field_y_plus[0][0], mWorld.outside_field_y_plus[0][1]};
    r.major = {mWorld.outside_field_y_plus[1][0], mWorld.outside_field_y_plus[1][1]};
    obs_rectangular.push_back(r);

    auto trajectory = pf.path_find(double_start, double_goal, obs_circular, obs_rectangular);

    return trajectory;
}


std::vector<std::vector<double>> RobotController::find_ball_trajectory(double start[2], double goal[2]) {
    C_trajectory pf(false, false, 0, 1000, 300, 0, mWorld.boundariesMinor, mWorld.boundariesMajor);
    std::vector<double> double_start = {start[0], start[1]};
    std::vector<double> double_goal = {goal[0], goal[1]};
    std::vector<Circle> obs_circular = {};
    std::vector<Rectangle> obs_rectangular = {};

    //add static enemies to obstacles
    for (int i = 0; i < size(mWorld.enemies) ; i++) {
        if (!mWorld.enemies[i].detected) {
            continue;
        }
        Circle c({mWorld.enemies[i].pos[0], mWorld.enemies[i].pos[1]}, mRadius);
        obs_circular.push_back(c);
    }

    auto trajectory = pf.path_find(double_start, double_goal, obs_circular, obs_rectangular);
    return trajectory;
}


std::vector<double> RobotController::motion_planner(std::vector<std::vector<double>> trajectory) {
    for (auto i : trajectory) {
        break;
        std::cout << i[0] << ", " << i[1] << std::endl;
    }
    std::vector<double> delta = {trajectory[1][0] - mpos[0], trajectory[1][1] - mpos[1]};
    double dist = norm(delta) / 1000.0; // metros
    std::vector<double> direction = normalize(1, delta);

    double curve_safe_speed = mVxy_min;
    double v_target_magnitude = mVxy_max;
    if (trajectory.size() > 2) {
        std::vector<double> p0 = {mpos[0], mpos[1]};
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
        curve_safe_speed = sqrt(mA_xy_max * radius)/4;
        curve_safe_speed = std::clamp(curve_safe_speed, mVxy_min, mVxy_max);

        if (angle_between > 70.0 * M_PI / 180.0) {
            curve_safe_speed = mVxy_min;
        }

    }

    double current_speed = sqrt(mvel[0]*mvel[0] + mvel[1]*mvel[1]);

    double brake_distance = (mVxy_max*mVxy_max - curve_safe_speed * curve_safe_speed) / (2.0 * mA_xy_brake);
    brake_distance = std::max(brake_distance, 0.0);
    if (dist <= brake_distance) {
        v_target_magnitude = curve_safe_speed;
    } else {
        v_target_magnitude = mVxy_max;
    }

    v_target_magnitude = std::clamp(v_target_magnitude, -mVxy_max, mVxy_max);
    std::vector<double> v_target = {v_target_magnitude * direction[0], v_target_magnitude * direction[1]};
    std::vector<double> error = {v_target[0] - mlast_target_vel[0], v_target[1] - mlast_target_vel[1]};
    std::vector<double> acceleration = {0, 0};

    if (fabs(v_target[0]) > fabs(mlast_target_vel[0])) {
        acceleration[0] = std::clamp(error[0] / mDelta_time, -mA_xy_max, mA_xy_max);
    } else {
        acceleration[0] = std::clamp(error[0] / mDelta_time, -mA_xy_brake, mA_xy_brake);
    }

    if (fabs(v_target[1]) > fabs(mlast_target_vel[1])) {
        acceleration[1] = std::clamp(error[1] / mDelta_time, -mA_xy_max, mA_xy_max);
    } else {
        acceleration[1] = std::clamp(error[1] / mDelta_time, -mA_xy_brake, mA_xy_brake);
    }


    std::vector<double> vel_cmd = {mlast_target_vel[0] + acceleration[0]*mDelta_time, mlast_target_vel[1] + acceleration[1]*mDelta_time};
    if (norm(vel_cmd) > mVxy_max) {
        vel_cmd = normalize(mVxy_max, vel_cmd);
    }

    if (isnan(vel_cmd[0])) vel_cmd[0] = 0;
    if (isnan(vel_cmd[1])) vel_cmd[1] = 0;

    mlast_target_vel[0] = vel_cmd[0];
    mlast_target_vel[1] = vel_cmd[1];
    return vel_cmd;
}



std::vector<double> RobotController::motion_control(std::vector<double> v_vet) {
    const double ang = -myaw;
    double x = v_vet[0];
    double y = v_vet[1];
    v_vet = {x * cos(ang) - y * sin(ang), x * sin(ang) + y * cos(ang)};
    return v_vet;
}

void RobotController::move_to(double goal[2], bool avoid_ball = true) {

    if (sqrt(pow(goal[0] - mpos[0], 2) + pow(goal[1] - mpos[1], 2)) < mStatic_position_tolarance) {
        mtarget_vel[0] = 0;
        mtarget_vel[1] = 0;
        mtarget_vyaw = 0;
        mPositioned = true;
        mTeam->positioned[mId] = true;
        return;
    }
    mPositioned = false;
    mTeam->positioned[mId] = false;
    std::vector<double> v_vet;
    auto trajectory = find_trajectory(mpos, goal, avoid_ball);
    size(trajectory) > 0 ? v_vet = motion_planner(trajectory) : v_vet = {0, 0};

    v_vet = motion_control(v_vet);

    mtarget_vel[0] = v_vet[0];
    mtarget_vel[1] = v_vet[1];
}

double RobotController::find_angle_error(double goal[2]) {
    double theta_final = atan2(goal[1] - mpos[1], goal[0] - mpos[0]);
    double orientation = myaw;
    double delta = theta_final - orientation;
    if (delta > M_PI) delta -= 2 * M_PI;
    if (delta < -M_PI) delta += 2 * M_PI;
    return delta;
}

double RobotController::turn_control(double delta) {
    double P = mKP_ang * delta;
    mI_ang = mI_ang + delta*mDelta_time*mKI_ang;
    double D = (delta-mLast_delta)*mKD_ang;
    //std::cout << P << ", " << I_ang << ", " << D << std::endl;
    double PID_vyaw = P + mI_ang + D;

    mLast_delta = delta;

    if (fabs(PID_vyaw) > mVyaw_max) {
        PID_vyaw = mVyaw_max*PID_vyaw/fabs(PID_vyaw);
    };
    if (fabs(PID_vyaw) < mVyaw_min && fabs(PID_vyaw) != 0) {
        PID_vyaw = mVyaw_min*PID_vyaw/fabs(PID_vyaw);
    }
    return PID_vyaw;
}


void RobotController::turn_to(double goal[2]) {
    double delta = find_angle_error(goal);
    if (fabs(delta) < mStatic_angle_tolarance) {
        mtarget_vyaw = 0;
        mOriented = true;
        return;
    }
    mOriented = false;
    mtarget_vyaw = turn_control(delta);
}

void RobotController::kick() {
    if (mWorld.ball_speed_module >= mVxy_min) {
        mkicker_x = 0;
        mState += 1;
    }

    std::vector<double> v_vet = {mWorld.ball_pos[0] - mpos[0], mWorld.ball_pos[1] - mpos[1]};
    v_vet = normalize(mVxy_min,v_vet);
    mtarget_vel[0] = v_vet[0]*cos(-myaw) - v_vet[1]*sin(-myaw);
    mtarget_vel[1] = v_vet[0]*sin(-myaw) + v_vet[1]*cos(-myaw);
    mkicker_x = 1000;

}


void RobotController::follow_trajectory(std::vector<std::vector<double>>& trajectory) {

    int i = 0;
    while (size(trajectory) > 0) {
        double distance = sqrt(pow(trajectory[0][0] - mpos[0], 2) + pow(trajectory[0][1] - mpos[1], 2));
        if (size(trajectory) == 1 && distance < mStatic_position_tolarance) {
            trajectory.erase(trajectory.begin());
            break;
        }

        if (distance < mDynamic_position_tolarance) {
            trajectory.erase(trajectory.begin());
            continue;
        }
        break;
    }
    if (size(trajectory) == 0) {
        mtarget_vel[0] = 0;
        mtarget_vel[1] = 0;
        mPositioned = true;
        mTeam->positioned[mId] = true;
        return;
    }

    double next_point[2] = {trajectory[0][0], trajectory[0][1]};
    auto mid_trajectory = find_trajectory(mpos, next_point, true);
    mid_trajectory.insert(mid_trajectory.end(), trajectory.begin() + 1, trajectory.end());
    auto v_vet = motion_planner(mid_trajectory);
    v_vet = motion_control(v_vet);

    mtarget_vel[0] = v_vet[0];
    mtarget_vel[1] = v_vet[1];
    mPositioned = false;
    mTeam->positioned[mId] = false;
}



void RobotController::select_behavior() {
    //TODO roles
    //role reset
    if (lastRole != mTeam->roles[mId]) {
        lastRole = mTeam->roles[mId];
        mState = 0;
    }


    if (mTeam->roles[mId] == TeamInfo::unknown) {
        mtarget_vel[0] = 0;
        mtarget_vel[1] = 0;
        mtarget_vyaw = 0;
    }

    if (mTeam->roles[mId] == TeamInfo::goal_keeper) {
        goal_keeper_role();
    }
    else if (mTeam->roles[mId] == TeamInfo::stricker) {
        stricker_role();
    } else if (mTeam->roles[mId] == TeamInfo::mid_field) {
        mid_field_role();
    } else if (mTeam->roles[mId] == TeamInfo::defender) {
        defender_role();
    }


    if (mTeam->roles[mId] == TeamInfo::debug_circular_trajectory) {
        if (size(mCurrent_trajectory) == 0) {
            int resolution = 120;
            for (int i = 0; i < resolution; i++) {
                mCurrent_trajectory.push_back({1000*sin(2*i*M_PI/resolution + mId*2*M_PI/size(mWorld.allies)), 1000*cos(2*i*M_PI/resolution + mId*2*M_PI/size(mWorld.allies))});
            }
        }
        follow_trajectory(mCurrent_trajectory);
    }

    if (mTeam->roles[mId] == TeamInfo::debug_squared_trajectory) {
        if (size(mCurrent_trajectory) == 0) {
            mCurrent_trajectory = {{4000, 4000}, {4000, -4000}, {4000, 4000}, {-4000, 4000}};
        }
        follow_trajectory(mCurrent_trajectory);
    }
}



void RobotController::stricker_role() {
    //TODO melhorar stricker_role para chutar para o passe
    //TODO fazer interceptar a bola melhor
    double goal[2] = {(mWorld.their_goal[0][1] + mWorld.their_goal[0][0])/2, (mWorld.their_goal[1][1] + mWorld.their_goal[1][0])/2};
    //double goal[2] = {(mWorld.our_goal[0][1] + mWorld.our_goal[0][0])/2, (mWorld.our_goal[1][1] + mWorld.our_goal[1][0])/2};
    bool isPivot;
    int Pivot_id = -1;
    double their_goal[2] = {mWorld.their_goal[0][0], 0};

    auto interceptor = mWorld.getIdOfTheBallInterceptor();
    if ( mWorld.ball_speed_module != 0 && interceptor < 20 && mTeam->num_of_active_robots > 1) {
        Pivot_id = interceptor;
    } else{
        auto closest_allies_to_ball = mWorld.getAlliesIdsAccordingToDistanceToBall();
        if (closest_allies_to_ball[0] != mTeam->goal_keeper_id) {
            Pivot_id = closest_allies_to_ball[0];
        } else if (mTeam->num_of_active_robots > 1) {
            Pivot_id = closest_allies_to_ball[1];
        }
    }

    if (Pivot_id == mId || Pivot_id == -1) {
        if (find_ball_trajectory(mWorld.ball_pos, their_goal).size() == 2 && distance_point(mWorld.ball_pos, their_goal) < mKick_distance) {
            position_and_kick_to_destination(their_goal);
        }
        else {
            int nearest_ally_id = mWorld.findNearestAllyThatIsntTheGoalKeeper(mId, mTeam->goal_keeper_id);
            nearest_ally_id != -1 ? position_and_kick_to_robot(nearest_ally_id) : position_and_kick_to_destination(their_goal);
        }
    }
    else {

        double striker_dislocation = fabs(mWorld.ball_pos[0]) + 2000;
        double x_position = std::clamp(mTeam->central_line_x + (striker_dislocation)*mTeam->our_side_sign, -mTeam->striker_max_dislocation, mTeam->striker_max_dislocation);
        double delta_y = sqrt(fabs(pow(mKick_distance, 2) - pow(x_position - mWorld.ball_pos[0], 2)));
        double y_position;
        mWorld.allies[Pivot_id].pos[1] != 0 ? y_position = mWorld.ball_pos[1] - delta_y*mWorld.allies[Pivot_id].pos[1]/fabs(mWorld.allies[Pivot_id].pos[1])
            : y_position = mWorld.ball_pos[1];
        if (y_position > mWorld.their_defese_area[0][1] - mRadius*2 && y_position < mWorld.their_defese_area[1][1] + mRadius*2) {
            if (y_position > 0) y_position = mWorld.their_defese_area[1][1] + mRadius*2;
            else y_position = mWorld.their_defese_area[0][1] - mRadius*2;
        }
        double wait_position[2] = {x_position, y_position};
        keep_a_location(wait_position);
        turn_to(mWorld.ball_pos);
    }
}

void RobotController::goal_keeper_role() {
    //TODO melhorar goal_keeper_role
    if (!mWorld.isBallOnOurArea() || mWorld.ball_speed_module != 0) {
        mState = 0;
        keep_x_line(mWorld.our_goal[0][0], mWorld.our_goal[1], (mWorld.our_goal[1][0] + mWorld.our_goal[1][1])/2);
    }
    else {
        double goal[2] = {(mWorld.their_goal[0][1] + mWorld.their_goal[0][0])/2, (mWorld.their_goal[1][1] + mWorld.their_goal[1][0])/2};
        position_and_kick_to_destination(goal);
    }
}

void RobotController::mid_field_role() {
    //TODO continuar
    bool isPivot;
    int Pivot_id = -1;
    double their_goal[2] = {mWorld.their_goal[0][0], 0};

    auto interceptor = mWorld.getIdOfTheBallInterceptor();
    if ( mWorld.ball_speed_module != 0 && interceptor < 20 && mTeam->num_of_active_robots > 1) {
        Pivot_id = interceptor;
    } else{
        auto closest_allies_to_ball = mWorld.getAlliesIdsAccordingToDistanceToBall();
        if (closest_allies_to_ball[0] != mTeam->goal_keeper_id) {
            Pivot_id = closest_allies_to_ball[0];
        } else if (mTeam->num_of_active_robots > 1) {
            Pivot_id = closest_allies_to_ball[1];
        }
    }


    if (Pivot_id == mId || Pivot_id == -1) {
        if (mWorld.ball_speed_module != 0) {
            keep_a_location(mpos);
        }
        if (find_ball_trajectory(mWorld.ball_pos, their_goal).size() == 2 && distance_point(mWorld.ball_pos, their_goal) < mKick_distance) {
            position_and_kick_to_destination(their_goal);
        }
        else {
            int nearest_ally_id = mWorld.findNearestAllyThatIsntTheGoalKeeper(mId, mTeam->goal_keeper_id);
            nearest_ally_id != -1 ? position_and_kick_to_robot(nearest_ally_id) : position_and_kick_to_destination(their_goal);
        }
    }
    else {
        double mid_field_dislocation = fabs(mWorld.ball_pos[0]);
        double x_position = std::clamp(mTeam->central_line_x + (mid_field_dislocation)*mTeam->our_side_sign, -mTeam->mid_field_max_dislocation, mTeam->mid_field_max_dislocation);
        double delta_y = sqrt(pow(mKick_distance, 2) - pow(x_position - mWorld.ball_pos[0], 2));
        double y_position;
        mWorld.allies[Pivot_id].pos[1] != 0 ? y_position = mWorld.ball_pos[1] - delta_y*mWorld.allies[Pivot_id].pos[1]/fabs(mWorld.allies[Pivot_id].pos[1])
            : y_position = mWorld.ball_pos[1];
        if (y_position > mWorld.their_defese_area[0][1] - mRadius*2 && y_position < mWorld.their_defese_area[1][1] + mRadius*2) {
            if (y_position > 0) y_position = mWorld.their_defese_area[1][1] + mRadius*2;
            else y_position = mWorld.their_defese_area[0][1] - mRadius*2;
        }
        double wait_position[2] = {x_position, y_position};
        keep_a_location(wait_position);
        turn_to(mWorld.ball_pos);
    }
}

void RobotController::defender_role() {
    //TODO
}

void RobotController::keep_a_location(double keep[2]) {
    if (mWorld.ball_speed_module == 0 || !mWorld.isBallMovingIdDirection(mId)) {
        move_to(keep);
    }
    else {
        if (distance_point(mpos, mWorld.ball_pos) > mRadius*5) move_to(mWorld.ball_stop_position);
        else move_to(mWorld.ball_pos);
    }
}



void RobotController::keep_x_line(double x_line, const double y_segment[2], double y_rest) {
    std::vector<int> enemies_ids = {};
    int enemy_stricker_id = 0;



    for (int i = 0; i < enemies_ids.size(); i++) {
        if (mTeam->enemy_roles[i] == TeamInfo::stricker) {
            enemy_stricker_id = i;
            break;
        }
    }

    double a = 1000;
    if (size(enemies_ids) != 0) {
        if (sqrt(pow(mWorld.ball_speed[0], 2) + pow(mWorld.ball_speed[1], 2)) > mVxy_min) {
            if (mWorld.ball_speed[0] != 0) {
                a = (mWorld.ball_speed[1]) / (mWorld.ball_speed[0]);
            }
        }
        else if (mWorld.ball_pos[0] - mWorld.enemies[enemy_stricker_id].pos[0] != 0) {    //para bola parada
            a = (mWorld.ball_pos[1] - mWorld.enemies[enemy_stricker_id].pos[1]) / (mWorld.ball_pos[0] - mWorld.enemies[enemy_stricker_id].pos[0]);
        }
    }

    double y_meet = a*(x_line) + mWorld.ball_pos[1] -a*mWorld.ball_pos[0];
    double y_max = y_segment[0] - mRadius;
    double y_min = y_segment[1] + mRadius;
    y_meet = std::clamp(y_meet, y_min, y_max);
    double x_meet = x_line - mRadius*x_line/fabs(x_line);
    double meet[2] = {x_meet, y_meet};

    if (!mWorld.isBallOnOurSide() || (distance_point(mWorld.enemies[enemy_stricker_id].pos, mWorld.ball_pos) > 500 && mWorld.ball_speed_module == 0)) {
        meet[1] = y_rest;
    }


    move_to(meet, false);
    mkicker_x = 0;
    turn_to(mWorld.ball_pos);
}

void RobotController::position_and_kick_to_destination(double goal[2]) {
    auto traj = find_ball_trajectory(mWorld.ball_pos, goal);
    std::vector<double> kick_pos = mWorld.getKickingPosition(traj[0], traj[1], mBall_avoidance_radius + mRadius);
    double next_point[2] = {traj[1][0], traj[1][1]};

    if (mState == 0) {
        double db_kick_pos[2] = {kick_pos[0], kick_pos[1]};
        move_to(db_kick_pos);
        turn_to(next_point);
        if (mPositioned and mOriented) {
            mState = 1;
        }
    }
    else if (mState == 1) {
        kick();
    }
    else if (mState == 2) {
        mState = 0;
    }
}

void RobotController::position_and_kick_to_robot(int id) {
    double goal[2] = {mWorld.allies[id].pos[0], mWorld.allies[id].pos[1]};
    auto traj = find_ball_trajectory(mWorld.ball_pos, goal);
    std::vector<double> kick_pos = mWorld.getKickingPosition(traj[0], traj[1], mBall_avoidance_radius + mRadius);
    double next_point[2] = {traj[1][0], traj[1][1]};

    if (mState == 0) {
        double db_kick_pos[2] = {kick_pos[0], kick_pos[1]};
        move_to(db_kick_pos);
        turn_to(next_point);
        if (mPositioned and mOriented and mTeam->positioned[id]) {
            mState = 1;
        }
    }
    else if (mState == 1) {
        kick();
        mTimer = 3;
    }
    else if (mState == 2) {
        move_to(mpos);
        turn_to(mWorld.ball_pos);
        mTimer -= mDelta_time;
        if (mTimer <= 0) {
            mTimer = 0;
            mState = 0;
        }
    }
}



void RobotController::check_connection() {
    if (!mDetected) {
        mOffline_counter++;
    } else {
        mOffline_counter = 0;
    }

    if (mOffline_counter >= mMax_offline_counter) {
        stop();
    }
}


void RobotController::receive_vision() {
    std::unordered_set<int> allies_detected = {};
    std::unordered_set<int> enemies_detected = {};

    for (auto blue_robot : han.new_vision.robots_blue) {
        if (mTeam->color == TeamInfo::blue) {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(mWorld.allies)) {
                for (int i = size(mWorld.allies); i <= rb_id; i++) {
                    mWorld.allies.push_back(Robot(i));
                }
            }

            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (mDelta_time > 0) {
                mWorld.allies[rb_id].stored_speed_x.push_back((blue_robot.position_x - mWorld.allies[rb_id].pos[0])/(mDelta_time*1000));
                mWorld.allies[rb_id].stored_speed_y.push_back((blue_robot.position_y - mWorld.allies[rb_id].pos[1])/(mDelta_time*1000));
                if (size(mWorld.allies[rb_id].stored_speed_x) > 10) {
                    mWorld.allies[rb_id].stored_speed_x.pop_front();
                    mWorld.allies[rb_id].stored_speed_y.pop_front();
                }
                mWorld.allies[rb_id].vel[0] = std::accumulate(mWorld.allies[rb_id].stored_speed_x.begin(), mWorld.allies[rb_id].stored_speed_x.end(), 0.0)/10;
                mWorld.allies[rb_id].vel[1] = std::accumulate(mWorld.allies[rb_id].stored_speed_y.begin(), mWorld.allies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            mWorld.allies[rb_id].yaw = new_yaw;
            mWorld.allies[rb_id].pos[0] = blue_robot.position_x;
            mWorld.allies[rb_id].pos[1] = blue_robot.position_y;
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = blue_robot.robot_id;
            if (rb_id >= size(mWorld.enemies)) {
                for (int i = size(mWorld.enemies); i <= rb_id; i++) {
                    mWorld.enemies.push_back(Robot(i));
                }
            }

            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (mDelta_time > 0) {
                mWorld.enemies[rb_id].stored_speed_x.push_back((blue_robot.position_x - mWorld.enemies[rb_id].pos[0])/(mDelta_time*1000));
                mWorld.enemies[rb_id].stored_speed_y.push_back((blue_robot.position_y - mWorld.enemies[rb_id].pos[1])/(mDelta_time*1000));
                if (size(mWorld.enemies[rb_id].stored_speed_x) > 10) {
                    mWorld.enemies[rb_id].stored_speed_x.pop_front();
                    mWorld.enemies[rb_id].stored_speed_y.pop_front();
                }
                mWorld.enemies[rb_id].vel[0] = std::accumulate(mWorld.enemies[rb_id].stored_speed_x.begin(), mWorld.enemies[rb_id].stored_speed_x.end(), 0.0)/10;
                mWorld.enemies[rb_id].vel[1] = std::accumulate(mWorld.enemies[rb_id].stored_speed_y.begin(), mWorld.enemies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            mWorld.enemies[rb_id].yaw = new_yaw;
            mWorld.enemies[rb_id].pos[0] = blue_robot.position_x;
            mWorld.enemies[rb_id].pos[1] = blue_robot.position_y;
            enemies_detected.insert(rb_id);
        }
    }


    for (auto yellow_robot : han.new_vision.robots_yellow) {
        if (mTeam->color == TeamInfo::yellow) {
            int rb_id = yellow_robot.robot_id;
            if (rb_id >= size(mWorld.allies)) {
                for (int i = size(mWorld.allies); i <= rb_id; i++) {
                    mWorld.allies.push_back(Robot(i));
                }
            }
            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (mDelta_time > 0) {
                mWorld.allies[rb_id].stored_speed_x.push_back((yellow_robot.position_x - mWorld.allies[rb_id].pos[0])/(mDelta_time*1000));
                mWorld.allies[rb_id].stored_speed_y.push_back((yellow_robot.position_y - mWorld.allies[rb_id].pos[1])/(mDelta_time*1000));
                if (size(mWorld.allies[rb_id].stored_speed_x) > 10) {
                    mWorld.allies[rb_id].stored_speed_x.pop_front();
                    mWorld.allies[rb_id].stored_speed_y.pop_front();
                }
                mWorld.allies[rb_id].vel[0] = std::accumulate(mWorld.allies[rb_id].stored_speed_x.begin(), mWorld.allies[rb_id].stored_speed_x.end(), 0.0)/10;
                mWorld.allies[rb_id].vel[1] = std::accumulate(mWorld.allies[rb_id].stored_speed_y.begin(), mWorld.allies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            mWorld.allies[rb_id].yaw = new_yaw;
            mWorld.allies[rb_id].pos[0] = yellow_robot.position_x;
            mWorld.allies[rb_id].pos[1] = yellow_robot.position_y;
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = yellow_robot.robot_id;
            if (rb_id >= size(mWorld.enemies)) {
                for (int i = size(mWorld.enemies); i <= rb_id; i++) {
                    mWorld.enemies.push_back(Robot(i));
                }
            }
            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (mDelta_time > 0) {
                mWorld.enemies[rb_id].stored_speed_x.push_back((yellow_robot.position_x - mWorld.enemies[rb_id].pos[0])/(mDelta_time*1000));
                mWorld.enemies[rb_id].stored_speed_y.push_back((yellow_robot.position_y - mWorld.enemies[rb_id].pos[1])/(mDelta_time*1000));
                if (size(mWorld.enemies[rb_id].stored_speed_x) > 10) {
                    mWorld.enemies[rb_id].stored_speed_x.pop_front();
                    mWorld.enemies[rb_id].stored_speed_y.pop_front();
                }
                mWorld.enemies[rb_id].vel[0] = std::accumulate(mWorld.enemies[rb_id].stored_speed_x.begin(), mWorld.enemies[rb_id].stored_speed_x.end(), 0.0)/10;
                mWorld.enemies[rb_id].vel[1] = std::accumulate(mWorld.enemies[rb_id].stored_speed_y.begin(), mWorld.enemies[rb_id].stored_speed_y.end(), 0.0)/10;
            }
            mWorld.enemies[rb_id].yaw = new_yaw;
            mWorld.enemies[rb_id].pos[0] = yellow_robot.position_x;
            mWorld.enemies[rb_id].pos[1] = yellow_robot.position_y;
            enemies_detected.insert(rb_id);
        }
    }

    for (int i = 0; i < size(mWorld.allies); i++) {
        if (allies_detected.find(i) != allies_detected.end()) mWorld.allies[i].detected = true;
        else mWorld.allies[i].detected = false;
        }

    for (int i = 0; i < size(mWorld.enemies); i++) {
        if (enemies_detected.find(i) != enemies_detected.end()) mWorld.enemies[i].detected = true;
        else mWorld.enemies[i].detected = false;
    }

    if (mDelta_time != 0) {
        mWorld.ball_speed[0] = (han.new_vision.balls.position_x - mWorld.ball_pos[0])/(mDelta_time*1000);
        mWorld.ball_speed[1] = (han.new_vision.balls.position_y - mWorld.ball_pos[1])/(mDelta_time*1000);
    }

    mWorld.ball_pos[0] = han.new_vision.balls.position_x;
    mWorld.ball_pos[1] = han.new_vision.balls.position_y;
    mWorld.ball_speed_module = sqrt(mWorld.ball_speed[0]*mWorld.ball_speed[0] + mWorld.ball_speed[1]*mWorld.ball_speed[1]);
    mWorld.generateBallStopPosition();


    mLast_time_stamp = han.new_vision.timestamp;

    mpos[0] = mWorld.allies[mId].pos[0];
    mpos[1] = mWorld.allies[mId].pos[1];
    myaw = mWorld.allies[mId].yaw;
    mvel[0] = mWorld.allies[mId].vel[0];
    mvel[1] = mWorld.allies[mId].vel[1];
    mvyaw = mWorld.allies[mId].vyaw;
    mDetected = mWorld.allies[mId].detected;
}



void RobotController::loadCalibration() {
    RobotCalibration load;
    mKick_distance = load.kickDistance;
}


void RobotController::publish() {
    /*
    std::cout << id << std::endl;
    std::cout << target_vel[1] << std::endl;
    std::cout << target_vel[0] << std::endl;
    std::cout << target_vyaw << std::endl;<
    */


    han.new_ia.robots[mId].kick_speed_x = 1050;
    han.new_ia.robots[mId].id = mId;
    han.new_ia.robots[mId].vel_normal = mtarget_vel[1];
    han.new_ia.robots[mId].vel_tang = mtarget_vel[0];
    han.new_ia.robots[mId].vel_ang = static_cast<float>(mtarget_vyaw);
    if (mkicker_x != 0) {
        han.new_ia.robots[mId].kick_speed_x = mKicker_x_max;
    } else {
        han.new_ia.robots[mId].kick_speed_x = 0;
    }
    han.lc->publish("IA", &han.new_ia);
    han.lc->publish("tartarus", &han.new_tartarus);

}
