#include "Blob.h"

void Blob::addCircle() {
    Circle c = Circle(
        Particle(
            center.centerParticle.pos + Vector(ofRandom(-1, 1), ofRandom(-1, 1), 0).normalized() * (2 * center.radius),
            Vector(0, 0, 0), Vector(0, 0, 0), 1), 10);
    circles.push_back(c);
    SpringForce *sf = new SpringForce(&circles.back().centerParticle, 50, center.radius + c.radius);
    PFR.add(&center.centerParticle, sf);
}

void Blob::removeCircle() {
    if (!circles.empty()) {
        PFR.remove(&center.centerParticle, nullptr);
        // Note: This does not delete the SpringForce, leading to a potential memory leak.
        circles.pop_back();
    }
}
