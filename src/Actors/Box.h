#pragma once

#include "RigidBody.h"

class Box final : public RigidBody {
public:
    Vector dimensions; // Largeur, Hauteur, Profondeur

    // Constructeur
    Box(const Vector& center, const Vector& massCenter, const Vector& vel, const Vector& acc,
        const Quaternion& orientation, const Vector& angularVel, const Vector& angularAcc, float mass,
        const Matrix3& invInertiaTensor, const Vector& dimensions);

    void drawShape() const override;
};