#pragma once
#include "../Maths/Vector.h"
#include <vector>

class RigidBody; // Déclaration anticipée

struct Contact {
    Vector point; // Point de contact en espace monde
    Vector normal; // Normal de collision (de body1 vers body2)
    float interpenetration; // Profondeur de pénétration
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
