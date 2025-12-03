#include "World.h"
#include "Matrix4.h"


World::World() {
	tree = new Octree(Vector(0, 0, 0), treeHalfSize);
}



void World::update(float dt) {
	for (RigidBody * body : rigidBodies) {
		float mass = 1 / body->invMass;
		body->addForce(Force(Vector(0, -9.81f * mass, 0), body->massCenter));
		body->updateAccelerationsWithAccumulator();
		body->integratePos(dt);
		body->integrateOrientation(dt);
		body->clearAccumulator();
		body->updateInvInertiaTensor();
	}
	rebuildOctree();
}



void World::draw() const {
	ofSetColor(100);
	ofDrawGrid(10.0f, 10, false, false, true, false);

	// Dessin des objets physiques
	for (const RigidBody * body : rigidBodies) {
		ofPushMatrix();
		ofTranslate(body->massCenter.x, body->massCenter.y, body->massCenter.z);
		ofMultMatrix(glm::mat4_cast(body->orientation.glmQuat()));

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
			ofDrawBox(0, -body->axeHandleDimensions.y / 2, 0,
				body->axeHandleDimensions.x, body->axeHandleDimensions.y, body->axeHandleDimensions.z);
			ofDrawBox(0, body->axeHeadDimensions.y / 2, 0,
				body->axeHeadDimensions.x, body->axeHeadDimensions.y, body->axeHeadDimensions.z);
			break;
		}
		ofPopMatrix();
	}
	drawOctree(tree);
}


void World::addRigidBody(RigidBody *body) {
    rigidBodies.push_back(body);
}

void World::rebuildOctree() {
	tree->clear();
	for (RigidBody * rb : rigidBodies)
		tree->insert(rb);
}

void World::drawOctree(const Octree * node) const {
	if (!node) return;

	// Récupération du centre et demi-taille
	Vector c = node->center;
	float h = node->halfSize;

	// On dessine un cube filaire
	ofNoFill();
	ofSetColor(255, 243, 0);
	ofDrawBox(c.x, c.y, c.z, h * 2, h * 2, h * 2);

	// Appel récursif sur les enfants
	for (int i = 0; i < 8; i++) {
		if (node->children[i])
			drawOctree(node->children[i].get());
	}
}
