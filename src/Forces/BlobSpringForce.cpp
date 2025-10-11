#include "BlobSpringForce.h"
#include <cmath>
#include <algorithm> // Pour std::fmin

BlobSpringForce::BlobSpringForce(Particle* other, float springConstant, float restLength, float elasticLimit)
    : other(other), springConstant(springConstant), restLength(restLength), elasticLimit(elasticLimit) {
}

void BlobSpringForce::updateforce(Particle* particule, float duration) {
    Vector force = particule->pos - other->pos;
    float magnitude = force.norm();

    float deltaL = magnitude - restLength;
    if (deltaL > 0) {
        // Tension: L > L_0. On plafonne l'étirement maximum à elasticLimit.
        // Si l'étirement dépasse la limite, deltaL est coupé, simulant une tension maximale ou un câble très tendu.
        deltaL = std::fmin(deltaL, elasticLimit);
    }
    float finalMagnitude = -springConstant * deltaL;
    if (magnitude > 0) {
        force.normalize();
        force *= finalMagnitude;
    }
    else {
        force = Vector{ 0, 0, 0 };
    }
    particule->addforce(force);
    other->addforce(force * -1);
}