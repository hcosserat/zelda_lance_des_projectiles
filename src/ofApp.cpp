#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // test_vector();
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
    collisionResolver.resolve(world.actors, dt, &world.constraintRegistry);

    // Update display value in HUD
    int totalBlobParticles = 0;
    if (blob) {
        totalBlobParticles = blob->circles.size();
    }
    hud.setTargetValue(static_cast<float>(totalBlobParticles));
    hud.update(dt);
}

//--------------------------------------------------------------
void ofApp::draw() {
    // Delta Time
    ofSetColor(255);
    ofDrawBitmapString("Delta Time: " + ofToString(dt, 3) + " ms", 10, 20); // Affiche avec 3 décimales, à (10, 20)
    // HUD
    hud.draw(10, 40);
    // Particles
    world.draw();
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
