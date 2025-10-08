#pragma once

#include "CollisionResult.h"
#include "Particle.h"

#include <array>
#include <limits>
#include <cmath>


class Rect {
public:
    /*
     Schema (vectors from the center to two adjacent sides):

       +-------+-------+
       |       ^       |
       |       |       |
       |       |       |
       |       C------>+
       |               |
       |               |
       |               |
       +---------------+

     - C : centerParticle position.
     - halfA * axisU : from C to the midpoint of one side.
     - halfB * axisV : from C to the midpoint of the adjacent side.
    */
    Particle centerParticle;
    Vector axisU;
    Vector axisV;
    float halfA;
    float halfB;

    explicit Rect(const Particle &centerParticle, const Vector &toSideA,
                  const Vector &toSideB) : centerParticle(centerParticle) {
        const float a = toSideA.norm();
        if (a > 0.0f) {
            axisU = (1.0f / a) * toSideA;
            halfA = a;
        } else {
            axisU = Vector{1.0f, 0.0f, 0.0f};
            halfA = 0.0f;
        }

        const float b = toSideB.norm();
        if (b > 0.0f) {
            axisV = (1.0f / b) * toSideB;
            halfB = b;
        } else {
            axisV = Vector{0.0f, 1.0f, 0.0f};
            halfB = 0.0f;
        }
    }

    CollisionResult collidesWith(const Rect &other) const;
};
