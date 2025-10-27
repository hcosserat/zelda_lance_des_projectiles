#include "Blob.h"
#include "Rect.h"
#include "ofMain.h"

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

void Blob::handleKeyPressed(int key) {
    switch (key) {
        // Blob manipulation controls
        case 'a':
            addCircle();
            break;
        case 'r':
            removeCircle();
            break;
        case 's':
            separateCircle();
            break;
        case 'f':
            fusionCircle();
            break;

        // Movement controls
        case OF_KEY_LEFT:
            centerParticle.vel.x = -100;
            break;
        case OF_KEY_RIGHT:
            centerParticle.vel.x = 100;
            break;
        case ' ': // Jump
            if (!isJumping) {
                centerParticle.vel.y = -250;
                isJumping = true;
                break;
            }
        default:
            break;
    }
}

void Blob::handleKeyReleased(int key) {
    switch (key) {
        case OF_KEY_LEFT:
        case OF_KEY_RIGHT:
            centerParticle.vel.x = 0;
            break;
        case ' ': {
            isJumping = false;
            break;
        }
        default:
            break;
    }
}
