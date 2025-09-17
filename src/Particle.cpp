#include "Particle.h"

Particle::Particle(const Vector pos, const Vector vel, const Vector acc, const float mass, const float d)
    : pos(pos)
      , vel(vel)
      , acc(acc)
      , inverseMass(mass != 0.f ? 1.f / mass : 0.f)
      , damping_factor(d > 0 ? logf(d) : 0) {
}

Vector Particle::integrate(const float dt) {
    vel = expf(damping_factor * dt) * vel + dt * acc;
    pos += dt * vel;
    return pos;
}
