#include "Gravity.h"

ParticleGravity::ParticleGravity(const Vector g)
	: gravity(g){}

void ParticleGravity::updateforce(Particle * p, float /*dt*/) {
	if (!p) return;
	if (p->inverseMass == 0.0f) return; // masse infinie
	p->acc += gravity * p->inverseMass; // a += g   (F/m = g)
	printf("x: %f\n", p->acc.x);
	printf("y: %f\n", p->acc.y);
	printf("z: %f\n", p->acc.z);
}
