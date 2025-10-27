#include "World.h"
#include "Forces/Gravity.h"
#include "Circle.h"
#include "Rect.h"
#include "Blob.h"


World::World() : blob(nullptr) {
    // Add Blob
    blob = new Blob(&constraintRegistry);
    blob->addCircle();
    actors.emplace_back(blob);

    // Anchor spring on a rect
    actors.emplace_back(new Rect(
        Particle(Vector{200, 200, 0}),
        Vector{60, 0, 0},
        Vector{0, 10, 0}
    ));
    const auto circleAnchor = new Circle(
        Particle(Vector{200, 180, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.5f),
        20.0f
    );
    actors.push_back(circleAnchor);
    constraintRegistry.addAnchorSpring(circleAnchor, Vector{200, 200, 0}, 150.0f, 20.0f);

    // Bungee spring setup
    actors.emplace_back(new Rect(
        Particle(Vector{800, 200, 0}),
        Vector{120, 0, 0},
        Vector{0, 10, 0}
    ));
    const auto circleBungee1 = new Circle(
        Particle(Vector{700, 180, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.5f),
        20.0f
    );
    const auto circleBungee2 = new Circle(
        Particle(Vector{900, 180, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, 0.5f),
        20.0f
    );
    actors.push_back(circleBungee1);
    actors.push_back(circleBungee2);
    constraintRegistry.addBungeeSpring(circleBungee1, circleBungee2, 150.0f, 10.0f);

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
    particleForceRegistry.clear();
    ParticleGravity grav;

    // Apply gravity to all actors
    for (auto *actor: actors) {
        particleForceRegistry.add(&actor->centerParticle, &grav);
    }
    // Apply gravity to blob circles
    for (auto &c: blob->circles) {
        particleForceRegistry.add(&c.centerParticle, &grav);
    }
    // Apply gravity to separated circles
    for (auto &c: blob->separatedCircles) {
        particleForceRegistry.add(&c.centerParticle, &grav);
    }

    constraintRegistry.applySpringForces(particleForceRegistry);
    particleForceRegistry.updateForces(dt);
}

void World::updateVelocities(const float dt) const {
    for (auto *actor: actors) {
        actor->centerParticle.integrateVelocity(dt);
    }
    for (auto &c: blob->circles) {
        c.centerParticle.integrateVelocity(dt);
    }
    for (auto &c: blob->separatedCircles) {
        c.centerParticle.integrateVelocity(dt);
    }
}

void World::updatePositions(const float dt) const {
    for (auto *actor: actors) {
        actor->centerParticle.integratePosition(dt);
        actor->centerParticle.clearAccum();
    }
    for (auto &c: blob->circles) {
        c.centerParticle.integratePosition(dt);
        c.centerParticle.clearAccum();
    }
    for (auto &c: blob->separatedCircles) {
        c.centerParticle.integratePosition(dt);
        c.centerParticle.clearAccum();
    }
}

void World::update(const float dt) {
    applyForces(dt);
    updateVelocities(dt);
    collisionResolver.resolve(actors, dt, &constraintRegistry);
    updatePositions(dt);
}

Blob *World::getBlob() const {
    return blob;
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

                // Check if constraint.a is one of the blob's circles
                for (const auto &c: blob->circles) {
                    if (constraint.a == &c) {
                        isInternalBlobConstraint = true;
                        break;
                    }
                }

                // Also check constraint.b if it exists
                if (!isInternalBlobConstraint && constraint.b) {
                    for (const auto &c: blob->circles) {
                        if (constraint.b == &c) {
                            isInternalBlobConstraint = true;
                            break;
                        }
                    }
                }

                if (isInternalBlobConstraint) break;
            }
        }

        if (isInternalBlobConstraint) continue;

        // Handle different constraint types for drawing
        Vector posA = constraint.a->centerParticle.pos;
        Vector posB;

        if (constraint.type == AnchorSpring) {
            // For anchor springs, draw from actor to anchor point
            posB = constraint.anchorPoint;
            ofSetColor(255, 165, 0); // Orange for anchor springs
            ofSetLineWidth(2);
        } else if (constraint.b) {
            // For constraints between two actors
            posB = constraint.b->centerParticle.pos;

            if (constraint.type == Rod) {
                ofSetColor(255, 0, 0); // Red for rod constraints
                ofSetLineWidth(2);
            } else if (constraint.type == Cable) {
                ofSetColor(0, 100, 255); // Blue for cable constraints
                ofSetLineWidth(1);
            } else if (constraint.type == BungeeSpring) {
                ofSetColor(255, 255, 0); // Yellow for bungee springs
                ofSetLineWidth(2);
            } else {
                // Default for other spring types
                ofSetColor(0, 255, 0); // Green for regular springs
                ofSetLineWidth(1);
            }
        } else {
            // Skip constraints without a second actor and not anchor springs
            continue;
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
