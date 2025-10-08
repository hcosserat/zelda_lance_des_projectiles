#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"
class ParticleForceRegistry
{

	// Keeps track of all particles and their corresponding force generators
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* fg;
	};

	// Dynamic array of registrations
	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

	// Add a new registration to the registry
	void add(Particle* particle, ParticleForceGenerator* fg);
	// Remove a registration from the registry
	void remove(Particle* particle, ParticleForceGenerator* fg);
	// Delete the registry but not the particles or force generators
	void clear();
	void updateForces(float duration);

};

