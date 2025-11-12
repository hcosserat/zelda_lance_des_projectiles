#pragma once

#include "../Maths/Vector.h"

class RigidBody {
    Vector center; // centre géométrique
    Vector massCenter; // centre de masse

    Vector vel; // vélocité
    Vector acc; // accéleration

    float invMass; // inverse de la masse totale

    /* todo: une implémentation de sa rotation, vitesse angulaire et accéleration angulaire */

    RigidBody(const Vector &center, const Vector &massCenter, const Vector &vel,
              const Vector &acc, const float mass)
        : center(center)
          , massCenter(massCenter)
          , vel(vel)
          , acc(acc)
          , invMass(mass != 0.f ? 1.f / mass : 0.f) {
    }

    void updatePos(float dt);
};
