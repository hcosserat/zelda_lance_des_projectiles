#include "Blob.h"

void Blob::addCircle() {
    Circle c = Circle(
        Particle(
            centerParticle.pos + Vector(ofRandom(-1, 1), ofRandom(-1, 1), 0).normalized() * (0.1 * centerRadius),
            Vector(0, 0, 0), Vector(0, 0, 0), 50), 15);
    circles.push_back(c);
}

void Blob::removeCircle() {
    if (!circles.empty()) {
        circles.pop_back();
    }
}

void Blob::separateCircle() {
    if (!circles.empty()) {
        Circle c = circles.back();
        circles.pop_back();
        separatedCircles.push_back(c);
    }
}

void Blob::fusionCircle() {
    if (!separatedCircles.empty()) {
        Circle c = separatedCircles.back();
        separatedCircles.pop_back();
        circles.push_back(c);
    }
}

CollisionResult Blob::_collidesWith(const Actor &other) {
    // this should never be called, check collisions with each circle instead
    return {false};
}
