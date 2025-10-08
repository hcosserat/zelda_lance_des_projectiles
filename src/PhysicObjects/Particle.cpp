#include "Particle.h"

Particle::Particle(const Vector pos, const Vector vel, const Vector acc, const float mass, const Vector accumForce)
	: pos(pos)
	  , vel(vel)
	  , acc(acc)
	  , accumForce(accumForce)
	  , inverseMass(mass != 0.f ? 1.f / mass : 0.f) {
}

Vector Particle::integrate(const float dt) {
	acc = accumForce * inverseMass;
	vel += dt * acc;
	pos += dt * vel;
	return pos;
}

void Particle::addforce(const Vector &force) {
	accumForce += force;
}

void Particle::clearAccum() {
	accumForce = Vector{0, 0, 0};
}
