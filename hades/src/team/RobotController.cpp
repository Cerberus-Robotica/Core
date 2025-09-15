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
#include "TeamInfo.h"


void RobotController::start(TeamInfo* team) {
    han.new_ia.robots[id].kick_speed_x = 3;
    setActive(true);
    this->mTeam = team;
    mTeam->num_of_active_robots++;
    mTerminate = false;
    mOffline_counter = 0;
    loadCalibration();
    std::cout << "iniciado " << id << std::endl;
    std::thread t(&RobotController::loop, this);
    t.detach();
}

void RobotController::stop() {
    mTeam->active_robots[id] = false;
    mTeam->roles[id] = unknown;
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
        receive_field_geometry();
        check_connection();
        select_behavior();
        //mTeam->role_map[support]->act(*this);
        publish();
        std::chrono::duration<double> delta = t1 - t0;
        t0 = std::chrono::steady_clock::now();
        mDelta_time = delta.count();
    }
    std::cout << "Encerrado " << id << std::endl;
}

void RobotController::setActive(bool active) {
    this->active = active;
}

bool RobotController::isActive() {
    return this->active;
}


void RobotController::select_behavior() {
    //TODO roles
    //role reset
    if (lastRole != mTeam->roles[id]) {
        lastRole = mTeam->roles[id];
        mState = 0;
    }

    try {
        mTeam->role_map[mTeam->roles[id]]->act(*this);
    }
    catch (...) {
        mTeam->role_map[Robot::halted]->act(*this);
        //when role inst on role_map
    }

}

void RobotController::check_connection() {
    if (!isDetected()) {
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
        if (!blue_robot.detected) continue;
        if (mTeam->color == TeamInfo::blue) {
            int rb_id = blue_robot.robot_id;
            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (mDelta_time > 0) {
                //TODO logica quebrada na troca de estrutura, refazer.
                Vector2d v = {((blue_robot.position_x - mWorld.allies[rb_id].getPosition().getX())/(mDelta_time*1000)), ((blue_robot.position_y - mWorld.allies[rb_id].getPosition().getY())/(mDelta_time*1000))};
                auto velocities = mWorld.allies[rb_id].getStoredVelocities();
                velocities.push_back(v);
                if (size(mWorld.allies[rb_id].getStoredVelocities()) > 10) {
                    velocities.pop_front();
                }
                mWorld.allies[rb_id].getVelocity().setX(v.getX()); // <<< aqui!
                mWorld.allies[rb_id].getVelocity().setY(v.getY()); // <<< e aqui!
                mWorld.allies[rb_id].setStoredVelocities(velocities);
            }
            mWorld.allies[rb_id].setYaw(new_yaw);
            mWorld.allies[rb_id].setPosition({blue_robot.position_x, blue_robot.position_y});
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = blue_robot.robot_id;
            double new_yaw = blue_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (mDelta_time > 0) {
                //TODO
                Vector2d v = {((blue_robot.position_x - mWorld.enemies[rb_id].getPosition().getX())/(mDelta_time*1000)), ((blue_robot.position_y - mWorld.allies[rb_id].getPosition().getY())/(mDelta_time*1000))};
                auto velocities = mWorld.enemies[rb_id].getStoredVelocities();
                velocities.push_back(v);
                if (size(mWorld.enemies[rb_id].getStoredVelocities()) > 10) {
                    velocities.pop_front();
                }
                mWorld.enemies[rb_id].getVelocity().setX(v.getX()); // <<< aqui!
                mWorld.enemies[rb_id].getVelocity().setY(v.getY()); // <<< e aqui!
                mWorld.enemies[rb_id].setStoredVelocities(velocities);
            }
            mWorld.enemies[rb_id].setYaw(new_yaw);
            mWorld.enemies[rb_id].setPosition({blue_robot.position_x, blue_robot.position_y});
            enemies_detected.insert(rb_id);
        }
    }


    for (auto yellow_robot : han.new_vision.robots_yellow) {
        if (!yellow_robot.detected) continue;
        if (mTeam->color == TeamInfo::yellow) {
            int rb_id = yellow_robot.robot_id;
            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (mDelta_time > 0) {
                //TODO logica quebrada na troca de estrutura, refazer.
                Vector2d v = {((yellow_robot.position_x - mWorld.allies[rb_id].getPosition().getX())/(mDelta_time*1000)), ((yellow_robot.position_y - mWorld.allies[rb_id].getPosition().getY())/(mDelta_time*1000))};
                auto velocities = mWorld.allies[rb_id].getStoredVelocities();
                velocities.push_back(v);
                if (size(mWorld.allies[rb_id].getStoredVelocities()) > 10) {
                    velocities.pop_front();
                }
                mWorld.allies[rb_id].getVelocity().setX(v.getX()); // <<< aqui!
                mWorld.allies[rb_id].getVelocity().setY(v.getY()); // <<< e aqui!
                mWorld.allies[rb_id].setStoredVelocities(velocities);
            }
            mWorld.allies[rb_id].setYaw(new_yaw);
            mWorld.allies[rb_id].setPosition({yellow_robot.position_x, yellow_robot.position_y});
            allies_detected.insert(rb_id);
        }
        else {
            int rb_id = yellow_robot.robot_id;

            double new_yaw = yellow_robot.orientation;
            if (new_yaw < 0) new_yaw += 2*M_PI;
            if (mDelta_time > 0) {
                //TODO
                Vector2d v = {((yellow_robot.position_x - mWorld.enemies[rb_id].getPosition().getX())/(mDelta_time*1000)), ((yellow_robot.position_y - mWorld.allies[rb_id].getPosition().getY())/(mDelta_time*1000))};
                auto velocities = mWorld.enemies[rb_id].getStoredVelocities();
                velocities.push_back(v);
                if (size(mWorld.enemies[rb_id].getStoredVelocities()) > 10) {
                    velocities.pop_front();
                }
                mWorld.enemies[rb_id].getVelocity().setX(v.getX()); // <<< aqui!
                mWorld.enemies[rb_id].getVelocity().setY(v.getY()); // <<< e aqui!
                mWorld.enemies[rb_id].setStoredVelocities(velocities);
            }
            mWorld.enemies[rb_id].setYaw(new_yaw);
            mWorld.enemies[rb_id].setPosition({yellow_robot.position_x, yellow_robot.position_y});
            enemies_detected.insert(rb_id);
        }
    }

    for (int i = 0; i < size(mWorld.allies); i++) {
        if (allies_detected.find(i) != allies_detected.end()) mWorld.allies[i].setDetected(true);
        else mWorld.allies[i].setDetected(false);
        }

    for (int i = 0; i < size(mWorld.enemies); i++) {
        if (enemies_detected.find(i) != enemies_detected.end()) mWorld.enemies[i].setDetected(true);
        else mWorld.enemies[i].setDetected(false);
    }

    if (mDelta_time != 0) {
        mWorld.ball.setVelocity({(han.new_vision.balls.position_x - mWorld.ball.getPosition().getX())/(mDelta_time*1000), (han.new_vision.balls.position_y - mWorld.ball.getPosition().getY())/(mDelta_time*1000)});
    }

    mWorld.ball.setPosition({han.new_vision.balls.position_x, han.new_vision.balls.position_y});
    VisibilityGraph graph;
    for (Robot enemy : mWorld.enemies) {
        if (!enemy.isDetected()) continue;
        graph.addShadow(CircularShadow(mWorld.ball.getPosition(), {enemy.getPosition(), enemy.getRadius()}));
    }
    mWorld.ball.setVisibilityGraph(graph);

    mLast_time_stamp = han.new_vision.timestamp;

    if (mWorld.allies.size() >= id) {
        setPosition(mWorld.allies[id].getPosition());
        setYaw(mWorld.allies[id].getYaw());
        setVelocity(mWorld.allies[id].getVelocity());
        setVyaw(mWorld.allies[id].getVyaw());
        setDetected(mWorld.allies[id].isDetected());
    }
}

