#pragma once
#include "Collisions/Octree.h"
#include "Actors/RigidBody.h"
#include <vector>
#include <memory>

class World {
public:
	explicit World(float worldSize = 200.0f);
	~World();

	// Physics
	void update(float dt);
	void addRigidBody(std::unique_ptr<RigidBody> body);

	// Rendering
	void draw() const;
	void setDebugDraw(bool enabled) { debugDrawEnabled = enabled; }

	// Configuration
	void setGravity(const Vector& g) { gravity = g; }
	Vector getGravity() const { return gravity; }

private:
	std::vector<std::unique_ptr<RigidBody>> rigidBodies;
	std::unique_ptr<Octree> tree;
	float treeHalfSize;
	Vector gravity;
	bool debugDrawEnabled;

	// Physics helpers
	void applyGravity();
	void integrateAll(float dt);
	void rebuildOctree();

	// Rendering helpers
	void drawBodies() const;
	void drawOctree(const Octree* node) const;
	void drawGrid() const;
};
