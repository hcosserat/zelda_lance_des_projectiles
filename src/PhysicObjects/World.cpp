#include "World.h"
#include "Forces/Gravity.h"
#include "Circle.h"
#include <Blob.h>
#include <BlobSpringForce.h>

World::World() {
    actors.emplace_back(new Circle(
        Particle(Vector{100, 200, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.1, Vector{0, 0, 0})
    ));

    // Add Blob
    Blob *blob = new Blob();
    blob->addCircle();
    actors.emplace_back(blob);
}

void World::WorldCollisions() {
}

void World::WorldForces(float dt) {
    ParticleGravity grav;
    Registry.clear();
    std::vector<BlobSpringForce *> blobForces; // To manage memory

    for (auto *actor: actors) {
        // Apply gravity
        Particle &particle = actor->centerParticle;
        Registry.add(&particle, &grav);
        // Apply Spring forces if Blob
        if (actor->getShape() == BlobShape) {
            Blob *blob = dynamic_cast<Blob *>(actor);
            for (auto &c: blob->circles) {
                Registry.add(&c.centerParticle, &grav);
                float restLength = (blob->center.radius + c.radius);
                BlobSpringForce *bsf = new BlobSpringForce(&blob->centerParticle, 1500.0f, restLength, 1.0f);
                blobForces.push_back(bsf);
                Registry.add(&c.centerParticle, bsf);
            }
        }
    }

    Registry.updateForces(dt);

    for (auto *actor: actors) {
        Particle &particle = actor->centerParticle;
        particle.pos = particle.integrate(dt);
        particle.clearAccum();
        if (Blob *blob = dynamic_cast<Blob *>(actor)) {
            for (auto &c: blob->circles) {
                Particle &satelliteParticle = c.centerParticle;
                satelliteParticle.pos = satelliteParticle.integrate(dt);
                satelliteParticle.clearAccum();
            }
        }
    }
    for (BlobSpringForce *bsf: blobForces) {
        delete bsf;
    }
    Registry.clear();
}
