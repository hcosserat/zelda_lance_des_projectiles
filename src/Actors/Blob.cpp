#include "Blob.h"
#include "Rect.h"
#include "ofMain.h"
#include "../Collisions/ConstraintRegistry.h"

void Blob::addCircle() {
    const auto c = Circle(
        Particle(
            centerParticle.pos + Vector(ofRandom(-1, 1), ofRandom(-1, 0), 0).normalized() * (0.1 * centerRadius),
            Vector(0, 0, 0), Vector(0, 0, 0), 50), 15);
    circles.push_back(c);

    // Add constraints for the new circle if constraint registry is available
    if (constraintRegistry) {
        Circle *newCircle = &circles.back();

        // Create spring force between circle and center
        const float restLength = 4 * (centerRadius + newCircle->radius);
        constraintRegistry->addSpring(newCircle, this, restLength, 100.0f);

        // Create cable constraint for elasticity limits
        constraintRegistry->addCable(this, newCircle, 200);

        // Create spring forces between consecutive circles
        if (circles.size() > 1) {
            const float restLengthC = 4 * newCircle->radius;

            if (circles.size() == 2) {
                // First connection between two circles
                constraintRegistry->addSpring(&circles[0], newCircle, restLengthC, 100.0f);
            } else {
                // Connect to previous circle
                Circle *prevCircle = &circles[circles.size() - 2];
                constraintRegistry->addSpring(prevCircle, newCircle, restLengthC, 100.0f);

                // Update the connection from the last circle to the first to maintain the ring
                // Remove old connection from previous last to first
                constraintRegistry->removeConstraint(prevCircle, &circles[0]);
                // Add new connection from new last to first
                constraintRegistry->addSpring(newCircle, &circles[0], restLengthC, 100.0f);
            }
        }
    }
}

void Blob::removeCircle() {
    if (!circles.empty() && constraintRegistry) {
        Circle *lastCircle = &circles.back();

        // Remove all constraints involving the last circle
        constraintRegistry->removeConstraint(lastCircle, this); // Spring and cable to center

        if (circles.size() > 1) {
            // Remove connection to previous circle
            Circle *prevCircle = &circles[circles.size() - 2];
            constraintRegistry->removeConstraint(prevCircle, lastCircle);

            // If there are more than 2 circles, reconnect the ring
            if (circles.size() > 2) {
                // Remove connection from last to first
                constraintRegistry->removeConstraint(lastCircle, &circles[0]);
                // Add connection from previous to first to maintain ring
                const float restLengthC = 4 * prevCircle->radius;
                constraintRegistry->addSpring(prevCircle, &circles[0], restLengthC, 100.0f);
            }
        }
    }

    if (!circles.empty()) {
        circles.pop_back();
    }
}

void Blob::separateCircle() {
    if (!circles.empty()) {
        Circle *lastCircle = &circles.back();

        // Remove all constraints involving the last circle before separating
        if (constraintRegistry) {
            constraintRegistry->removeConstraint(lastCircle, this); // Spring and cable to center

            if (circles.size() > 1) {
                // Remove connection to previous circle
                Circle *prevCircle = &circles[circles.size() - 2];
                constraintRegistry->removeConstraint(prevCircle, lastCircle);

                // If there are more than 2 circles, reconnect the ring
                if (circles.size() > 2) {
                    // Remove connection from last to first
                    constraintRegistry->removeConstraint(lastCircle, &circles[0]);
                    // Add connection from previous to first to maintain ring
                    float restLengthC = 4 * prevCircle->radius;
                    constraintRegistry->addSpring(prevCircle, &circles[0], restLengthC, 100.0f);
                }
            }
        }

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

        // Add constraints for the newly fused circle
        if (constraintRegistry) {
            Circle *newCircle = &circles.back();

            // Create spring force between circle and center
            float restLength = 4 * (centerRadius + newCircle->radius);
            constraintRegistry->addSpring(newCircle, this, restLength, 100.0f);

            // Create cable constraint for elasticity limits
            constraintRegistry->addCable(this, newCircle, 200);

            // Create spring forces between consecutive circles
            if (circles.size() > 1) {
                float restLengthC = 4 * newCircle->radius;

                if (circles.size() == 2) {
                    // First connection between two circles
                    constraintRegistry->addSpring(&circles[0], newCircle, restLengthC, 100.0f);
                } else {
                    // Connect to previous circle
                    Circle *prevCircle = &circles[circles.size() - 2];
                    constraintRegistry->addSpring(prevCircle, newCircle, restLengthC, 100.0f);

                    // Update the connection from the last circle to the first to maintain the ring
                    // Remove old connection from previous last to first
                    constraintRegistry->removeConstraint(prevCircle, &circles[0]);
                    // Add new connection from new last to first
                    constraintRegistry->addSpring(newCircle, &circles[0], restLengthC, 100.0f);
                }
            }
        }
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
