#include "World.h"
#include "Forces/Gravity.h"
#include "Circle.h"

World::World() {
	actors.emplace_back(new Circle(
		Particle(Vector{100, 200, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.1, Vector{0, 0, 0})
	));
}

void World::WorldCollisions() { }

void World::WorldForces(float dt) {
	ParticleGravity grav;
	Registry.clear();

	for (auto * actor : actors) {
		Particle & particle = actor->centerParticle;
		Registry.add(&particle, &grav);
	}

	Registry.updateForces(dt);

	for (auto * actor : actors) {
		Particle & particle = actor->centerParticle;
		particle.pos = particle.integrate(dt);
		particle.clearAccum();
	}

	Registry.clear();
}
