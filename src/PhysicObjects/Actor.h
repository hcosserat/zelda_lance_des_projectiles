#pragma once

#include "CollisionResult.h"
#include "Shape.h"

class Actor {
public:
    virtual ~Actor() = default;

    virtual Shape getShape() const = 0;

    virtual CollisionResult collidesWith(const Actor &other) = 0;
};

