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
    Circle *circleAnchor;
    Circle *circleBungee1;
    Circle *circleBungee2;
    Blob *blob;

    explicit World();

    void applyForces(float dt);

    void updateVelocities(float dt);

    void updatePositions(float dt);

    void update(float dt);

    Blob *getBlob() const;

    void draw() const;
};
