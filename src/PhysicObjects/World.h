#pragma once
#include <vector>
#include "Forces/ParticleForceRegistry.h"
#include "../Collisions/ConstraintRegistry.h"
#include "Actor.h"

class World {
public:
    std::vector<Actor *> actors;
    ParticleForceRegistry Registry;
    ConstraintRegistry constraintRegistry;

    explicit World();

    void WorldCollisions();

    void WorldForces(float dt);
};
