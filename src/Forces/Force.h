#pragma once

#include "../Maths/Vector.h"

struct Force {
    Vector force;
    Vector applicationPoint;

    Force() : force(), applicationPoint() {}
    Force(const Vector& f, const Vector& p) : force(f), applicationPoint(p) {}
};
