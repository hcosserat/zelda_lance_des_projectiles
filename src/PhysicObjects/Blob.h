#pragma once
#include <Circle.h>
#include "../Forces/SpringForce.h"
#include "../Forces/ParticleForceRegistry.h"

class Blob : public Actor {
public:
    Circle center;
    std::vector<Circle> circles;

    explicit Blob(
        const Circle& center = Circle(Particle(Vector(400, 500, 0), Vector(0, 0, 0),
            Vector(0, 0, 0), 100), 18),
        const std::vector<Circle>& circles = {}) : Actor(center), center(center), circles(circles) {
    }

    Shape getShape() const override {
        return BlobShape;
	}

    CollisionResult _collidesWith(const Actor& other) override;

    void addCircle();
    void removeCircle();
};
