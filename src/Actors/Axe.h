#pragma once

#include "RigidBody.h"

class Axe final : public RigidBody {
public:
    Vector handleDimensions; // Manche
    Vector headDimensions;   // Tête

    // Constructeur
    Axe(const Vector& center, const Vector& massCenter, const Vector& vel, const Vector& acc,
        const Quaternion& orientation, const Vector& angularVel, const Vector& angularAcc, float mass,
        const Matrix3& invInertiaTensor, const Vector& handleDimensions, const Vector& headDimensions);

    void drawShape() const override;
};