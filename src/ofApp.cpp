#include "ofApp.h"
#include "Blob.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Unit tests
    test_vector();

    // Initialization of app attributes
    dt = 1.0f / 60;
    world = World();
}

//--------------------------------------------------------------
void ofApp::update() {
    dt = ofGetLastFrameTime();
    world.update(dt);

    // Update display value in HUD
    if (world.blob) {
        hud.setTargetValue(world.blob->circles.size());
    } else {
        hud.setTargetValue(0);
    }
    hud.update(dt);
}


//--------------------------------------------------------------
void ofApp::draw() {
    // Draw world and all actors
    world.draw();

    // Draw UI elements
    ofSetColor(255);
    ofDrawBitmapString("Delta Time: " + ofToString(dt, 3) + " ms", 10, 20);
    hud.draw(10, 40);
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(const int key) {
    if (world.blob) {
        world.blob->handleKeyPressed(key);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(const int key) {
    if (world.blob) {
        world.blob->handleKeyReleased(key);
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
