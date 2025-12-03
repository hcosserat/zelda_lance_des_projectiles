#pragma once

#include "RigidBody.h"

class Cylinder final : public RigidBody {
public:
    float radius;
    float height;

    // Constructeur
    Cylinder(const Vector& center, const Vector& massCenter, const Vector& vel, const Vector& acc,
        const Quaternion& orientation, const Vector& angularVel, const Vector& angularAcc, float mass,
        const Matrix3& invInertiaTensor, float radius, float height);

    void drawShape() const override;
};