#pragma once

#include "ParticleForceGenerator.h"
#include "../Maths/Vector.h"
#include "../Actors/Particle.h"

#define GRAVITY Vector { 0, 980.0, 0 }

class Particle;

// G�n�rateur de force de gravit�
class ParticleGravity : public ParticleForceGenerator {
public:
    Vector gravity;

    // Initialise la gravit�
    explicit ParticleGravity(
        Vector g = GRAVITY
    );

    // Modifie la gravit�
    void set(const Vector &g) { gravity = g; }
    const Vector &get() const { return gravity; }

    // Applique la force de gravit� sur la particule
    virtual void updateforce(Particle *particle, float duration);
};
