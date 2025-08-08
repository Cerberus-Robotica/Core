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
        roles::goal_keeper(*this);
    } else if (mTeam->roles[mId] == TeamInfo::striker) {
        roles::striker(*this);
    } else if (mTeam->roles[mId] == TeamInfo::mid_field) {
        roles::mid_field(*this);
    } else if (mTeam->roles[mId] == TeamInfo::defender) {
        roles::defender(*this);
    } else if (mTeam->roles[mId] == TeamInfo::halted) {
        roles::halted(*this);
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

    if (mWorld.allies.size() >= mId) {
        mpos[0] = mWorld.allies[mId].pos[0];
        mpos[1] = mWorld.allies[mId].pos[1];
        myaw = mWorld.allies[mId].yaw;
        mvel[0] = mWorld.allies[mId].vel[0];
        mvel[1] = mWorld.allies[mId].vel[1];
        mvyaw = mWorld.allies[mId].vyaw;
        mDetected = mWorld.allies[mId].detected;
    }
}



void RobotController::loadCalibration() {
    //TODO
}


void RobotController::publish() {
    han.new_ia.robots[mId].id = mId;
    han.new_ia.robots[mId].vel_normal = mtarget_vel[1];
    han.new_ia.robots[mId].vel_tang = mtarget_vel[0];
    han.new_ia.robots[mId].vel_ang = static_cast<float>(mtarget_vyaw);
    if (mkicker_x != 0) {
        han.new_ia.robots[mId].kick = true;
        han.new_ia.robots[mId].kick_speed_x = mkicker_x;
    } else {
        han.new_ia.robots[mId].kick = false;
        han.new_ia.robots[mId].kick_speed_x = mkicker_x;
    }
    han.lc->publish("IA", &han.new_ia);
}
