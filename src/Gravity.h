#pragma once

#include "ParticleForceGenerator.h"
#include "Vector.h"

class Particle;

// Générateur de force de gravité
class ParticleGravity : public ParticleForceGenerator {

public:

	Vector gravity;

	// Initialise la gravité
	explicit ParticleGravity(const Vector & g) : gravity(g) { }

	// Modifie la gravité
	void set(const Vector & g) { gravity = g; }
	const Vector & get() const { return gravity; }

	// Applique la force de gravité sur la particule
	virtual void updateForce(Particle * particle, float duration);
};
