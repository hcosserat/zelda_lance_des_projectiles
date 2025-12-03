#pragma once
#include "RigidBody.h"

class Plane : public RigidBody {
public:
    Vector point;
    Vector normal;

    Plane(const Vector& p, const Vector& n)
        : RigidBody(
            p,                  // center (arbitraire, on utilise point)
            p,                  // massCenter
            Vector(0, 0, 0),      // velocity (statique)
            Vector(0, 0, 0),      // acceleration
            Quaternion(1, 0, 0, 0),// orientation (identité, car définie par normale)
            Vector(0, 0, 0),      // angularVel
            Vector(0, 0, 0),      // angularAcc
            0.0f,               // MASSE (0 = infini dans RigidBody::RigidBody mais on force invMass après)
            Matrix3()           // Inertia (0)
        ),
        point(p),
        normal(n)
    {
        this->shape = PLANE;
        this->invMass = 0.0f;   // Masse infinie (ne bouge pas)
        this->normal.normalize();
    }
};