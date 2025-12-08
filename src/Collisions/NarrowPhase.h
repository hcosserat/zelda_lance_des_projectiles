#pragma once
#include "../Actors/RigidBody.h"
#include "../Actors/BoxShape.h"
#include "../Actors/PlaneShape.h"
#include "CollisionData.h"

class NarrowPhase {
public:
    static CollisionData CheckCollision(RigidBody *body1, RigidBody *body2);

private:
    static CollisionData BoxAndPlane(RigidBody *boxBody, RigidBody *planeBody);

    static CollisionData BoxAndBox(RigidBody *boxBody1, RigidBody *boxBody2);

    static void TestVerticesAgainstBox(RigidBody *testBody, RigidBody *targetBoxBody, CollisionData &collisionData,
                                       bool flipNormal);
};