void RobotController::receive_field_geometry() {
    //TODO implementar urgente
    mWorld.field.inside_dimensions.setMinorPoint({static_cast<double>(-han.new_vision.field.field_length/2), static_cast<double>(-han.new_vision.field.field_width/2)});
    mWorld.field.inside_dimensions.setMajorPoint({static_cast<double>(han.new_vision.field.field_length/2), static_cast<double>(han.new_vision.field.field_width/2)});

    AreaRectangular leftDefenseArea = {{-han.new_vision.field.field_width/2, -han.new_vision.field.defense_area_height/2},{-han.new_vision.field.field_width/2 + han.new_vision.field.defense_area_width, han.new_vision.field.defense_area_height/2}};
    AreaRectangular rightDefenseArea = {{han.new_vision.field.field_width/2 - han.new_vision.field.defense_area_width, -han.new_vision.field.defense_area_height/2}, {han.new_vision.field.field_width/2, han.new_vision.field.defense_area_height/2}};

    LineSegment leftGoal = {Point(-han.new_vision.field.field_length/2 , -han.new_vision.field.goal_height/2), Point(-han.new_vision.field.field_length/2 , han.new_vision.field.goal_height/2)};
    LineSegment rightGoal = {Point(han.new_vision.field.field_length/2 , -han.new_vision.field.goal_height/2), Point(han.new_vision.field.field_length/2 , han.new_vision.field.goal_height/2)};

    AreaRectangular leftFisicalBarrier = {leftGoal.getStart(), {leftGoal.getEnd().getX() - han.new_vision.field.goal_depth, leftGoal.getEnd().getY()}};
    AreaRectangular rightFisicalBarrier = {rightGoal.getStart(), {rightGoal.getEnd().getX() + han.new_vision.field.goal_depth, rightGoal.getEnd().getY()}};
    if (mTeam->our_side == TeamInfo::left) {
        mWorld.field.ourFisicalBarrier = leftFisicalBarrier;
        mWorld.field.theirFisicalBarrier = rightFisicalBarrier;
        //mWorld.field.ourGoal = leftGoal;
        //mWorld.field.theirGoal = rightGoal;
        mWorld.field.ourDefenseArea = leftDefenseArea;
        mWorld.field.theirDefenseArea = rightDefenseArea;
    }
    if (mTeam->our_side == TeamInfo::right) {
        mWorld.field.ourFisicalBarrier = rightFisicalBarrier;
        mWorld.field.theirFisicalBarrier = leftFisicalBarrier;
        //mWorld.field.ourGoal = rightGoal;
        //mWorld.field.theirGoal = leftGoal;
        mWorld.field.ourDefenseArea = rightDefenseArea;
        mWorld.field.theirDefenseArea = leftDefenseArea;
    }
}

void RobotController::loadCalibration() {
    //TODO
}


void RobotController::publish() {
    han.new_ia.robots[id].id = id;



    han.new_ia.robots[id].vel_normal = mtarget_vel.getY();
    han.new_ia.robots[id].vel_tang = mtarget_vel.getX();
    han.new_ia.robots[id].vel_ang = static_cast<float>(mtarget_vyaw);
    if (mkicker_x != 0) {
        han.new_ia.robots[id].kick = true;
        han.new_ia.robots[id].kick_speed_x = mkicker_x;
    } else {
        han.new_ia.robots[id].kick = false;
        han.new_ia.robots[id].kick_speed_x = mkicker_x;
    }
    han.lc->publish("IA", &han.new_ia);
}
