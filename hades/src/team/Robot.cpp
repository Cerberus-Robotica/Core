//
// Created by caiom on 4/7/2025.
//

#include "Robot.h"

// --- Getters ---
bool Robot::isAlly() const {
	return ally;
}

int Robot::getId() const {
	return id;
}

Point Robot::getOldPosition() const {
	return old_pos;
}

Point Robot::getPosition() const {
	return pos;
}

double Robot::getYaw() const {
	return yaw;
}

Vector2d& Robot::getVelocity() {
	return vel;
}

double Robot::getVyaw() const {
	return vyaw;
}

bool Robot::isDetected() const {
	return detected;
}

enum Robot::role Robot::getRole() const {
	return this_role;
}

const std::deque<Vector2d>& Robot::getStoredVelocities() const {
	return stored_velocities;
}

// --- Setters ---
void Robot::setAlly(bool is) {
	this->ally = is;
}

void Robot::setId(int id) {
	this->id = id;
}

void Robot::setPosition(const Point& p) {
	old_pos = pos;
	pos = p;
}

void Robot::setYaw(double y) {
	yaw = y;
}

void Robot::setVelocity(const Vector2d& v) {
	vel = v;
}

void Robot::setVyaw(double v) {
	vyaw = v;
}

void Robot::setDetected(bool d) {
	detected = d;
}

void Robot::setRole(enum Robot::role r) {
	this_role = r;
}

// --- Stored Velocities ---
void Robot::setStoredVelocities(const std::deque<Vector2d>& vels) {
	stored_velocities = vels;
}

