#include "World.h"

World::World(float worldSize)
	: tree(std::make_unique<Octree>(Vector(0, 0, 0), worldSize))
	, treeHalfSize(worldSize)
	, gravity(0, -9.81f, 0)
	, debugDrawEnabled(true) {
}

World::~World() = default;

// ============== Physics ==============

void World::update(float dt) {
	applyGravity();
	integrateAll(dt);
	rebuildOctree();
}

void World::applyGravity() {
	for (auto& body : rigidBodies) {
		if (body->invMass > 0) { // Skip static bodies (infinite mass)
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

void World::rebuildOctree() {
	tree->clear();
	for (const auto& body : rigidBodies) {
		tree->insert(body.get());
	}
}

// ============== Rendering ==============

void World::draw() const {
	drawGrid();
	drawBodies();

	if (debugDrawEnabled) {
		drawOctree(tree.get());
	}
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

void World::drawOctree(const Octree* node) const {
	if (!node) return;

	Vector c = node->center;
	float h = node->halfSize;

	ofNoFill();
	ofSetColor(255, 243, 0);
	ofDrawBox(c.x, c.y, c.z, h * 2, h * 2, h * 2);

	for (const auto& child : node->children) {
		if (child) {
			drawOctree(child.get());
		}
	}
}
