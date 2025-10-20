#include "SpringForce.h"

SpringForce::SpringForce(Particle *other, float springConstant, float restLength)
    : other(other), springConstant(springConstant), restLength(restLength) {
}

void SpringForce::updateforce(Particle *particule, float duration) {
	Vector l = particule->pos - other->pos; // Vector of the spring
    float magnitude = l.norm() - restLength; // Calculate the magnitude of the force
    magnitude *= springConstant; // Calculate the final magnitude of the force
	Vector force = l.normalized(); // Calculate the final force direction
    force *= -magnitude * springConstant; // Calculate the final force and apply it
    particule->addforce(force);
    other->addforce(-force);
}
