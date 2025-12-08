#pragma once
#include "Collisions/CollisionComponent.h"
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

	void setDebugDraw(bool enabled);

	// Configuration
	void setGravity(const Vector &g) { gravity = g; }
	Vector getGravity() const { return gravity; }

	// Collision access
	const CollisionComponent &getCollisionComponent() const { return collisionComponent; }

private:
	std::vector<std::unique_ptr<RigidBody>> rigidBodies;
	CollisionComponent collisionComponent;
	Vector gravity;

	// Physics helpers
	void applyGravity();
	void integrateAll(float dt);

	// Rendering helpers
	void drawBodies() const;
	void drawGrid() const;
};
