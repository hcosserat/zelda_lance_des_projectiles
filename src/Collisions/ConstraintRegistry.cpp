#include "ConstraintRegistry.h"
#include "../Forces/ParticleForceRegistry.h"
#include "../Forces/SpringForce.h"
#include "../Forces/AnchorSpringForce.h"
#include "../Forces/BungeeForce.h"
#include "../Actors/Actor.h"
#include <algorithm>

void ConstraintRegistry::applySpringForces(ParticleForceRegistry &forceRegistry) {
    // Clear previous spring forces
    for (auto *force: springForces) {
        delete force;
    }
    springForces.clear();

    for (const auto &constraint: constraints) {
        switch (constraint.type) {
            case Spring: {
                // Create spring forces between two actors
                SpringForce *forceAB = new SpringForce(&constraint.b->centerParticle, constraint.springConstant,
                                                       constraint.length);
                SpringForce *forceBA = new SpringForce(&constraint.a->centerParticle, constraint.springConstant,
                                                       constraint.length);

                springForces.push_back(forceAB);
                springForces.push_back(forceBA);

                forceRegistry.add(&constraint.a->centerParticle, forceAB);
                forceRegistry.add(&constraint.b->centerParticle, forceBA);
                break;
            }
            case AnchorSpring: {
                // Create anchor spring force
                AnchorSpringForce *anchorForce = new AnchorSpringForce(constraint.anchorPoint,
                                                                       constraint.springConstant, constraint.length);
                springForces.push_back(anchorForce);
                forceRegistry.add(&constraint.a->centerParticle, anchorForce);
                break;
            }
            case BungeeSpring: {
                // Create bungee spring forces between two actors
                BungeeForce *bungeeAB = new BungeeForce(&constraint.b->centerParticle, constraint.springConstant,
                                                        constraint.length);
                BungeeForce *bungeeBA = new BungeeForce(&constraint.a->centerParticle, constraint.springConstant,
                                                        constraint.length);

                springForces.push_back(bungeeAB);
                springForces.push_back(bungeeBA);

                forceRegistry.add(&constraint.a->centerParticle, bungeeAB);
                forceRegistry.add(&constraint.b->centerParticle, bungeeBA);
                break;
            }
            case Rod:
            case Cable:
                // Handled by the collision resolver
                break;
        }
    }
}

void ConstraintRegistry::removeConstraint(Actor *a, Actor *b) {
    constraints.erase(
        std::remove_if(constraints.begin(), constraints.end(),
                       [a, b](const Constraint &constraint) {
                           return (constraint.a == a && constraint.b == b) ||
                                  (constraint.a == b && constraint.b == a);
                       }),
        constraints.end()
    );
}

void ConstraintRegistry::clear() {
    constraints.clear();
    for (auto *force: springForces) {
        delete force;
    }
    springForces.clear();
}

ConstraintRegistry::~ConstraintRegistry() {
    clear();
}
