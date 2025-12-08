#pragma once
#include "RigidBody.h"

class Plane : public RigidBody {
public:
    Vector point;
    Vector normal;

    Plane(const Vector &p, const Vector &n)
        : RigidBody(
              p, // center (arbitraire, on utilise point)
              p, // massCenter
              Vector(0, 0, 0), // velocity (statique)
              Vector(0, 0, 0), // acceleration
              Quaternion(1, 0, 0, 0), // orientation (identité, car définie par normale)
              Vector(0, 0, 0), // angularVel
              Vector(0, 0, 0), // angularAcc
              0.0f, // MASSE (0 = infini)
              Matrix3(), // Inertia (0)
              PLANE,
          ),
          point(p),
          normal(n) {
        this->normal.normalize();
    }
};
