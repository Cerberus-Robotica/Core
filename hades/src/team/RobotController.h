//
// Created by caiom on 4/4/2025.
//

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H
#include <vector>
#include <lcm/lcm-cpp.hpp>

#include "Robot.h"
#include "geometry/WorldModel.h"
#include "geometry/geometry.h"
#include "skills/skills.h"
#include "tactics/tactics.h"
#include "roles/roles.h"

class TeamInfo;

class RobotController : public Robot{

public:
    RobotController(int new_id): Robot(new_id), mWorld() {
        id = new_id;
        setPosition({0, 0});
    }

    void start(TeamInfo* team_ads);
    void stop();
    void loop();
    bool isActive();

    bool active = false;

    double mRadius = 160;
    double mBall_avoidance_radius = 0;

    //target movimentation
    double mtarget_yaw = 0;
    Vector2d mtarget_vel = {0, 0};
    double mtarget_vyaw = 0;
    Vector2d mlast_target_vel = {0, 0};

    //actuators activationstop
    double mkicker_x = 0;
    double mkicker_z = 0;
    double mdribbler = 0;

    //strategy status
    TeamInfo* mTeam;  //role; -1 unsigned

    int mState = 0;  //estado

    double mDelta_time = 0;
    double mTimer = 0;
    std::vector<Point> mCurrent_trajectory = {};



    //on-field detection
    int mOffline_counter = 0;
    int mMax_offline_counter = 100;
    bool mTerminate = false;


    //extreme params
    double mVxy_max = 1.5;
    double mVxy_min = 0.2;
    double mA_xy_max = 2;
    double mA_xy_brake = 2;
    double mVyaw_max = 5;
    double mVyaw_min = 0.5;
    double mA_ang_max = 15;
    double mKick_distance = 3000;
    double mKicker_x_max = 3;
    double mKicker_x_min = 0.5;
    double mKicker_z_max = 1;
    double mKicker_z_min = 0.5;
    double mDribbler_max = 1;
    double mDribbler_min = 0.5;

    //angle and position tolerance
    double mStatic_position_tolarance = mRadius/4;
    double mDynamic_position_tolarance = mRadius/16;
    double mStatic_angle_tolarance = 0.005;

    //PID control
    double mKP_ang = 10;
    double mKI_ang = 0.0005;
    double mKD_ang = 5;
    double mI_ang = 0;
    double mLast_delta = 0;

    double mKP_mov = 0;
    double mKI_mov = 0;
    double mKD_mov = 0;
    double mI_mov = 0;

    //field info
    WorldModel mWorld;
    int64_t mLast_time_stamp = 0;

    enum role lastRole = unknown;

private:

    void check_connection();
    void dynamic_calculations();
    void setActive(bool active);

    void receive_vision();
    void receive_field_geometry();
    void publish();
    void loadCalibration();

    void select_behavior();

};



#endif //ROBOT_H
