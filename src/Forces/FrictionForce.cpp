#include "FrictionForce.h"

FrictionForce::FrictionForce(const float coef, const Vector n)
	: coef_friction(coef)
	, vec_norm(n) {
}

void FrictionForce::updateforce(Particle * p, float /*dt*/) {
	if (!p) return;

	//p->addforce();
}
