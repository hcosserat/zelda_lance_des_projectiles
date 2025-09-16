#pragma once

#include "Vector.h"

class Particle {
public:
    // all units should be SI
    Vector pos, vel, acc;  // postion, velocity, acceleration vectors
    Vector prevPos;  // previous position
    float mass;
    float inverseMass; // inverse of mass

    // Constructor
    explicit Particle(
        Vector pos = Vector{0, 0, 0},
        Vector vel = Vector{0, 0, 0},
        Vector acc = Vector{0, 0, 0},
        float mass = 1);

    // Euler method, returns new position
    Vector updateEuler(float dt, float dampingFactor = 0);

    // Verlet integrator, returns new position
    // It requires to use a constant `dt` and first iteration will use Euler instead
    Vector updateVerlet(float dt, float dampingFactor = 0);
};
