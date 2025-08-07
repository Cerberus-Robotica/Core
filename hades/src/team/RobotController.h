//
// Created by caiom on 4/4/2025.
//

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H
#include <vector>
#include <lcm/lcm-cpp.hpp>

#include "Robot.h"
#include "TeamInfo.h"
#include "geometry/WorldModel.h"
#include "RobotCalibration.h"

class RobotController {

public:


    RobotController(int new_id){
        mId = new_id;
    }

    void start(TeamInfo* team_ads);
    void stop();
    void loop();



private:

    int mId = -1; //id; -1 unsigned
    double mRadius = 160;
    double mBall_avoidance_radius = 0;

    //observed status
    double mpos[2] = {0, 0};
    double myaw = 0;
    double mvel[2] = {0, 0};
    double mvyaw = 0;

    //target movimentation
    double mtarget_yaw = 0;
    double mtarget_vel[2] = {0, 0};
    double mtarget_vyaw = 0;
    double mlast_target_vel[2] = {0, 0};

    //actuators activation
    double mkicker_x = 0;
    double mkicker_z = 0;
    double mdribbler = 0;

    //strategy status
    TeamInfo* mTeam;  //role; -1 unsigned
    TeamInfo::role lastRole = TeamInfo::unknown;

    int mState = 0;  //estado

    double mDelta_time = 0;
    double mTimer = 0;
    std::vector<std::vector<double>> mCurrent_trajectory = {};



    //on-field detection
    bool mDetected = true;
    int mOffline_counter = 0;
    int mMax_offline_counter = 100;
    bool mTerminate = false;


    //extreme params
    double mVxy_max = 1.5;
    double mVxy_min = 0.2;
    double mA_xy_max = 2;
    double mA_xy_brake = 2;
    double mVyaw_max = 10;
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
    double mStatic_position_tolarance = mRadius/8;
    double mDynamic_position_tolarance = mRadius/16;
    double mStatic_angle_tolarance = 0.005;

    //PID control
    double mKP_ang = 10;
    double mKI_ang = 0.5;
    double mKD_ang = 5;
    double mI_ang = 0;
    double mLast_delta = 0;

    double mKP_mov = 0;
    double mKI_mov = 0;
    double mKD_mov = 0;
    double mI_mov = 0;

    //flags
    bool mPositioned = true;
    bool mAlligned = true;
    bool mOriented = true;


    //field info
    WorldModel mWorld;
    int64_t mLast_time_stamp = 0;


    void check_connection();
    void dynamic_calculations();

    void move_to(double goal[2], bool avoid_ball);
    std::vector<std::vector<double>> find_trajectory(double start[2], double goal[2], bool avoid_ball);
    std::vector<std::vector<double>> find_ball_trajectory(double start[2], double goal[2]);
    std::vector<double> motion_planner(std::vector<std::vector<double>> trajectory);
    std::vector<double> motion_control(std::vector<double> v_vet);


    void turn_to(double goal[2]);
    double find_angle_error(double goal[2]);
    double turn_control(double delta);


    void kick();
    void follow_trajectory(std::vector<std::vector<double>>& trajectory);

    void receive_vision();
    void receive_field_geometry();
    void publish();
    void loadCalibration();

    void select_behavior();
    void stricker_role();
    void goal_keeper_role();
    void mid_field_role();
    void defender_role();

    void keep_x_line(double x_line, const double y_segment[2], double y_rest);
    void keep_a_location(double keep[2]);
    void position_and_kick_to_destination(double goal[2]);
    void position_and_kick_to_robot(int id);
};



#endif //ROBOT_H
