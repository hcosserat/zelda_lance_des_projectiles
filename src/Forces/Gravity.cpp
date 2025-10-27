#include "Gravity.h"

ParticleGravity::ParticleGravity(const Vector g)
    : gravity(g) {
}

void ParticleGravity::updateforce(Particle *p, float /*dt*/) {
    if (!p) return;
    p->addforce(gravity * (1 / p->inverseMass));
}
