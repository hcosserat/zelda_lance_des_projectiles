#pragma once

#include "../Maths/Vector.h"

class Actor;

enum ConstraintType {
    Rod, // Fixed distance constraint (equality)
    Cable, // Maximum distance constraint (inequality - only prevents stretching)
    Spring, // Spring force between two actors
    AnchorSpring, // Spring force to a fixed point
    BungeeSpring // Spring force that only applies when stretched
};

struct Constraint {
    Actor *a;
    Actor *b;
    ConstraintType type;
    float length; // Rod: exact distance | Cable: maximum distance | Spring/Bungee: rest length
    float springConstant; // Only used for spring types
    Vector anchorPoint; // Only used for AnchorSpring

    // Constructor for Rod and Cable
    Constraint(Actor *a, Actor *b, const ConstraintType type, const float length)
        : a(a), b(b), type(type), length(length), springConstant(0.0f), anchorPoint{0, 0, 0} {
    }

    // Constructor for Spring and BungeeSpring
    Constraint(Actor *a, Actor *b, const ConstraintType type, const float restLength, const float springConstant)
        : a(a), b(b), type(type), length(restLength), springConstant(springConstant), anchorPoint{0, 0, 0} {
    }

    // Constructor for AnchorSpring
    Constraint(Actor *a, const ConstraintType type, const Vector &anchor, const float restLength,
               const float springConstant)
        : a(a), b(nullptr), type(type), length(restLength), springConstant(springConstant), anchorPoint(anchor) {
    }
};
