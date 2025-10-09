#pragma once
#include <Circle.h>
#include "../Forces/SpringForce.h"
#include "../Forces/ParticleForceRegistry.h"

class Blob {
public:
    Circle center;
    std::vector<Circle> circles;
    ParticleForceRegistry PFR;

    explicit Blob(
        const Circle &center = Circle(Particle(Vector(ofGetWidth() / 2, ofGetHeight() / 2, 0), Vector(0, 0, 0),
                                               Vector(0, 0, 0), 1), 50),
        const std::vector<Circle> &circles = {}) : center(center), circles(circles) {
    }

    void addCircle();

    void removeCircle();
};
