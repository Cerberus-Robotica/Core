//
// Created by caiu on 21/08/25.
//

#include "Ball.h"
#include <cmath>

// --- Setters e Getters ---
void Ball::setVelocity(const Vector2d& v) {
    velocity = v;
    stopPosition = findStopPosition();
}

Vector2d Ball::getVelocity() const {
    return velocity;
}

void Ball::setDetected(bool d) {
    detected = d;
}

bool Ball::getDetected() {
    return detected;
}

void Ball::setPosition(Point p) {
    position = p;
    stopPosition = findStopPosition();
}

Point Ball::getPosition() {
    return position;
}

// --- Cálculo da posição final da bola ---
Point Ball::findStopPosition() const {
    // Se a bola está parada, posição final é a atual
    if (velocity.getX() == 0 && velocity.getY() == 0) {
        return position;
    }

    // Módulo da velocidade inicial
    double v0 = std::sqrt(velocity.getX() * velocity.getX() +
                          velocity.getY() * velocity.getY());

    // Distância até parar (Torricelli)
    double distance = (v0 * v0) / (2 * deceleration);

    // Direção da velocidade (vetor normalizado)
    double dirX = velocity.getX() / v0;
    double dirY = velocity.getY() / v0;

    // Posição final = posição atual + deslocamento
    return Point(position.getX() + dirX * distance,
                 position.getY() + dirY * distance);
}
