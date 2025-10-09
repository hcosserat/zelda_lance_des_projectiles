#pragma once

#include "Particle.h"
#include "Actor.h"
#include "CollisionResult.h"
#include "Shape.h"

class Rect;

class Circle : public Actor {
public:
    Particle centerParticle;
    float radius;

    explicit Circle(const Particle &centerParticle, const float radius = 1) : Actor(centerParticle),
                                                                              radius(radius) {
    }

    Shape getShape() const override;

    CollisionResult collidesWithCircle(const Circle &other) const;

    CollisionResult collidesWithRect(const Rect &rect) const;

    CollisionResult collidesWith(const Actor &other) override;
};
