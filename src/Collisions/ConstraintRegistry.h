#pragma once

#include <vector>
#include "Constraint.h"

class Actor; // Forward declaration

class ConstraintRegistry {
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

    void clear() {
        constraints.clear();
    }

    const std::vector<Constraint> &getConstraints() const {
        return constraints;
    }
};
