#include "Particle.h"

Particle::Particle(const Vector pos, const Vector vel, const Vector acc, const float mass, const Vector accumForce)
    : pos(pos)
      , vel(vel)
      , acc(acc)
      , accumForce(accumForce)
      , inverseMass(mass != 0.f ? 1.f / mass : 0.f) {
}

Vector Particle::integrateVelocity(const float dt) {
    if (inverseMass == 0.0f) {
        // Infinite mass, no movement
        return vel;
    }

    acc = accumForce * inverseMass;
    vel += dt * acc;
    return vel;
}

Vector Particle::integratePosition(const float dt) {
    if (inverseMass == 0.0f) {
        // Infinite mass, no movement
        return pos;
    }

    pos += dt * vel;
    return pos;
}

void Particle::addforce(const Vector &force) {
    if (inverseMass == 0.0f) {
        // Infinite mass, no movement
        return;
    }

    accumForce += force;
}

void Particle::clearAccum() {
    if (inverseMass == 0.0f) {
        // Infinite mass, no movement
        return;
    }

    accumForce = Vector{0, 0, 0};
}
