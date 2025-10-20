#pragma once

#include "../Maths/Vector.h"
#include "CollisionType.h"

class Actor;

typedef struct CollisionResult {
    bool collides = false;
    Vector normalVector = Vector{0, 0, 0};
    float penetration = 0.0f;
    CollisionType collisionType = TBD;
} CollisionResult;
