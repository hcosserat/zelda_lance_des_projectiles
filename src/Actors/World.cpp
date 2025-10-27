#include "World.h"
#include "Forces/Gravity.h"
#include "Circle.h"
#include "Rect.h"
#include "Blob.h"


World::World() {
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

    constraintRegistry.applySpringForces(particleForceRegistry);
    particleForceRegistry.updateForces(dt);
}

void World::updateVelocities(const float dt) const {
    for (auto *actor: actors) {
        actor->centerParticle.integrateVelocity(dt);
    }
}

void World::updatePositions(const float dt) const {
    for (auto *actor: actors) {
        actor->centerParticle.integratePosition(dt);
        actor->centerParticle.clearAccum();
    }
}

void World::update(const float dt) {
    applyForces(dt);
    updateVelocities(dt);
    collisionResolver.resolve(actors, dt, &constraintRegistry);
    updatePositions(dt);
}


void World::draw_rect(Actor *actor) const {
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
}

void World::draw() const {
    ofSetColor(245, 0, 0);

    // Draw constraint lines
    for (const auto &constraint: constraintRegistry.getConstraints()) {
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
                draw_rect(actor);
                break;
            }
            default: {
                std::cout << "Forme inconnue dans World::draw()" << std::endl;
                break;
            }
        }
    }
}
