#include "ParticleForceRegistry.h"

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
	ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	registrations.push_back(registration);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
{
	for (auto x = registrations.begin(); x != registrations.end(); ++x) {
		if (x->particle == particle && x->fg == fg) {
			registrations.erase(x);
			break;
		}
	}
}

void ParticleForceRegistry::updateForces(float duration)
{
	for (auto& x : registrations) {
		x.fg->updateforce(*x.particle, duration);
	}
}
