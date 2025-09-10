#pragma once

#include "Vector.h"

class Particle {
public:
    Vector pos, vel, acc;
    float mass;
    float inverseMass;

    // Constructor
    explicit Particle(
        Vector pos = Vector{0, 0, 0},
        Vector vel = Vector{0, 0, 0},
        Vector acc = Vector{0, 0, 0},
        float mass = 1);
};
