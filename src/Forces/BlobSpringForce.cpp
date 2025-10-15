#include "BlobSpringForce.h"
#include <cmath>
#include <algorithm> // Pour std::fmin

BlobSpringForce::BlobSpringForce(Particle *other, float springConstant, float restLength, float elasticLimit)
    : other(other), springConstant(springConstant), restLength(restLength), elasticLimit(elasticLimit) {
}

void BlobSpringForce::updateforce(Particle *particule, float duration) {
    Vector force = particule->pos - other->pos;
    float magnitude = force.norm();

    float deltaL = magnitude - restLength;
    if (magnitude <= 0.001f) {
        return;
    }
    float finalMagnitude;
    if (deltaL > 0) {
        // Tension: L > L_0. On plafonne l'�tirement maximum � elasticLimit.
        // Si l'�tirement d�passe la limite, deltaL est coup�, simulant une tension maximale ou un c�ble tr�s tendu.
        deltaL = std::fmin(deltaL, elasticLimit);
        finalMagnitude = springConstant * deltaL;
        force.normalize();
        force *= -finalMagnitude;
    } else {
        finalMagnitude = springConstant * std::fabs(deltaL); // Compression: L < L_0
        force.normalize();
        force *= finalMagnitude;
    }
    particule->addforce(force);
    other->addforce(force * -1);
}
