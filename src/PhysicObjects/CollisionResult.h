#pragma once

#include "../Maths/Vector.h"
#include "CollisionType.h"

typedef struct _CollisionResult {
    bool collides = false;
    Vector normalVector = Vector{0, 0, 0};
    CollisionType collisionType = TBD;
} CollisionResult;
