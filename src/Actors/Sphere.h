#pragma once

#include "RigidBody.h"

class Sphere : public RigidBody {
    float radius;

public:
    Sphere(const Vector &center, const Vector &massCenter, const Vector &vel, const Vector &acc,
           const Vector &orientation, const Vector &angularVel, const Vector &angularAcc, const float mass,
           const float radius) : RigidBody(center, massCenter, vel, acc, orientation, angularVel, angularAcc, mass,
                                           Matrix3::Identity() * (2.0f / 5.0f * mass * radius)), radius(radius) {
    }
};
