#pragma once

#include "Vector.h"


class Particle {
public:
    // all units should be SI
    Vector pos, vel, acc; // postion, velocity, acceleration vectors
    float inverseMass; // inverse of mass
    float damping_factor; // = ln(d)
    // Projectile type
    enum projectileType { Balle, Boulet, Laser, BouleDeFeu };
	projectileType type;
	bool showTrajectory = true;

    // Constructor
    explicit Particle(
        Vector pos = Vector{0, 0, 0},
        Vector vel = Vector{0, 0, 0},
        Vector acc = Vector{0, 0, 0},
        float mass = 1,
        float d = 1,
        projectileType type = Balle);

    // Euler method, returns new position
    Vector integrate(float dt);

};
