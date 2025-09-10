#include "Particle.h"

Particle::Particle(const Vector pos, const Vector vel, const Vector acc, const float mass) {
    this->pos = pos;
    this->vel = vel;
    this->acc = acc;
    this->mass = mass;
    this->inverseMass = 1 / mass;
}
