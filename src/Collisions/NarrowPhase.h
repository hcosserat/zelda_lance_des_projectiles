#pragma once
#include "../Actors/Box.h"
#include "../Actors/Plane.h"
#include "CollisionData.h"

class NarrowPhase {

public:
	static CollisionData CheckCollision(RigidBody* body1, RigidBody* body2);

private:
	static CollisionData BoxAndPlane(Box* box, Plane* plane);
	static CollisionData BoxAndBox(Box* box1, Box* box2);

	static void TestVerticesAgainstBox(Box* box1, Box* box2, CollisionData& collisionData);
};