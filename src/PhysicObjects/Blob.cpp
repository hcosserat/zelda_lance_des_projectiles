#include "Blob.h"

void Blob::addCircle() {
    Circle c = Circle(
        Particle(
            center.centerParticle.pos + Vector(ofRandom(-1, 1), ofRandom(-1, 1), 0).normalized() * (
                0.1 * center.radius),
            Vector(0, 0, 0), Vector(0, 0, 0), 80), 15);
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
    for (Circle &circle: circles) {
        if (const CollisionResult collision_result = circle._collidesWith(other); collision_result.collides) {
            return collision_result;
        }
    }

    return {false};
}
