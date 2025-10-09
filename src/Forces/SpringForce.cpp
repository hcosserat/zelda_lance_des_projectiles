#include "SpringForce.h"

SpringForce::SpringForce(Particle *other, float springConstant, float restLength)
	: other(other), springConstant(springConstant), restLength(restLength)
{
}

void SpringForce::updateforce(Particle* particule, float duration) {
    Vector force = particule->pos - other->pos; // Vector of the spring
    float magnitude = force.norm(); // Current length of the spring
    magnitude = fabs(magnitude - restLength); // Calculate the magnitude of the force
    magnitude *= springConstant; // Calculate the final magnitude of the force
    force.normalize(); // Normalize the vector
    force *= -magnitude; // Calculate the final force and apply it
    particule->addforce(force);
	other->addforce(force * -1);
}
