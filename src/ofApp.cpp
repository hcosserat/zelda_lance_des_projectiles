#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Unit tests
    test_vector();
    test_matrix3();
    test_matrix4();
    test_quaternion();

    // Initialization of app attributes
    dt = 1.0f / 60;
    world = World();

    // Enable depth testing for 3D rendering
    ofEnableDepthTest();
    ofSetBackgroundColor(30, 30, 30);

    // Set initial projectile type
    currentProjectileType = P_BOX;
}

//--------------------------------------------------------------
void ofApp::update() {
    dt = ofGetLastFrameTime();
    world.update(dt);

    // Update display value in HUD
    //hud.update(dt);
}


//--------------------------------------------------------------
void ofApp::draw() {
    // Draw from the camera's perspective
    cam.begin();

    // Draw world and all actors
    world.draw();

    cam.end();

    // Draw UI elements
    ofDisableDepthTest();
    // Display current projectile type
    string typeName;
    switch (currentProjectileType) {
        case P_BOX:
            typeName = "Box";
            break;
        case P_CYLINDER:
            typeName = "Cylinder";
            break;
        case P_AXE:
            typeName = "Axe";
            break;
        default:
            typeName = "Unknown";
            break;
    }
    ofSetColor(255);
    ofDrawBitmapString("Delta Time: " + ofToString(dt, 3) + " ms", 10, 20);
    ofDrawBitmapString("Current Projectile: " + typeName, 400, 40);
    ofDrawBitmapStringHighlight(
        "Press 1: Choose Box \nPress 2: Choose Cylinder \nPress 3: Choose Axe \nPress Space: Throw Projectile", 40, 60);
    ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(const int key) {
    // Change projectile type based on key press
    switch (key) {
        case '1':
            currentProjectileType = P_BOX;
            break;
        case '2':
            currentProjectileType = P_CYLINDER;
            break;
        case '3':
            currentProjectileType = P_AXE;
            break;
        case ' ':
            throwProjectile();
            break;
        default:
            break;
    }
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

void ofApp::throwProjectile() {
    // This function can be used to encapsulate projectile throwing logic
    float mass = 20.0f;
    Vector initialPosition(0, 5, 0);
    Vector force(0, 1500, 5000); // Force propelling the projectile up and forward
    Vector localApplicationPoint(0.5, 0.5, 0); // Apply force not in the center of mass to induce rotation
    RigidBody *projectile = nullptr;
    Matrix3 inertiaTensor;
    Matrix3 invInertiaTensor;

    switch (currentProjectileType) {
        case P_BOX: {
            Vector boxDimensions(1, 1, 1);
            float w = boxDimensions.x;
            float h = boxDimensions.y;
            float d = boxDimensions.z;
            float k = (1.0f / 12.0f) * mass;
            inertiaTensor = Matrix3::Identity();
            inertiaTensor(0, 0) = k * (h * h + d * d);
            inertiaTensor(1, 1) = k * (w * w + d * d);
            inertiaTensor(2, 2) = k * (w * w + h * h);
            invInertiaTensor = inertiaTensor.inverse();
            projectile = new RigidBody(initialPosition, initialPosition, Vector(0, 0, 0), Vector(0, 0, 0),
                                       Quaternion(), Vector(0, 0, 0), Vector(0, 0, 0),
                                       mass, invInertiaTensor);
            projectile->shape = BOX;
            projectile->boxDimensions = boxDimensions;
            break;
        }
        case P_CYLINDER: {
            float radius = 0.5f;
            float height = 2.0f;
            float Ixx = (1.0f / 12.0f) * mass * (3 * radius * radius + height * height);
            float Iyy = (0.5f) * mass * radius * radius;
            float Izz = Ixx;
            inertiaTensor = Matrix3::Identity();
            inertiaTensor(0, 0) = Ixx;
            inertiaTensor(1, 1) = Iyy;
            inertiaTensor(2, 2) = Izz;
            invInertiaTensor = inertiaTensor.inverse();
            projectile = new RigidBody(initialPosition, initialPosition, Vector(0, 0, 0), Vector(0, 0, 0),
                                       Quaternion(), Vector(0, 0, 0), Vector(0, 0, 0),
                                       mass, invInertiaTensor);
            projectile->shape = CYLINDER;
            projectile->radius = radius;
            projectile->height = height;
            break;
        }
        case P_AXE: {
            Vector handleDimensions(0.2f, 2.0f, 0.2f);
            Vector headDimensions(0.8f, 0.5f, 0.1f);
            // Approximate to a big box for simplicity
            Vector overallDims(0.8f, 2.5f, 0.2f);
            float w = overallDims.x;
            float h = overallDims.y;
            float d = overallDims.z;
            float k = (1.0f / 12.0f) * mass;
            inertiaTensor = Matrix3::Identity();
            inertiaTensor(0, 0) = k * (h * h + d * d);
            inertiaTensor(1, 1) = k * (w * w + d * d);
            inertiaTensor(2, 2) = k * (w * w + h * h);
            invInertiaTensor = inertiaTensor.inverse();
            projectile = new RigidBody(initialPosition, initialPosition, Vector(0, 0, 0), Vector(0, 0, 0),
                                       Quaternion(), Vector(0, 0, 0), Vector(0, 0, 0),
                                       mass, invInertiaTensor);
            projectile->shape = AXE;
            projectile->axeHandleDimensions = handleDimensions;
            projectile->axeHeadDimensions = headDimensions;
            break;
        }
        default:
            return; // Unknown type
    }
    Vector globalApplicationPoint = initialPosition + localApplicationPoint;
    // Apply the force at the specified local application point
    projectile->addForce(Force(force, globalApplicationPoint));
    // Add the projectile to the world
    world.addRigidBody(projectile);
}
