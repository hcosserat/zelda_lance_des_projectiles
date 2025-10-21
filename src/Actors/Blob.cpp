#include "Blob.h"
#include "Rect.h"

void Blob::addCircle() {
    Circle c = Circle(
        Particle(
            centerParticle.pos + Vector(ofRandom(-1, 1), ofRandom(-1, 0), 0).normalized() * (0.1 * centerRadius),
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

CollisionResult Blob::collidesWithCircle(const Circle &other) const {
    Circle blobCenter = getCenter();
    return blobCenter.collidesWithCircle(other);
}

CollisionResult Blob::collidesWithRect(const Rect &rect) const {
    Circle blobCenter = getCenter();
    return blobCenter.collidesWithRect(rect);
}

CollisionResult Blob::_collidesWith(const Actor &other) {
    // Handle collisions using the center circle only
    switch (other.getShape()) {
        case CircleShape:
            return collidesWithCircle(dynamic_cast<const Circle &>(other));
        case RectShape:
            return collidesWithRect(dynamic_cast<const Rect &>(other));
        case BlobShape: {
            const Blob &otherBlob = dynamic_cast<const Blob &>(other);
            Circle blobCenter = getCenter();
            Circle otherBlobCenter = otherBlob.getCenter();
            return blobCenter.collidesWithCircle(otherBlobCenter);
        }
        default:
            return {false};
    }
}
