#pragma once
#include "../Maths/Vector.h"
#include <vector>

class RigidBody; // Forward declaration

struct Contact {
    Vector point; // Contact point in world space
    Vector normal; // Collision normal (from body1 to body2)
    float interpenetration; // Penetration depth
};

struct CollisionData {
    RigidBody *body1 = nullptr;
    RigidBody *body2 = nullptr;
    std::vector<Contact> contacts;

    bool hasCollision() const { return !contacts.empty(); }

    void addContact(const Vector &pt, const Vector &norm, float interpen) {
        contacts.push_back({pt, norm, interpen});
    }
};
