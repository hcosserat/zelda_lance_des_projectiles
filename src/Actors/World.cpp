#include "World.h"

World::World(float worldSize)
	: collisionComponent(worldSize)
	  , gravity(0, -9.81f, 0) {
}

World::~World() = default;

// ============== Physics ==============

void World::update(float dt) {
	applyGravity();
	integrateAll(dt);
	collisionComponent.updateSpatialStructure(rigidBodies);
	collisionComponent.detectCollisions(rigidBodies);

	// TODO: Collision resolution will be added here
}

void World::applyGravity() {
	for (auto& body : rigidBodies) {
		if (body->invMass > 0) {
			// Skip static bodies (infinite mass)
			float mass = 1.0f / body->invMass;
			body->addForce(Force(gravity * mass, body->massCenter));
		}
	}
}

void World::integrateAll(float dt) {
	for (auto& body : rigidBodies) {
		body->updateAccelerationsWithAccumulator();
		body->integratePos(dt);
		body->integrateOrientation(dt);
		body->clearAccumulator();
		body->updateInvInertiaTensor();
	}
}

void World::addRigidBody(std::unique_ptr<RigidBody> body) {
	rigidBodies.push_back(std::move(body));
}

// ============== Rendering ==============

void World::draw() const {
	drawGrid();
	drawBodies();
	collisionComponent.drawDebug();
}

void World::setDebugDraw(bool enabled) {
	collisionComponent.setDebugDraw(enabled);
}

void World::drawGrid() const {
	ofSetColor(100);
	ofDrawGrid(10.0f, 10, false, false, true, false);
}

void World::drawBodies() const {
	for (const auto& body : rigidBodies) {
		ofPushMatrix();
		ofTranslate(body->massCenter.x, body->massCenter.y, body->massCenter.z);
		ofMultMatrix(glm::mat4_cast(body->orientation.glmQuat()));

		// Draw center of mass marker
		ofSetColor(255, 0, 0);
		ofDrawSphere(0, 0, 0, 0.1f);

		// Delegate shape drawing to the shape component
		body->shape->drawShape();

		ofPopMatrix();
	}
}
