#pragma once
#include <vector>
#include "Forces/ParticleForceRegistry.h"
#include "../Collisions/ConstraintRegistry.h"
#include "Actor.h"

class Circle;

class World {
public:
    std::vector<Actor *> actors;
    ParticleForceRegistry Registry;
    ConstraintRegistry constraintRegistry;
    Circle* circleAnchor;
    Circle* circleBungee1;
	Circle* circleBungee2;

    explicit World();

    void applyForces(float dt);

    void updateVelocities(float dt);

    void updatePositions(float dt);

    void draw() const;
};
