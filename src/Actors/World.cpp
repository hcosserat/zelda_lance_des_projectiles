#include "World.h"
#include "Forces/Gravity.h"
#include "Circle.h"
#include "Rect.h"
#include "Blob.h"
#include "AnchorSpringForce.h"
#include "BungeeForce.h"


World::World() {
    // Add Blob
    Blob *blob = new Blob();
    blob->addCircle();
    actors.emplace_back(blob);

    // Anchor spring on a rect
    actors.emplace_back(new Rect(
        Particle(Vector{200, 200, 0}),
        Vector{60, 0, 0},
        Vector{0, 10, 0}
    ));
    circleAnchor = new Circle(
        Particle(Vector{200, 180, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.5f),
        20.0f
    );
    actors.push_back(circleAnchor);

    // Bungee spring on a rect
    actors.emplace_back(new Rect(
        Particle(Vector{800, 200, 0}),
        Vector{120, 0, 0},
        Vector{0, 10, 0}
    ));
    circleBungee1 = new Circle(
        Particle(Vector{700, 180, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.5f),
        20.0f
    );
    circleBungee2 = new Circle(
        Particle(Vector{900, 180, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.5f),
        20.0f
    );
    actors.push_back(circleAnchor);
    actors.push_back(circleBungee1);
    actors.push_back(circleBungee2);

    // Add Floor
    actors.emplace_back(new Rect(
        Particle(Vector{500, 800, 0}),
        Vector{3000, 0, 0},
        Vector{0, 200, 0}
    ));

    // Add Walls
    actors.emplace_back(new Rect(Particle(Vector{10, 400, 0}), Vector{0, 600, 0}, Vector{20, 0, 0}));
    actors.emplace_back(new Rect(Particle(Vector{1014, 400, 0}), Vector{0, -600, 0}, Vector{20, 0, 0}));
}

void World::applyForces(const float dt) {
    Registry.clear();
    std::vector<SpringForce *> blobForces; // fixme : memory leak!!!!

    ParticleGravity grav;
    constraintRegistry.clear();  // todo: gérer les cables du blob autrement

    // Anchor spring force
    AnchorSpringForce *anchorSpring = new AnchorSpringForce(  // todo: créer un registre qui gère les ressorts
        Vector{200, 200, 0}, // Anchor point
        20.0f, // Spring constant
        150.0f // Rest length
    );
    Registry.add(&circleAnchor->centerParticle, anchorSpring);
    // Bungee spring forces
    BungeeForce *bungeeSpring = new BungeeForce(
        &circleBungee1->centerParticle,
        10.0f, // Spring constant
        150.0f // Rest length
    );
    Registry.add(&circleBungee2->centerParticle, bungeeSpring);

    for (auto *actor: actors) {
        Registry.add(&actor->centerParticle, &grav);

        if (actor->getShape() == BlobShape) {
            Blob *blob = dynamic_cast<Blob *>(actor);
            for (auto &c: blob->circles) {
                // Apply gravity to each circle
                Registry.add(&c.centerParticle, &grav);

                // Create spring forces between circles
                SpringForce *psf;
                float restLengthC = 4 * c.radius;
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
                constraintRegistry.addCable(blob, &c, 200);
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

    // Draw constraint lines
    for (const auto &constraint: constraintRegistry.getConstraints()) {
        // Skip constraints that involve blob circles (they're internal to the blob)
        bool isInternalBlobConstraint = false;

        // Check if either actor is a Circle that's part of a blob
        for (const auto *actor: actors) {
            if (actor->getShape() == BlobShape) {
                const Blob *blob = dynamic_cast<const Blob *>(actor);

                // Check if constraint.a or constraint.b is one of the blob's circles
                for (const auto &c: blob->circles) {
                    if (constraint.a == &c || constraint.b == &c) {
                        isInternalBlobConstraint = true;
                        break;
                    }
                }
                if (isInternalBlobConstraint) break;
            }
        }

        if (isInternalBlobConstraint) continue;

        // Draw the constraint line
        const Vector posA = constraint.a->centerParticle.pos;
        const Vector posB = constraint.b->centerParticle.pos;

        if (constraint.type == Rod) {
            ofSetColor(255, 0, 0); // Red for rod constraints
            ofSetLineWidth(2);
        } else {
            // Cable
            ofSetColor(0, 100, 255); // Blue for cable constraints
            ofSetLineWidth(1);
        }

        ofDrawLine(posA.x, posA.y, posB.x, posB.y);
    }

    // Reset line width
    ofSetLineWidth(1);

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

                // Calculate the four corners
                Vector center = rect->centerParticle.pos;
                Vector corner1 = center + rect->halfA * rect->axisU + rect->halfB * rect->axisV;
                Vector corner2 = center - rect->halfA * rect->axisU + rect->halfB * rect->axisV;
                Vector corner3 = center - rect->halfA * rect->axisU - rect->halfB * rect->axisV;
                Vector corner4 = center + rect->halfA * rect->axisU - rect->halfB * rect->axisV;

                // Draw the rectangle
                ofBeginShape();
                ofVertex(corner1.x, corner1.y);
                ofVertex(corner2.x, corner2.y);
                ofVertex(corner3.x, corner3.y);
                ofVertex(corner4.x, corner4.y);
                ofEndShape(true); // close the shape

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
