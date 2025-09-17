#include "ofApp.h"
#include "VectorTest.h"

//--------------------------------------------------------------
void ofApp::setup() {
	test_vector();
	float dt = 1/60;
}

//--------------------------------------------------------------
void ofApp::update() {
	dt = ofGetLastFrameTime();
}

//--------------------------------------------------------------
void ofApp::draw() {
	std::cout << "Delay :" << dt;
	std::cout << "ms" << std::endl;

	ofSetColor(255);
	for (auto& p : particles) {
		//p.updateVerlet(0.016f, 0.1f);
		ofDrawCircle(p.pos.x, p.pos.y, 5);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key)
	{
	default:
		break;

	case 'a':
		SpawnParticle(Vector(100, 100, 0), Vector(0, 9.8, 0), 1);
		break;

	case 'z':
		SpawnParticle(Vector(200, 100, 0), Vector(0, 9.8, 0), 10);
		break;

	case 'e':
		SpawnParticle(Vector(300, 100, 0), Vector(0, 9.8, 0), 100);
		break;

	case 'r':
		SpawnParticle(Vector(400, 100, 0), Vector(0, 9.8, 0), 1000);
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

void ofApp::SpawnParticle(Vector v, Vector a, float m) {

	Particle Particle(Vector(100, 100, 100), v, a, m);

	particles.push_back(Particle);

}