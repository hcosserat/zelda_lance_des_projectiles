#include "AnchorSpringForce.h"

void AnchorSpringForce::updateforce(Particle &particule, float duration) {
    Vector force = particule.pos - anchorPoint; // Vector of the spring
    float magnitude = force.norm(); // Current length of the spring
    magnitude = fabs(magnitude - restLength); // Calculate the magnitude of the force
    magnitude *= springConstant; // Calculate the final magnitude of the force
    force.normalize(); // Normalize the vector
    force *= -magnitude; // Calculate the final force and apply it
    particule.addforce(force);
}
