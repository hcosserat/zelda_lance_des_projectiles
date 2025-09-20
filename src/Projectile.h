#pragma once

#include "Vector.h"
#include "Particle.h"

class Projectile {
public:
	Particle particle;
	// Projectile type
	enum projectileType { Balle, Boulet, Laser, BouleDeFeu };
	projectileType type;
	bool showTrajectory = true; // Whether to show trajectory or not
	Vector velStart; // Initial velocity for trajectory calculation

	explicit Projectile(
		Particle p = Particle(),
		projectileType t = Balle
	);

};