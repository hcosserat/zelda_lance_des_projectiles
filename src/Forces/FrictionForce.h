#pragma once
#include "ParticleForceGenerator.h"
#include "../Maths/Vector.h"
#include "../Actors/Particle.h"

class Particle;

class FrictionForce : public ParticleForceGenerator {
public:
	float coef_friction;
	Vector vec_norm;

	// Initialise la friction
	explicit FrictionForce(
		float coef = 1.0,
		Vector n = Vector { 0, 1, 0 }
	);

	// Modifie la friction
	void set(const float & coef) { coef_friction = coef; }
	const float & get() const { return coef_friction; }

	// Applique la force de friction sur la particule
	virtual void updateforce(Particle * particle, float duration);
};
