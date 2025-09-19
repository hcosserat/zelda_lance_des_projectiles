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

	// Ground
	ofSetColor(150, 75, 0); 
	ofSetLineWidth(4);
	ofDrawLine(0, ofGetHeight() - 50, ofGetWidth(), ofGetHeight() - 50);

	// Explanation
	ofSetColor(255);
	ofDrawBitmapString("Tir balistique : touches = projectile", 10, 40);
	ofDrawBitmapString("'a' = Balle", 10, 60);
	ofDrawBitmapString("'z' = Boulet", 10, 80);
	ofDrawBitmapString("'e' = Laser", 10, 100);
	ofDrawBitmapString("'r' = Boule de feu", 10, 120);

	// Cannon
	Vector cannonPos(100, ofGetHeight() - 120, 0);
	ofSetColor(100, 100, 100);
	ofDrawRectangle(cannonPos.x - 45, cannonPos.y - 70, 90, 70);
	ofSetColor(80, 80, 80);
	ofDrawRectangle(cannonPos.x + 40, cannonPos.y - 50, 80, 30);
	ofSetColor(90, 90, 90);
	ofDrawRectangle(cannonPos.x - 15, cannonPos.y - 65, 55, 25);
	ofSetColor(50, 50, 50);
	ofDrawCircle(cannonPos.x + 120, cannonPos.y - 35, 18);
	ofSetColor(70, 70, 70);
	ofSetLineWidth(8);
	ofDrawLine(cannonPos.x - 10, cannonPos.y, cannonPos.x - 40, cannonPos.y + 60);
	ofDrawLine(cannonPos.x + 10, cannonPos.y, cannonPos.x + 40, cannonPos.y + 60);

	// Delta Time
	ofSetColor(255); 
	ofDrawBitmapString("Delta Time: " + ofToString(dt, 3) + " ms", 10, 20); // Affiche avec 3 décimales, à (10, 20)

	// Particles
	for (auto & p : particles) {
		p.integrate(dt);
		switch (p.type)
		{
		case Particle::projectileType::Balle:
			// Draw trajectory
			if (p.showTrajectory) {
				DrawTrajectory(spawnPos, p.velStart, Vector(0, 9.8, 0), p.type);
			}
			// Draw particle
			ofSetColor(245, 211, 101);
			if (!(p.pos.y >= ofGetHeight() - 50 || p.pos.x > ofGetWidth() || p.pos.x < 0 || p.pos.y < 0)) {	// If not out of bounds
				ofDrawCircle(p.pos.x, p.pos.y, 10);
			}
			else {
				p.showTrajectory = false;
			}
			break;
		case Particle::projectileType::Boulet:
			// Draw trajectory
			if (p.showTrajectory) {
				DrawTrajectory(spawnPos, p.velStart, Vector(0, 9.8, 0), p.type);
			}
			// Draw particle
			ofSetColor(205, 205, 205);
			if (!(p.pos.y >= ofGetHeight() - 50 || p.pos.x > ofGetWidth() || p.pos.x < 0 || p.pos.y < 0)) {	// If not out of bounds
				ofDrawCircle(p.pos.x, p.pos.y, 20);
			}
			else {
				p.showTrajectory = false;
			}
			break;
		case Particle::projectileType::Laser:
			// Draw trajectory
			if (p.showTrajectory) {
				DrawTrajectory(spawnPos, p.velStart, Vector(0, 9.8, 0), p.type);
			}
			// Draw particle
			ofSetColor(0, 255, 0);
			if (!(p.pos.y >= ofGetHeight() - 50 || p.pos.x > ofGetWidth() || p.pos.x < 0 || p.pos.y < 0)) {	// If not out of bounds
				ofDrawRectangle(p.pos.x, p.pos.y, 30, 5);
			}
			else {
				p.showTrajectory = false;
			}
			break;
		case Particle::projectileType::BouleDeFeu:
			// Draw trajectory
			if (p.showTrajectory) {
				DrawTrajectory(spawnPos, p.velStart, Vector(0, 9.8, 0), p.type);
			}
			// Draw particle
			ofSetColor(255, 165, 0);
			if (!(p.pos.y >= ofGetHeight() - 50 || p.pos.x > ofGetWidth() || p.pos.x < 0 || p.pos.y < 0)) {	// If not out of bounds
				ofDrawCircle(p.pos.x, p.pos.y, 15);
			}
			else {
				p.showTrajectory = false;
			}
			break;
		default:
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Direction from spawn position to mouse position
	Vector mousePos(ofGetMouseX(), ofGetMouseY(), 0);
	Vector dir = mousePos - spawnPos;
	dir.normalize();

	switch (key)
	{
	default:
		break;

	case 'a': // Balle
		SpawnParticle(dir * 300, Vector(0, 9.8, 0), 1, Particle::projectileType::Balle);
		break;

	case 'z': // Boulet
		SpawnParticle(dir * 200, Vector(0, 9.8, 0), 10, Particle::projectileType::Boulet);
		break;

	case 'e': // Laser
		SpawnParticle(dir * 500, Vector(0, 9.8, 0), 0.1, Particle::projectileType::Laser);
		break;

	case 'r': // Boule de feu
		SpawnParticle(dir * 250, Vector(0, 9.8, 0), 5, Particle::projectileType::BouleDeFeu);
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

void ofApp::SpawnParticle(Vector v, Vector a, float m, Particle::projectileType t) {

	Particle Particle(spawnPos, v, a, m, 1, t);
	particles.push_back(Particle);

}

void ofApp::DrawTrajectory(Vector startPos, Vector v, Vector a, Particle::projectileType t) {

	float timeStep = 0.016f; // ~60 FPS
	int maxSteps = 300; // Limit to avoid infinite loops

	switch (t)
	{
	case Particle::Balle:
		ofSetColor(245, 211, 101, 100);
		break;
	case Particle::Boulet:
		ofSetColor(205, 205, 205, 100);
		break;
	case Particle::Laser:
		ofSetColor(0, 255, 0, 100);
		break;
	case Particle::BouleDeFeu:
		ofSetColor(255, 165, 0, 100);
		break;
	}

	Vector pos = startPos;
	Vector vel = v;
	Vector prevPos = pos;

	for (int i = 0; i < maxSteps; ++i) {
		vel += a * timeStep;
		pos += vel * timeStep;
		if (pos.y >= ofGetHeight() - 50) { // Ground collision
			break;
		}
		ofDrawLine(prevPos.x, prevPos.y, pos.x, pos.y);
		prevPos = pos;
	}

}
