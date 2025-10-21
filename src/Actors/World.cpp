#include "World.h"
#include "Forces/Gravity.h"
#include "Circle.h"
#include "Rect.h"
#include <Blob.h>
#include <BlobSpringForce.h>

World::World() {
    // Add Blob
    Blob *blob = new Blob();
    blob->addCircle();
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
        Particle(Vector{500, 800, 0}),
        Vector{3000, 0, 0},
        Vector{0, 200, 0}
    ));

    // Add Walls
    actors.emplace_back(new Rect(Particle(Vector{1014, 400, 0}), Vector{50, -600, 0}, Vector{30, 30, 0}));
    actors.emplace_back(new Rect(Particle(Vector{10, 400, 0}), Vector{50, 600, 0}, Vector{30, 30, 0}));
}

void World::applyForces(const float dt) {
    Registry.clear();
    constraintRegistry.clear();
    std::vector<SpringForce *> blobForces;

    ParticleGravity grav;

    for (auto *actor: actors) {
        Registry.add(&actor->centerParticle, &grav);

        if (actor->getShape() == BlobShape) {
            Blob *blob = dynamic_cast<Blob *>(actor);
            for (auto &c: blob->circles) {
                // Apply gravity to each circle
                Registry.add(&c.centerParticle, &grav);
                // Create spring forces between circles
                SpringForce *psf;
				float restLengthC = 4 * (c.radius);
                if (&c == &blob->circles.back()) {
					psf = new SpringForce(&blob->circles.front().centerParticle, 100.0f, restLengthC);
                } else {
                    psf = new SpringForce(&std::next(&c)->centerParticle, 100.0f, restLengthC);
                }
                blobForces.push_back(psf);
                Registry.add(&c.centerParticle, psf);
                // Create spring forces between circle and center
                float restLength = 4 * (blob->centerRadius + c.radius);
                SpringForce *sf = new SpringForce(&blob->centerParticle, 100.0f, restLength);
                blobForces.push_back(sf);
                Registry.add(&c.centerParticle, sf);
                // Create elasticity limits between circles and center
                const float elasticityFactor = 5.0f;
                float currentLength = (blob->centerParticle.pos - c.centerParticle.pos).norm();
                if (currentLength > restLength * elasticityFactor) {
                    constraintRegistry.addCable(blob, &c,
                                                restLength * elasticityFactor);
                }
            }
            for (auto &c: blob->separatedCircles) {
                Registry.add(&c.centerParticle, &grav);
            }
        }
    }

    Registry.updateForces(dt);
}

void World::updateVelocities(const float dt) {
    for (auto *actor: actors) {
        actor->centerParticle.integrateVelocity(dt);
        if (Blob *blob = dynamic_cast<Blob *>(actor)) {
            for (auto &c: blob->circles) {
                c.centerParticle.integrateVelocity(dt);
            }
            for (auto &c: blob->separatedCircles) {
                c.centerParticle.integrateVelocity(dt);
            }
        }
    }
}

void World::updatePositions(const float dt) {
    for (auto *actor: actors) {
        actor->centerParticle.integratePosition(dt);
        actor->centerParticle.clearAccum();
        if (Blob *blob = dynamic_cast<Blob *>(actor)) {
            for (auto &c: blob->circles) {
                c.centerParticle.integratePosition(dt);
                c.centerParticle.clearAccum();
            }
            for (auto &c: blob->separatedCircles) {
                c.centerParticle.integratePosition(dt);
                c.centerParticle.clearAccum();
            }
        }
    }
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
