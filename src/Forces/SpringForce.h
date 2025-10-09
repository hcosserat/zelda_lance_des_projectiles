#pragma once
#include "ParticleForceGenerator.h"

class SpringForce : public ParticleForceGenerator {
    Particle *other;
    float springConstant; // stiffness constant K
    float restLength; // length at rest L0

public:
    SpringForce(Particle *other, float springConstant, float restLength);

    void updateforce(Particle *particule, float duration);
};
