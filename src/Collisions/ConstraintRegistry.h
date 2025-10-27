#pragma once

#include <vector>
#include <memory>
#include "Constraint.h"
#include "../Maths/Vector.h"

class Actor; // Forward declaration
class ParticleForceRegistry; // Forward declaration
class ParticleForceGenerator; // Forward declaration

class ConstraintRegistry {
private:
    std::vector<ParticleForceGenerator *> springForces; // For memory management

public:
    std::vector<Constraint> constraints;

    // Add a rod constraint (fixed distance)
    void addRod(Actor *a, Actor *b, float length) {
        constraints.emplace_back(a, b, Rod, length);
    }

    // Add a cable constraint (maximum distance)
    void addCable(Actor *a, Actor *b, float maxLength) {
        constraints.emplace_back(a, b, Cable, maxLength);
    }

    // Add a spring constraint between two actors
    void addSpring(Actor *a, Actor *b, float restLength, float springConstant) {
        constraints.emplace_back(a, b, Spring, restLength, springConstant);
    }

    // Add an anchor spring constraint (actor to fixed point)
    void addAnchorSpring(Actor *a, const Vector &anchorPoint, float restLength, float springConstant) {
        constraints.emplace_back(a, AnchorSpring, anchorPoint, restLength, springConstant);
    }

    // Add a bungee spring constraint (only applies force when stretched)
    void addBungeeSpring(Actor *a, Actor *b, float restLength, float springConstant) {
        constraints.emplace_back(a, b, BungeeSpring, restLength, springConstant);
    }

    // Remove constraint between two actors
    void removeConstraint(Actor *a, Actor *b);

    // Apply spring forces from constraints to the force registry
    void applySpringForces(ParticleForceRegistry &forceRegistry);

    // Clear all constraints and clean up spring forces
    void clear();

    // Destructor to clean up memory
    ~ConstraintRegistry();

    const std::vector<Constraint> &getConstraints() const {
        return constraints;
    }
};
