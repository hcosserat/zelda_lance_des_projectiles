#include "ofApp.h"
#include "Maths/VectorTest.h"
#include "PhysicObjects/Particle.h"
#include "Forces/Gravity.h"

//--------------------------------------------------------------
void ofApp::setup() {
	test_vector();
	dt = 1.0f / 60;
	world = World();
	// Svp pas touche
	Rect *r;
	Circle *c;
	Actor *a;

	// Get the blob
	for (Actor* actor : world.actors) {
		if (actor->getShape() == BlobShape) {
			blob = dynamic_cast<Blob*>(actor);
			break;
		}
	}
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
	for (Actor * actor : world.actors) {
		switch (actor->getShape()) {
		case CircleShape: {
			ofDrawCircle(actor->centerParticle.pos.x, actor->centerParticle.pos.y, dynamic_cast<Circle *>(actor)->radius);
			break;
		}
		case RectShape: {
			std::cout << "C'est un rectangle" << std::endl;
			break;
		}
		case BlobShape: {
			Blob* blob = dynamic_cast<Blob*>(actor);
			ofSetColor(0, 0, 245); // Blue
			ofDrawCircle(blob->centerParticle.pos.x, blob->centerParticle.pos.y, blob->center.radius);
			for (const auto& c : blob->circles) {
				ofSetColor(0, 245, 0); // Green
				ofDrawCircle(c.centerParticle.pos.x, c.centerParticle.pos.y, c.radius);
			}
			break;
		}
		default: {
			std::cout << "Forme inconnu" << std::endl;
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
