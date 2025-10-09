#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	test_vector();
	dt = 1.0f / 60;
	test = Particle(Vector{100, 200, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.01, Vector{0, 0, 0});

	// Svp pas touche
	Rect *r;
	Circle *c;
	Actor *a;
}

//--------------------------------------------------------------
void ofApp::update() {
	dt = ofGetLastFrameTime();
	ParticleGravity grav;
	Registry.add(&test, &grav);
	Registry.updateForces(dt);
	Registry.clear();
	test.pos = test.integrate(dt);
	test.clearAccum();
}

//--------------------------------------------------------------
void ofApp::draw() {
	// Delta Time
	ofSetColor(255);
	ofDrawBitmapString("Delta Time: " + ofToString(dt, 3) + " ms", 10, 20); // Affiche avec 3 décimales, à (10, 20)
	// Particles
	ofSetColor(245, 0, 0);
	ofDrawCircle(test.pos.x, test.pos.y, 10);
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
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
