#pragma once
#include <vector>
#include "Forces/ParticleForceRegistry.h"
#include "../Collisions/ConstraintRegistry.h"
#include "../Collisions/CollisionResolver.h"
#include "Actor.h"

class Circle;
class Blob;

class World {
public:
    std::vector<Actor *> actors;
    ParticleForceRegistry particleForceRegistry;
    ConstraintRegistry constraintRegistry;
    CollisionResolver collisionResolver;
    Blob *blob;

    explicit World();

    void applyForces(float dt);

    void updateVelocities(float dt) const;

    void updatePositions(float dt) const;

    void update(float dt);

    Blob *getBlob() const;

    void draw() const;
};
