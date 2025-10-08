#include "Gravity.h"

void ParticleGravity::updateForce(Particle *p, float /*dt*/) {
    if (!p) return;
    if (p->inverseMass == 0.0f) return; // masse infinie
    p->acc += gravity; // a += g   (F/m = g)
}
