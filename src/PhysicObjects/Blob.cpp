#include "Blob.h"

void Blob::addCircle() {
    Circle c = Circle(
        Particle(
            center.centerParticle.pos + Vector(ofRandom(-1, 1), ofRandom(-1, 1), 0).normalized() * (
                0.1f * center.radius),
            Vector(0, 0, 0), Vector(0, 0, 0), 50), 10);
    circles.push_back(c);
}

void Blob::removeCircle() {
    if (!circles.empty()) {
        circles.pop_back();
    }
}

CollisionResult Blob::_collidesWith(const Actor &other) {
    for (Circle &circle: circles) {
        if (const CollisionResult collision_result = circle._collidesWith(other); collision_result.collides) {
            return collision_result;
        }
    }

    return {false};
}
