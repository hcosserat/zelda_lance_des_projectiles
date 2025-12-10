#include "ofApp.h"
#include "Actors/BoxShape.h"
#include "Actors/PlaneShape.h"

#include "Maths/VectorTest.h"
#include "Maths/Matrix3Test.h"
#include "Maths/Matrix4Test.h"
#include "Maths/QuaternionTest.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Lancer les tests unitaires
    test_vector();
    test_matrix3();
    test_matrix4();
    test_quaternion();

    // Configurer la caméra pour observer la zone de collisions
    cam.setPosition(0, 25, 50); // Centré sur X, élevé, reculé sur Z
    cam.lookAt({0, 10, 0}); // Regarder à la hauteur de la collision

    // Rendu
    ofEnableDepthTest();
    ofSetBackgroundColor(30, 30, 30);
}

//--------------------------------------------------------------
void ofApp::update() {
    dt = ofGetLastFrameTime();
    world.update(dt);
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();
    world.draw();
    cam.end();

    drawHUD();
}

//--------------------------------------------------------------
void ofApp::drawHUD() const {
    ofDisableDepthTest();

    ofSetColor(255);
    ofDrawBitmapString("Delta Time: " + ofToString(dt * 1000.0f, 2) + " ms", 10, 20);
    ofDrawBitmapString("FPS: " + ofToString(1.0f / dt, 1), 10, 40);
    ofDrawBitmapStringHighlight(
        "Espace: Lancer deux boites", 40, 70);
	ofDrawBitmapStringHighlight("C: Lancer une boite depuis la caméra", 40, 90);
    ofDrawBitmapString("D: debug", 40, 110);

    ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(const int key) {

	switch (key) {

	case ' ': {
		float now = ofGetElapsedTimef();
		if (now - lastThrowTime >= throwCooldown) {
			throwProjectile();
			lastThrowTime = now;
		}
	} break;

	case 'd':
	case 'D':
		world.toggleDebugDraw();
		break;

	case 'c':
	case 'C': {
		float now = ofGetElapsedTimef();
		if (now - lastThrowTime >= throwCooldown) {
			throwForwardProjectile();
			lastThrowTime = now;
		}
	} break;


	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::throwProjectile() {
    // Masse volumique (kg/m³)
    const float volumetricMass = 2500.0f; // similaire au béton

    // Hauteur de lancement
    const float launchHeight = 15.0f;

    // Distance sur l'axe X
    const float launchDistance = 30.0f;

    // Dimensions de base avec légère variation aléatoire
    float randomScale1 = ofRandom(0.8f, 1.2f);
    float randomScale2 = ofRandom(0.8f, 1.2f);
    Vector dimensions1(3.0f * randomScale1, 3.0f * randomScale1, 3.0f * randomScale1);
    Vector dimensions2(3.0f * randomScale2, 3.0f * randomScale2, 3.0f * randomScale2);

    // Calcul des masses à partir du volume
    float volume1 = dimensions1.x * dimensions1.y * dimensions1.z;
    float volume2 = dimensions2.x * dimensions2.y * dimensions2.z;
    float mass1 = volume1 * volumetricMass;
    float mass2 = volume2 * volumetricMass;

    // Boîte 1 : lancée depuis la gauche (X négatif)
    Vector position1(-launchDistance, launchHeight, launchDistance);
    auto shape1 = std::make_unique<BoxShape>(dimensions1);
    Matrix3 inertiaTensor1 = BoxShape::computeInertiaTensor(dimensions1, mass1);
    Matrix3 invInertiaTensor1 = inertiaTensor1.inverse();

    auto projectile1 = std::make_unique<RigidBody>(
        position1, // centre
        position1, // centre de masse
        Vector(20, 10, -20), // vitesse initiale
        Vector(0, 0, 0), // accélération
        Quaternion(), // orientation
        Vector(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)), // vitesse angulaire aléatoire
        Vector(0, 0, 0), // accélération angulaire
        mass1,
        invInertiaTensor1,
        std::move(shape1)
    );

    // Boîte 2 : lancée depuis la droite (X positif)
    Vector position2(launchDistance, launchHeight, -launchDistance);
    auto shape2 = std::make_unique<BoxShape>(dimensions2);
    Matrix3 inertiaTensor2 = BoxShape::computeInertiaTensor(dimensions2, mass2);
    Matrix3 invInertiaTensor2 = inertiaTensor2.inverse();

    auto projectile2 = std::make_unique<RigidBody>(
        position2, // centre
        position2, // centre de masse
        Vector(-20.0f, 10, 20), // vitesse initiale
        Vector(0, 0, 0), // accélération
        Quaternion(), // orientation
        Vector(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)), // vitesse angulaire aléatoire
        Vector(0, 0, 0), // accélération angulaire
        mass2,
        invInertiaTensor2,
        std::move(shape2)
    );

    // Ajouter les projectiles au monde
    world.addRigidBody(std::move(projectile1));
    world.addRigidBody(std::move(projectile2));
}

void ofApp::throwForwardProjectile() {
	const float volumetricMass = 2500.0f;
	const float boxSize = 2.0f;
	const float initialSpeed = 60.0f;

	// Dimensions
	Vector dimensions(boxSize, boxSize, boxSize);
	float volume = boxSize * boxSize * boxSize;
	float mass = volume * volumetricMass;

	// Direction de la caméra : glm::vec3
	glm::vec3 camPos = cam.getGlobalPosition();
	glm::vec3 dir = glm::normalize(cam.getLookAtDir()); // <-- correction

	// Position de spawn : un peu devant la caméra
	glm::vec3 spawn = camPos + dir * 5.0f;

	Vector spawnPos(spawn.x, spawn.y, spawn.z);

	// Vitesse dans l’axe caméra
	Vector velocity(dir.x * initialSpeed,
		dir.y * initialSpeed,
		dir.z * initialSpeed);

	// Shape + inertie
	auto shape = std::make_unique<BoxShape>(dimensions);
	Matrix3 inertiaTensor = BoxShape::computeInertiaTensor(dimensions, mass);
	Matrix3 invInertiaTensor = inertiaTensor.inverse();

	// Corps rigide
	auto projectile = std::make_unique<RigidBody>(
		spawnPos,
		spawnPos,
		velocity,
		Vector(0, 0, 0),
		Quaternion(),
		Vector(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)),
		Vector(0, 0, 0),
		mass,
		invInertiaTensor,
		std::move(shape));

	world.addRigidBody(std::move(projectile));
}


//--------------------------------------------------------------
void ofApp::keyReleased(const int key) {
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
