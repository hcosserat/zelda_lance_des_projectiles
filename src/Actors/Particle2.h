#pragma once

#include "../Maths/Vector.h"

class Particle2 {
public:
    // all units should be SI
    Vector pos, vel, acc; // postion, velocity, acceleration vectors
    Vector prevPos; // previous position
    float inverseMass; // inverse of mass
    float damping_factor; // = ln(d)

    // Constructor
    explicit Particle2(
        Vector pos = Vector{0, 0, 0},
        Vector vel = Vector{0, 0, 0},
        Vector acc = Vector{0, 0, 0},
        float mass = 1,
        float d = 1);

    // Verlet integrator, returns new position
    // Requires a constant `dt` and uses Euler for its first step
    Vector integrate(float dt);
};
