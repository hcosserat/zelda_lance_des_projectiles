#pragma once
#include "ParticleForceGenerator.h"
#include "../Maths/Vector.h"
#include "../Actors/Particle.h"

class Particle;

class FrictionForce : public ParticleForceGenerator {
public:
    float coef_friction_sta;
    float coef_friction_cin;
    Vector vec_norm;

    // Initialise la friction
    explicit FrictionForce(
        float coef_sta = 10.0,
        float coef_cin = 1.0,
        Vector n = Vector{0, 1, 0}
    );

    // Applique la force de friction sur la particule
    virtual void updateforce(Particle *particle, float duration);
};
