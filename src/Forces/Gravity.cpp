#include "Gravity.h"

ParticleGravity::ParticleGravity(const Vector g)
    : gravity(g) {
}

void ParticleGravity::updateforce(Particle *p, float /*dt*/) {
    if (!p) return;
    if (p->inverseMass == 0.0f) return; // masse infinie
    p->accumForce += gravity; // F += g
}
