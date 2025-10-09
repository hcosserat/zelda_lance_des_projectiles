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
    explicit ParticleGravity(
        Vector g = Vector{0, 9.8, 0}
    );

    // Modifie la gravit�
    void set(const Vector &g) { gravity = g; }
    const Vector &get() const { return gravity; }

    // Applique la force de gravit� sur la particule
    virtual void updateforce(Particle *particle, float duration);
};
