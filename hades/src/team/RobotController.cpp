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
        tactics::follow_trajectory(*this, mCurrent_trajectory);
    }

    if (mTeam->roles[mId] == TeamInfo::debug_squared_trajectory) {
        if (size(mCurrent_trajectory) == 0) {
            mCurrent_trajectory = {{4000, 4000}, {4000, -4000}, {4000, 4000}, {-4000, 4000}};
        }
        tactics::follow_trajectory(*this, mCurrent_trajectory);
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
        if (tactics::aux::find_ball_trajectory(*this, mWorld.ball_pos, their_goal).size() == 2 && distance_point(mWorld.ball_pos, their_goal) < mKick_distance) {
            tactics::position_and_kick_to_destination(*this, their_goal);
        }
        else {
            int nearest_ally_id = mWorld.findNearestAllyThatIsntTheGoalKeeper(mId, mTeam->goal_keeper_id);
            nearest_ally_id != -1 ? tactics::position_and_kick_to_robot(*this, nearest_ally_id) : tactics::position_and_kick_to_destination(*this, their_goal);
        }
    }
    else {

        double striker_dislocation = fabs(mWorld.ball_pos[0]) + 1000;
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
        tactics::keep_a_location(*this, wait_position);
        skills::turn_to(*this, mWorld.ball_pos);
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
    han.new_ia.robots[mId].id = mId;
    han.new_ia.robots[mId].vel_normal = mtarget_vel[1];
    han.new_ia.robots[mId].vel_tang = mtarget_vel[0];
    han.new_ia.robots[mId].vel_ang = static_cast<float>(mtarget_vyaw);
    if (mkicker_x != 0) {
        han.new_ia.robots[mId].kick = true;
        han.new_ia.robots[mId].kick_speed_x = mkicker_x;
    } else han.new_ia.robots[mId].kick = false;

    han.lc->publish("IA", &han.new_ia);
}
