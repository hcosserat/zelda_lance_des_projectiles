#include "World.h"
#include "Forces/Gravity.h"
#include "Circle.h"
#include "Rect.h"
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

	// Add Floor
	actors.emplace_back(new Rect(
        Particle(Vector{500, 700, 0}),
        Vector { 1000, 0, 0},
        Vector{0, 100, 0 }
	));
}

void World::WorldCollisions() {
}

void World::WorldForces(float dt) {
    ParticleGravity grav;
    Registry.clear();
    std::vector<SpringForce *> blobForces; // To manage memory

    for (auto *actor: actors) {
        // Apply gravity
        Particle &particle = actor->centerParticle;
        Registry.add(&particle, &grav);
        // Apply Spring forces if Blob
        if (actor->getShape() == BlobShape) {
            Blob *blob = dynamic_cast<Blob *>(actor);
            for (auto &c: blob->circles) {
                Registry.add(&c.centerParticle, &grav);
                SpringForce* psf;
                if (&c == &blob->circles.back()) {
                    psf = new SpringForce(&blob->circles.front().centerParticle, 1.0f,
						100);
				}
                else {
					psf = new SpringForce(&(*(std::next(&c))).centerParticle, 1.0f, 100);
                }
				blobForces.push_back(psf);
				Registry.add(&c.centerParticle, psf);
                float restLength = 10 * (blob->center.radius + c.radius);
				SpringForce* sf = new SpringForce(&blob->centerParticle, 1.0f, restLength);
                blobForces.push_back(sf);
                Registry.add(&c.centerParticle, sf);
            }
            for (auto &c: blob->separatedCircles) {
                Registry.add(&c.centerParticle, &grav);
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
            for (auto &c: blob->separatedCircles) {
                Particle &satelliteParticle = c.centerParticle;
                satelliteParticle.pos = satelliteParticle.integrate(dt);
                satelliteParticle.clearAccum();
			}
        }
    }
    for (SpringForce *sf: blobForces) {
        delete sf;
    }
    Registry.clear();
}
