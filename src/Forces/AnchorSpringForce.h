#pragma once
#include "ParticleForceGenerator.h"

class AnchorSpringForce : public ParticleForceGenerator
{
	Vector anchorPoint;
	float springConstant; // stiffness constant K
	float restLength;    // length at rest L0

public:

	AnchorSpringForce(const Vector &anchorPoint, float springConstant, float restLength)
		: anchorPoint(anchorPoint), springConstant(springConstant), restLength(restLength) {
	}
	void updateforce(Particle& particule, float duration);
};

