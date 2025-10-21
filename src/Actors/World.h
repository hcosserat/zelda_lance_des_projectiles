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

    void applyForces(float dt);

    void updateVelocities(float dt);

    void updatePositions(float dt);

    void draw() const;
};
