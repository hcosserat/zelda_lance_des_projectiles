#pragma once

#include "Particle.h"
#include "CollisionResult.h"
#include "Shape.h"

class ConstraintRegistry;

class Actor {
public:
    Particle centerParticle;

    explicit Actor(const Particle &centerParticle)
        : centerParticle(centerParticle) {
    }

    virtual ~Actor() = default;

    virtual Shape getShape() const = 0;

    virtual CollisionResult _collidesWith(const Actor &other) = 0;

    CollisionResult collidesWith(const Actor &other, float frame_length);

    CollisionResult checkConstraint(const Actor &other, ConstraintRegistry *registry) const;
};
