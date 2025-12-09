#include "World.h"
#include "PlaneShape.h"

World::World(float worldSize)
	: collisionComponent(worldSize)
	  , gravity(0, -9.81f, 0)
	  , debugEnabled(true) {
	// Créer le sol (plan) en y=0
	auto groundShape = std::make_unique<PlaneShape>(Vector(0, 0, 0), Vector(0, 1, 0));
	auto groundPlane = std::make_unique<RigidBody>(
		Vector(0, 0, 0), // centre
		Vector(0, 0, 0), // centre de masse
		Vector(0, 0, 0), // vitesse
		Vector(0, 0, 0), // accélération
		Quaternion(), // orientation
		Vector(0, 0, 0), // vitesse angulaire
		Vector(0, 0, 0), // accélération angulaire
		0.0f, // masse = 0 (statique)
		Matrix3() * 0, // inertie nulle pour objet statique
		std::move(groundShape)
	);
	addRigidBody(std::move(groundPlane));
}

World::~World() = default;

// ============== Physique ==============

void World::update(float dt) {
	collisionComponent.updateSpatialStructure(rigidBodies);
	collisionComponent.detectCollisions();
	collisionComponent.resolveCollisions(dt);
	applyGravity();
	integrateAll(dt);
}

void World::applyGravity() {
	for (auto &body: rigidBodies) {
		if (body->invMass > 0) {
			// Ignorer les corps statiques
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

// ============== Rendu ==============

void World::draw() const {
	drawGrid();
	drawBodies();
	collisionComponent.drawDebug();
}

void World::setDebugDraw(bool enabled) {
	debugEnabled = enabled;
	collisionComponent.setDebugDraw(enabled);
}

bool World::isDebugDrawEnabled() const {
	return debugEnabled;
}

void World::toggleDebugDraw() {
	setDebugDraw(!debugEnabled);
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

		// Marqueur centre de masse
		ofSetColor(255, 0, 0);
		ofDrawSphere(0, 0, 0, 0.1f);

		// Délégué au shape
		body->shape->drawShape();

		ofPopMatrix();

		if (debugEnabled) {
			// Vecteur vitesse
			ofSetColor(0, 255, 255); // Cyan
			ofDrawLine(body->massCenter.x, body->massCenter.y, body->massCenter.z,
			           body->massCenter.x + body->vel.x,
			           body->massCenter.y + body->vel.y,
			           body->massCenter.z + body->vel.z);

			// Sphère englobante
			ofNoFill();
			ofSetColor(255, 255, 0); // Jaune
			ofDrawSphere(body->center.x, body->center.y, body->center.z, body->boundingRadius());
			ofFill();
		}
	}
}
