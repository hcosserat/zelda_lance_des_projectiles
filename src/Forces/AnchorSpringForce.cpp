#include "AnchorSpringForce.h"

AnchorSpringForce::AnchorSpringForce(const Vector &anchorPoint, float springConstant, float restLength)
    : anchorPoint(anchorPoint), springConstant(springConstant), restLength(restLength) {
}

void AnchorSpringForce::updateforce(Particle *particule, float duration) {
    Vector l = particule->pos - anchorPoint; // Vector of the spring
    float magnitude = l.norm() - restLength; // Current magnitude of the spring
    magnitude *= springConstant; // Calculate the final magnitude of the force
    Vector force = l.normalized(); // Calculate the final force direction
    force *= -magnitude; // Calculate the final force and apply it
    particule->addforce(force);
}
