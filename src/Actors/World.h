#pragma once
#include "Collisions/CollisionComponent.h"
#include "Actors/RigidBody.h"
#include <vector>
#include <memory>

class World {
public:
	explicit World(float worldSize = 200.0f);
	~World();

	// Physique
	void update(float dt);
	void addRigidBody(std::unique_ptr<RigidBody> body);

	// Rendu
	void draw() const;

	void setDebugDraw(bool enabled);

	// Configuration
	void setGravity(const Vector &g) { gravity = g; }
	Vector getGravity() const { return gravity; }

	// Accès aux collisions
	const CollisionComponent &getCollisionComponent() const { return collisionComponent; }

private:
	std::vector<std::unique_ptr<RigidBody>> rigidBodies;
	CollisionComponent collisionComponent;
	Vector gravity;

	// Aides physique
	void applyGravity();
	void integrateAll(float dt);

	// Aides rendu
	void drawBodies() const;
	void drawGrid() const;
};
