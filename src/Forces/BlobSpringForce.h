#pragma once
#pragma once
#include "ParticleForceGenerator.h"
#include "Actors/Particle.h"
#include "Maths/Vector.h"

class BlobSpringForce : public ParticleForceGenerator {
public:
    Particle *other;
    float springConstant;
    float restLength;
    float elasticLimit; // Distance limit

    BlobSpringForce(Particle *other, float springConstant, float restLength, float elasticLimit);

    void updateforce(Particle *particule, float duration) override;
};
