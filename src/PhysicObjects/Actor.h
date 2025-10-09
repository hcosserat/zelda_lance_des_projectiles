#pragma once

#include "Particle.h"
#include "CollisionResult.h"
#include "Shape.h"

class Actor {
public:
    Particle centerParticle;

    explicit Actor(const Particle &centerParticle)
            : centerParticle(centerParticle) {}

    virtual ~Actor() = default;

    virtual Shape getShape() const = 0;

    virtual CollisionResult collidesWith(const Actor &other) = 0;
};

