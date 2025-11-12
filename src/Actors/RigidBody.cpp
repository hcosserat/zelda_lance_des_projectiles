#include "RigidBody.h"

void RigidBody::updatePos(const float dt) {
    if (invMass == 0.0f) {
        // Masse infinie, pas de mouvement
        return;
    }

    // Mise à jour de la vélocité
    vel += acc * dt;

    // Mise à jour de la position
    center += vel * dt;
}
