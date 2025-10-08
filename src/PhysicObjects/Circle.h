#pragma once

#include "Particle.h"
#include "Rect.h"
#include "CollisionResult.h"

class Circle {
public:
    Particle centerParticle;
    float radius;

    explicit Circle(const Particle &centerParticle, const float radius = 1) : centerParticle(centerParticle),
                                                                              radius(radius) {
    }

    CollisionResult collidesWith(const Circle &other) const;

    CollisionResult collidesWith(const Rect &rect) const;
};
