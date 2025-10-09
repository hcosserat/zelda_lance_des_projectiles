#include "ofApp.h"
#include "Maths/VectorTest.h"
#include "PhysicObjects/Particle.h"
#include "Forces/Gravity.h"

//--------------------------------------------------------------
void ofApp::setup() {
	test_vector();
	dt = 1.0f / 60;
	blob = Blob();
	blob.addCircle();
	world = World();
	// Svp pas touche
	Rect *r;
	Circle *c;
	Actor *a;
}

//--------------------------------------------------------------
void ofApp::update() {
	dt = ofGetLastFrameTime();
	world.WorldForces(dt);
}

//--------------------------------------------------------------
void ofApp::draw() {
	// Delta Time
	ofSetColor(255);
	ofDrawBitmapString("Delta Time: " + ofToString(dt, 3) + " ms", 10, 20); // Affiche avec 3 décimales, à (10, 20)
	// Particles
	ofSetColor(245, 0, 0);
	ofDrawCircle(test.pos.x, test.pos.y, 10);
	// Blob
	ofSetColor(0, 0, 245); // Blue
	ofDrawCircle(blob.center.centerParticle.pos.x, blob.center.centerParticle.pos.y, blob.center.radius);
	for (const auto &c: blob.circles) {
		ofSetColor(0, 245, 0); // Green
		ofDrawCircle(c.centerParticle.pos.x, c.centerParticle.pos.y, c.radius);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
		case 'a':
			blob.addCircle();
			break;
		case 'r':
			blob.removeCircle();
			break;
		default:
			break;
	}
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
