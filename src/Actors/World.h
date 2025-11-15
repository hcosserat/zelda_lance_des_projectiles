#pragma once
#include <vector>
#include "Forces/ParticleForceRegistry.h"
#include "../Collisions/ConstraintRegistry.h"
#include "../Collisions/CollisionResolver.h"
#include "Actor.h"
#include "Actors/RigidBody.h"

class Circle;

class World {
public:
	std::vector<RigidBody*> rigidBodies;

    explicit World();

    void update(float dt);

    void draw() const;

	void addRigidBody(RigidBody* body);
};
