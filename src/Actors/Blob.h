#pragma once
#include <Circle.h>
#include "../Forces/SpringForce.h"
#include "../Forces/ParticleForceRegistry.h"

class Blob : public Actor {
public:
    float centerRadius;
    std::vector<Circle> circles;
    std::vector<Circle> separatedCircles;

    explicit Blob(
        const Particle &centerParticle = Particle(Vector(400, 500, 0), Vector(0, 0, 0),
                                                   Vector(0, 0, 0), 100, Vector(0, 0, 0)),
        const float centerRadius = 18,
        const std::vector<Circle> &circles = {})
        : Actor(centerParticle), centerRadius(centerRadius), circles(circles) {
    }

    Shape getShape() const override {
        return BlobShape;
    }

    Circle getCenter() const {
        return Circle(centerParticle, centerRadius);
    }

    CollisionResult _collidesWith(const Actor &other) override;

    void addCircle();

    void removeCircle();

    void separateCircle();

    void fusionCircle();
};
