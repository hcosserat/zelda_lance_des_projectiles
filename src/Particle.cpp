#include "Particle.h"

Particle::Particle(const Vector pos, const Vector vel, const Vector acc, const float mass) {
    this->pos = pos;
    this->prevPos = Vector(NAN, NAN, NAN);
    this->vel = vel;
    this->acc = acc;
    this->mass = mass;
    this->inverseMass = 1 / mass;
}

Vector Particle::updateEuler(const float dt, const float dampingFactor) {
    prevPos = pos;

    acc -= dampingFactor * vel;  // damping force

    const auto prevVel = vel;
    const auto posIncr = prevVel + dt * 0.5f * acc;

    vel += dt * acc;
    pos += dt * posIncr;

    return pos;
}

Vector Particle::updateVerlet(const float dt, const float dampingFactor) {
    // First iteration uses Eurler method, to initialize prevPos
    if (prevPos == Vector(NAN, NAN, NAN)) {
        return this->updateEuler(dt);
    }

    acc -= dampingFactor * vel; // damping force

    const auto temp = pos;
    const auto velIncr = dt * acc;

    vel += velIncr;
    pos += pos - prevPos + dt * velIncr;

    prevPos = temp;

    return pos;
}
