#include "ofApp.h"
#include "Actors/BoxShape.h"
#include "Actors/PlaneShape.h"

// Unit tests - included only in cpp
#include "Maths/VectorTest.h"
#include "Maths/Matrix3Test.h"
#include "Maths/Matrix4Test.h"
#include "Maths/QuaternionTest.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Run unit tests
    test_vector();
    test_matrix3();
    test_matrix4();
    test_quaternion();


    // Configure rendering
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
    // 3D scene
    cam.begin();
    world.draw();
    cam.end();

    // 2D UI overlay
    drawHUD();
}

//--------------------------------------------------------------
void ofApp::drawHUD() const {
    ofDisableDepthTest();

    ofSetColor(255);
    ofDrawBitmapString("Delta Time: " + ofToString(dt * 1000.0f, 2) + " ms", 10, 20);
    ofDrawBitmapString("FPS: " + ofToString(1.0f / dt, 1), 10, 40);
    ofDrawBitmapStringHighlight(
        "Press Space: Throw Box", 40, 70);

    ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(const int key) {
    switch (key) {
        case ' ':
            throwProjectile();
            break;
        case 'd':
        case 'D':
            // Toggle debug drawing
            static bool debugEnabled = true;
            debugEnabled = !debugEnabled;
            world.setDebugDraw(debugEnabled);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::spawnBox(const Vector &position, const Vector &dimensions, float mass) {
    auto shape = std::make_unique<BoxShape>(dimensions);
    Matrix3 inertiaTensor = BoxShape::computeInertiaTensor(dimensions, mass);
    Matrix3 invInertiaTensor = inertiaTensor.inverse();

    auto body = std::make_unique<RigidBody>(
        position, // center
        position, // massCenter
        Vector(0, 0, 0), // velocity
        Vector(0, 0, 0), // acceleration
        Quaternion(), // orientation
        Vector(0, 0, 0), // angular velocity
        Vector(0, 0, 0), // angular acceleration
        mass,
        invInertiaTensor,
        std::move(shape)
    );

    world.addRigidBody(std::move(body));
}

//--------------------------------------------------------------
void ofApp::throwProjectile() {
    Vector position(0, 5, 0);
    Vector dimensions(1, 1, 1);
    float mass = 20.0f;

    auto shape = std::make_unique<BoxShape>(dimensions);
    Matrix3 inertiaTensor = BoxShape::computeInertiaTensor(dimensions, mass);
    Matrix3 invInertiaTensor = inertiaTensor.inverse();

    auto projectile = std::make_unique<RigidBody>(
        position, // center
        position, // massCenter
        Vector(0, 0, 0), // velocity
        Vector(0, 0, 0), // acceleration
        Quaternion(), // orientation
        Vector(0, 0, 0), // angular velocity
        Vector(0, 0, 0), // angular acceleration
        mass,
        invInertiaTensor,
        std::move(shape)
    );

    // Apply force to throw the projectile
    Vector force(0, 1500, 5000);
    Vector applicationPoint = position + Vector(0.5f, 0.5f, 0); // Off-center for rotation
    projectile->addForce(Force(force, applicationPoint));

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

