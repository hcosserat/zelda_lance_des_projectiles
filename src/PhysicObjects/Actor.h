#pragma once

#include "Particle.h"
#include "CollisionResult.h"
#include "Shape.h"

class Actor {
public:
    Particle centerParticle;

    explicit Actor(const Particle &centerParticle)
        : centerParticle(centerParticle) {
    }

    virtual ~Actor() = default;

    virtual Shape getShape() const = 0;

    virtual CollisionResult _collidesWith(const Actor &other) = 0;

    CollisionResult collidesWith(const Actor &other, const float frame_length);
};
