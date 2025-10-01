#pragma once

#include "Vector.h"

class Particle {
public:
    // all units should be SI
    Vector pos, vel, acc, accumForce; // postion, velocity, acceleration and accumForce vectors
    float inverseMass; // inverse of mass
    

    // Constructor
	explicit Particle(
        Vector pos = Vector{0, 0, 0},
        Vector vel = Vector{0, 0, 0},
        Vector acc = Vector{0, 0, 0},
        float mass = 1,
        Vector accumForce = Vector{0, 0, 0}
	);

    // Euler method, returns new position
    Vector integrate(float dt);
	void addforce(const Vector & force);
	void clearAccum();
};
