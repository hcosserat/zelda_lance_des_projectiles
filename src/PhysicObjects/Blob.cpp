#include "Blob.h"

void Blob::addCircle() {
    Circle c = Circle(
        Particle(
            center.centerParticle.pos + Vector(ofRandom(-1, 1), ofRandom(-1, 1), 0).normalized() * (0.1 * center.radius),
            Vector(0, 0, 0), Vector(0, 0, 0), 50), 10);
    circles.push_back(c);
}

void Blob::removeCircle() {
    if (!circles.empty()) {
        circles.pop_back();
    }
}
