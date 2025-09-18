//
// Created by caiom on 4/7/2025.
//

#ifndef ROBOT_H
#define ROBOT_H

#include <deque>
#include "geometry/Point.h"
#include "geometry/Vector2d.h"

class Robot {
public:
    enum role {
        unknown = -1,
        goal_keeper,
        striker,
        support,
        defender,
        halted,
        kickoff_kicker,
        kickoff_goal_keeper,
        kickoff_support,
        marker,
        debug_circular_trajectory,
        debug_squared_trajectory,
        retaker
    };

protected:
    bool ally = true;
    int id = -1;
    Point old_pos = {0, 0};
    Point pos = {0, 0};
    double yaw = 0.0;
    Vector2d vel = Vector2d(0,0);
    std::deque<Vector2d> stored_velocities = {};
    double vyaw = 0.0;
    double radius = 160;
    bool detected = false;
    double kickDistance = 3000;
    enum role this_role = unknown;

public:
    // Construtor
    Robot(int id) : id(id) {this_role = unknown;}

    //flags
    bool positioned = true;
    bool aligned = true;
    bool oriented = true;

    // --- Getters ---
    bool isAlly() const;
    int getId() const;
    Point getOldPosition() const;
    Point getPosition() const;
    double getYaw() const;
    Vector2d& getVelocity();
    double getVyaw() const;
    bool isDetected() const;
    enum role getRole() const;
    const std::deque<Vector2d>& getStoredVelocities() const;
    double getRadius() const;
    double getKickDistance() const {return kickDistance;};

    // --- Setters ---
    void setAlly(bool is);
    void setId(int id);
    void setPosition(const Point& p);
    void setYaw(double y);
    void setVelocity(const Vector2d& v);
    void setVyaw(double v);
    void setDetected(bool d);
    void setRole(enum role r);
    void setStoredVelocities(const std::deque<Vector2d>& vels);

};

#endif //ROBOT_H
