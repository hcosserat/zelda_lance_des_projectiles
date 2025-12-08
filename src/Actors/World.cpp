#include "World.h"
#include "PlaneShape.h"

World::World(float worldSize)
	: collisionComponent(worldSize)
	, gravity(0, -9.81f, 0) {
	// Create ground plane at y=0 with normal pointing up
	auto groundShape = std::make_unique<PlaneShape>(Vector(0, 0, 0), Vector(0, 1, 0));
	auto groundPlane = std::make_unique<RigidBody>(
		Vector(0, 0, 0),      // center
		Vector(0, 0, 0),      // massCenter
		Vector(0, 0, 0),      // velocity
		Vector(0, 0, 0),      // acceleration
		Quaternion(),         // orientation
		Vector(0, 0, 0),      // angular velocity
		Vector(0, 0, 0),      // angular acceleration
		0.0f,                 // mass = 0 (static/infinite mass)
		Matrix3() * 0,            // inertia tensor (zero for static)
		std::move(groundShape)
	);
	addRigidBody(std::move(groundPlane));
}

World::~World() = default;

// ============== Physics ==============

void World::update(float dt) {
	applyGravity();
	integrateAll(dt);
	collisionComponent.updateSpatialStructure(rigidBodies);
	collisionComponent.detectCollisions(rigidBodies);
	collisionComponent.resolveCollisions(dt);
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

		if (collisionComponent.isDebugDrawEnabled()) {
			// Draw velocity vector
			ofSetColor(0, 255, 255); // Cyan
			ofDrawLine(body->massCenter.x, body->massCenter.y, body->massCenter.z,
			           body->massCenter.x + body->vel.x,
			           body->massCenter.y + body->vel.y,
			           body->massCenter.z + body->vel.z);

			// Draw bounding sphere
			ofNoFill();
			ofSetColor(255, 255, 0); // Yellow
			ofDrawSphere(body->center.x, body->center.y, body->center.z, body->boundingRadius());
			ofFill();
		}
	}
}
