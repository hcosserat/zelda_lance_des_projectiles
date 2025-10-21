#include "World.h"
#include "Forces/Gravity.h"
#include "Circle.h"
#include "Rect.h"
#include <Blob.h>
#include <BlobSpringForce.h>

World::World() {
    // Add Blob
    Blob *blob = new Blob();
    // blob->addCircle();
    actors.emplace_back(blob);

    // Create a pendulum
    // Circle *anchor = new Circle(
    //     Particle(Vector{400, 100, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.0f),
    //     5.0f
    // );
    // Circle *ball = new Circle(
    //     Particle(Vector{400, 300, 0}, Vector{50, 0, 0}, Vector{0, 0, 0}, 0.1f),
    //     20.0f
    // );
    // actors.push_back(anchor);
    // actors.push_back(ball);
    // constraintRegistry.addRod(anchor, ball, 200.0f);

    // Add Floor
    actors.emplace_back(new Rect(
        Particle(Vector{500, 700, 0}),
        Vector{1000, 0, 0},
        Vector{0, 100, 0}
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
                SpringForce *psf;
                if (&c == &blob->circles.back()) {
                    psf = new SpringForce(&blob->circles.front().centerParticle, 100.0f,
                                          1);
                } else {
                    psf = new SpringForce(&(*(std::next(&c))).centerParticle, 100.0f, 1);
                }
                blobForces.push_back(psf);
                Registry.add(&c.centerParticle, psf);
                float restLength = 1 * (blob->centerRadius + c.radius);
                SpringForce *sf = new SpringForce(&blob->centerParticle, 100.0f, restLength);
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

void World::draw() const {
    ofSetColor(245, 0, 0);

    for (Actor *actor: actors) {
        switch (actor->getShape()) {
            case CircleShape: {
                ofSetColor(170);
                ofDrawCircle(actor->centerParticle.pos.x, actor->centerParticle.pos.y,
                             dynamic_cast<Circle *>(actor)->radius);
                break;
            }
            case RectShape: {
                ofSetColor(100);

                Rect *rect = dynamic_cast<Rect *>(actor);

                ofPushMatrix();

                // Translate to the center of the rectangle
                ofTranslate(actor->centerParticle.pos.x, actor->centerParticle.pos.y);

                // Calculate rotation angle from axisU
                float angle = atan2(rect->axisU.y, rect->axisU.x) * 180.0f / PI;
                ofRotateDeg(angle);

                // Draw the rectangle centered at origin
                ofDrawRectangle(-rect->halfA, -rect->halfB, rect->halfA * 2, rect->halfB * 2);

                ofPopMatrix();
                break;
            }
            case BlobShape: {
                Blob *blob = dynamic_cast<Blob *>(actor);
                Circle center = blob->getCenter();
                for (auto &c: blob->circles) {
                    ofSetColor(245, 0, 245); // Purple
                    ofDrawLine(center.centerParticle.pos.x, center.centerParticle.pos.y,
                               c.centerParticle.pos.x, c.centerParticle.pos.y);
                    if (&c != &blob->circles.back()) {
                        ofDrawLine(c.centerParticle.pos.x, c.centerParticle.pos.y,
                                   std::next(&c)->centerParticle.pos.x,
                                   std::next(&c)->centerParticle.pos.y);
                    } else {
                        ofDrawLine(c.centerParticle.pos.x, c.centerParticle.pos.y,
                                   blob->circles.front().centerParticle.pos.x,
                                   blob->circles.front().centerParticle.pos.y);
                    }
                }
                ofSetColor(0, 0, 245); // Blue
                ofDrawCircle(center.centerParticle.pos.x, center.centerParticle.pos.y, center.radius);
                for (const auto &c: blob->circles) {
                    ofSetColor(0, 245, 0); // Green
                    ofDrawCircle(c.centerParticle.pos.x, c.centerParticle.pos.y, c.radius);
                }
                for (const auto &c: blob->separatedCircles) {
                    ofSetColor(100, 100, 100); // Grey
                    ofDrawCircle(c.centerParticle.pos.x, c.centerParticle.pos.y, c.radius);
                }
                break;
            }
            default: {
                std::cout << "Forme inconnue dans World::draw()" << std::endl;
                break;
            }
        }
    }
}
