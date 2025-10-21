#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    test_vector();
    dt = 1.0f / 60;
    world = World();

    // Get the blob
    for (Actor *actor: world.actors) {
        if (actor->getShape() == BlobShape) {
            blob = dynamic_cast<Blob *>(actor);
            break;
        }
    }

    // Initialize Collision Resolver
    collisionResolver = CollisionResolver();
}

//--------------------------------------------------------------
void ofApp::update() {
    dt = ofGetLastFrameTime();
    world.WorldForces(dt);

    // Update display value in HUD
    int totalBlobParticles = 0;
    if (blob) {
        totalBlobParticles = blob->circles.size();
    }
    hud.setTargetValue(static_cast<float>(totalBlobParticles));
    hud.update(dt);

    collisionResolver.resolve(world.actors, dt, &world.constraintRegistry);
}

//--------------------------------------------------------------
void ofApp::draw() {
    // Delta Time
    ofSetColor(255);
    ofDrawBitmapString("Delta Time: " + ofToString(dt, 3) + " ms", 10, 20); // Affiche avec 3 décimales, à (10, 20)
    // HUD
    hud.draw(10, 40);
    // Particles
    ofSetColor(245, 0, 0);
    for (Actor *actor: world.actors) {
        switch (actor->getShape()) {
            case CircleShape: {
                ofDrawCircle(actor->centerParticle.pos.x, actor->centerParticle.pos.y,
                             dynamic_cast<Circle *>(actor)->radius);
                break;
            }
            case RectShape: {
                Rect *rect = dynamic_cast<Rect *>(actor);

                ofPushMatrix();

                // Translate to the center of the rectangle
                ofTranslate(actor->centerParticle.pos.x, actor->centerParticle.pos.y);

                // Calculate rotation angle from axisU
                float angle = atan2(rect->axisU.y, rect->axisU.x) * 180.0f / PI;
                ofRotateDeg(angle);

                // Draw the rectangle centered at origin
                ofSetColor(100, 100, 100);
                ofDrawRectangle(-rect->halfA, -rect->halfB, rect->halfA * 2, rect->halfB * 2);

                ofPopMatrix();
                break;
            }
            case BlobShape: {
                Blob *blob = dynamic_cast<Blob *>(actor);
                for (auto &c: blob->circles) {
                    ofSetColor(245, 0, 245); // Purple
                    ofDrawLine(blob->centerParticle.pos.x, blob->centerParticle.pos.y,
                               c.centerParticle.pos.x, c.centerParticle.pos.y);
                    if (&c != &blob->circles.back()) {
                        ofDrawLine(c.centerParticle.pos.x, c.centerParticle.pos.y,
                                   std::next(&c)->centerParticle.pos.x,
                                   std::next(&c)->centerParticle.pos.y);
                    } else {
                        ofDrawLine(c.centerParticle.pos.x, c.centerParticle.pos.y,
                                   blob->circles.front().centerParticle.pos.x,
                                   blob->circles.front().centerParticle.pos.y);
                    }
                }
                ofSetColor(0, 0, 245); // Blue
                ofDrawCircle(blob->centerParticle.pos.x, blob->centerParticle.pos.y, blob->center.radius);
                for (const auto &c: blob->circles) {
                    ofSetColor(0, 245, 0); // Green
                    ofDrawCircle(c.centerParticle.pos.x, c.centerParticle.pos.y, c.radius);
                }
                for (const auto &c: blob->separatedCircles) {
                    ofSetColor(100, 100, 100); // Grey
                    ofDrawCircle(c.centerParticle.pos.x, c.centerParticle.pos.y, c.radius);
                }
                break;
            }
            default: {
                std::cout << "Forme inconnue" << std::endl;
                break;
            }
        }
    }
    //ofDrawCircle(test.pos.x, test.pos.y, 10);
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
        case 'a':
            blob->addCircle();
            break;
        case 'r':
            blob->removeCircle();
            break;
        case 's':
            blob->separateCircle();
            break;
        case 'f':
            blob->fusionCircle();
            break;
        // Left arrow to move left
        case OF_KEY_LEFT:
            blob->centerParticle.vel.x = -100;
            break;
        // Right arrow to move right
        case OF_KEY_RIGHT:
            blob->centerParticle.vel.x = 100;
            break;
        // Space to jump
        case ' ':
            blob->centerParticle.vel.y = -250;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
        case OF_KEY_LEFT:
        case OF_KEY_RIGHT:
            blob->centerParticle.vel.x = 0;
            break;
        case ' ':
            blob->centerParticle.vel.y = 0;
            // TODO : No need to set to 0, just let gravity do its job but doesn't work well for now
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
