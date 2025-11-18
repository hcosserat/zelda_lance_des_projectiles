#include "World.h"
#include "Forces/Gravity.h"
#include "Matrix4.h"


World::World() {
}


void World::update(const float dt) {
    for (RigidBody *body: rigidBodies) {
        // Update accelerations based on accumulated forces
        body->updateAccelerationsWithAccumulator();
        // Integrate position and orientation
        body->integratePos(dt);
        body->integrateOrientation(dt);
        // Clear accumulated forces for the next frame
        body->clearAccumulator();
        body->updateInvInertiaTensor();
    }
}


void World::draw() const {
    ofSetColor(100);
    ofDrawGrid(10.0f, 10, false, false, true, false); // Draw ground grid

    for (const RigidBody *body: rigidBodies) {
        ofPushMatrix();

        ofTranslate(body->massCenter.x,
                    body->massCenter.y,
                    body->massCenter.z);

        const Vector& o = body->orientation; // pitch=x, yaw=y, roll=z
        ofRotateZDeg(ofRadToDeg(o.z)); // roll
        ofRotateYDeg(ofRadToDeg(o.y)); // yaw
        ofRotateXDeg(ofRadToDeg(o.x)); // pitch

        // Center of mass
        ofSetColor(255, 0, 0);
        ofDrawSphere(0, 0, 0, 0.1f);
        switch (body->shape) {
            case BOX:
                ofSetColor(200, 50, 50);
                ofDrawBox(0, 0, 0,
                          body->boxDimensions.x, body->boxDimensions.y, body->boxDimensions.z);
                break;
            case CYLINDER:
                ofSetColor(50, 200, 50);
                ofDrawCylinder(0, 0, 0,
                               body->radius, body->height);
                break;
            case AXE:
                ofSetColor(50, 50, 200);
                // Draw handle
                ofDrawBox(0, -body->axeHandleDimensions.y / 2.0f,
                          0,
                          body->axeHandleDimensions.x, body->axeHandleDimensions.y, body->axeHandleDimensions.z);
                // Draw head
                ofDrawBox(0, body->axeHeadDimensions.y / 2,
                          0,
                          body->axeHeadDimensions.x, body->axeHeadDimensions.y, body->axeHeadDimensions.z);
                break;
            default:
                break;
        }
        ofPopMatrix();
    }
}

void World::addRigidBody(RigidBody *body) {
    rigidBodies.push_back(body);
}
