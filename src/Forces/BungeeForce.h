#pragma once
#include "ParticleForceGenerator.h"

class BungeeForce : public ParticleForceGenerator {
    Particle *other;
    float springConstant; // stiffness constant K
    float restLength; // length at rest L0

public:
    BungeeForce(Particle *other, float springConstant, float restLength)
        : other(other), springConstant(springConstant), restLength(restLength) {
    }

    void updateforce(Particle *particule, float duration);
};
