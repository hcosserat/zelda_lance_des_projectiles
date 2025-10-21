#pragma once

#include "../Maths/Vector.h"
#include "CollisionType.h"

class Actor;
struct Constraint;

typedef struct CollisionResult {
    bool collides = false;
    Vector normalVector = Vector{0, 0, 0};
    float penetration = 0.0f;
    CollisionType collisionType = TBD;
    Constraint *constraint = nullptr; // Pointer to the constraint if this is a constraint collision
} CollisionResult;
