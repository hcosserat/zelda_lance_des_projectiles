#pragma once

#include "../PhysicObjects/Particle.h"

class ParticleForceGenerator {
public:
    virtual void updateforce(Particle particule, float duration) = 0;
};
