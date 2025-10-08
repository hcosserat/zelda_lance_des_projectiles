#pragma once

#include "ParticleForceGenerator.h"
#include "../Maths/Vector.h"
#include "../PhysicObjects/Particle.h"

class Particle;

// G�n�rateur de force de gravit�
class ParticleGravity : public ParticleForceGenerator {
public:
    Vector gravity;

    // Initialise la gravit�
    explicit ParticleGravity(const Vector &g) : gravity(g) {
    }

    // Modifie la gravit�
    void set(const Vector &g) { gravity = g; }
    const Vector &get() const { return gravity; }

    // Applique la force de gravit� sur la particule
    virtual void updateForce(Particle *particle, float duration);
};
