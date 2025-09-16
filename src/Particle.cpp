#include "Particle.h"

Particle::Particle(const Vector pos, const Vector vel, const Vector acc, const float mass)
    : pos(pos)
      , vel(vel)
      , acc(acc)
      , prevPos(pos)
      , mass(mass)
      , inverseMass(mass != 0.f ? 1.f / mass : 0.f)
      , firstVerletStep(true) {
}

Vector Particle::updateEuler(const float dt, const float dampingFactor) {
    prevPos = pos;
    const Vector effectiveAcc = acc - dampingFactor * vel;
    pos += vel * dt + effectiveAcc * (0.5f * dt * dt);
    vel += effectiveAcc * dt;
    return pos;
}

Vector Particle::updateVerlet(const float dt, const float dampingFactor) {
    if (firstVerletStep) {
        firstVerletStep = false;
        return updateEuler(dt, dampingFactor);
    }
    const Vector effectiveAcc = acc - dampingFactor * vel;
    const Vector newPos = pos + (pos - prevPos) + effectiveAcc * (dt * dt);
    vel = (newPos - prevPos) * (0.5f / dt);
    prevPos = pos;
    pos = newPos;
    return pos;
}
