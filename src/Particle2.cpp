#include "Particle2.h"

Particle2::Particle2(const Vector pos, const Vector vel, const Vector acc, const float mass, const float d)
    : pos(pos)
      , vel(vel)
      , acc(acc)
      , prevPos(Vector{NAN, NAN, NAN})
      , inverseMass(mass != 0.f ? 1.f / mass : 0.f)
      , damping_factor(d > 0 ? logf(d) : 0) {
}

Vector Particle2::integrate(const float dt) {
    vel = expf(damping_factor * dt) * vel + dt * acc;

    if (prevPos.isNAN()) {
        // Pour la première itération, on utilise Euler pour initialiser prevPos
        prevPos = pos;
        pos += dt * vel;
        return pos;
    }

    const auto currentPos = pos;
    pos += pos - prevPos + dt * dt * acc;
    prevPos = currentPos;
    return pos;
}
