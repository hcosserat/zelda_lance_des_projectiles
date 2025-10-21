#include "BungeeForce.h"

void BungeeForce::updateforce(Particle *particule, float duration) {
    Vector l = particule->pos - other->pos; // Vector of the spring
    float magnitude = l.norm(); // Current length of the spring
    if (magnitude <= restLength) return; // No force if the bungee is not stretched
    magnitude = magnitude - restLength; // Calculate the magnitude of the force
    magnitude *= springConstant; // Calculate the final magnitude of the force
	Vector force = l.normalized(); // Calculate the final force direction
    force *= -magnitude; // Calculate the final force and apply it
    particule->addforce(force);
    other->addforce(-force);
}
