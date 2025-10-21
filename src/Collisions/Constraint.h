#pragma once

class Actor;

enum ConstraintType {
    Rod, // Fixed distance constraint (equality)
    Cable // Maximum distance constraint (inequality - only prevents stretching)
};

struct Constraint {
    Actor *a;
    Actor *b;
    ConstraintType type;
    float length; // Rod: exact distance | Cable: maximum distance

    Constraint(Actor *a, Actor *b, const ConstraintType type, const float length)
        : a(a), b(b), type(type), length(length) {
    }
};
