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
	Vector spawnPos(100, ofGetHeight() - 120, 0);
	ofSetColor(100, 100, 100);
	ofDrawRectangle(spawnPos.x - 45, spawnPos.y - 70, 90, 70);
	ofSetColor(80, 80, 80);
	ofDrawRectangle(spawnPos.x + 40, spawnPos.y - 50, 80, 30);
	ofSetColor(90, 90, 90);
	ofDrawRectangle(spawnPos.x - 15, spawnPos.y - 65, 55, 25);
	ofSetColor(50, 50, 50);
	ofDrawCircle(spawnPos.x + 120, spawnPos.y - 35, 18);
	ofSetColor(70, 70, 70);
	ofSetLineWidth(8);
	ofDrawLine(spawnPos.x - 10, spawnPos.y, spawnPos.x - 40, spawnPos.y + 60);
	ofDrawLine(spawnPos.x + 10, spawnPos.y, spawnPos.x + 40, spawnPos.y + 60);

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
			DrawTrajectory(Vector(220, ofGetHeight() - 155, 0), Vector(300, 0, 0), Vector(0, 9.8, 0), p.type);
			// Draw particle
			ofSetColor(245, 211, 101);
			ofDrawCircle(p.pos.x, p.pos.y, 10);
			break;
		case Particle::projectileType::Boulet:
			// Draw trajectory
			DrawTrajectory(Vector(220, ofGetHeight() - 155, 0), Vector(200, 0, 0), Vector(0, 9.8, 0), p.type);
			// Draw particle
			ofSetColor(205, 205, 205);
			ofDrawCircle(p.pos.x, p.pos.y, 20);
			break;
		case Particle::projectileType::Laser:
			// Draw trajectory
			DrawTrajectory(Vector(220, ofGetHeight() - 155, 0), Vector(500, 0, 0), Vector(0, 9.8, 0), p.type);
			// Draw particle
			ofSetColor(0, 255, 0);
			ofDrawRectangle(p.pos.x, p.pos.y, 30, 5);
			break;
		case Particle::projectileType::BouleDeFeu:
			// Draw trajectory
			DrawTrajectory(Vector(220, ofGetHeight() - 155, 0), Vector(250, 0, 0), Vector(0, 9.8, 0), p.type);
			// Draw particle
			ofSetColor(255, 165, 0);
			ofDrawCircle(p.pos.x, p.pos.y, 15);
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

	switch (key)
	{
	default:
		break;

	case 'a': // Balle
		SpawnParticle(Vector(300, 0, 0), Vector(0, 9.8, 0), 1, Particle::projectileType::Balle);
		break;

	case 'z': // Boulet
		SpawnParticle(Vector(200, 0, 0), Vector(0, 9.8, 0), 10, Particle::projectileType::Boulet);
		break;

	case 'e': // Laser
		SpawnParticle(Vector(500, 0, 0), Vector(0, 9.8, 0), 0.1, Particle::projectileType::Laser);
		break;

	case 'r': // Boule de feu
		SpawnParticle(Vector(250, 0, 0), Vector(0, 9.8, 0), 5, Particle::projectileType::BouleDeFeu);
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

	Particle Particle(Vector(220, ofGetHeight() - 155, 0), v, a, m, 1, t);

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
