#pragma once
#include "../Maths/Vector.h"
#include <vector>

struct Contact {
	Vector point; // Point d'impact (R)
	Vector normal; // Normal de collision
	float interpenetration; // Profondeur (||t||)
};

struct CollisionData {
	std::vector<Contact> contacts;
	bool hasCollision() const { return !contacts.empty(); }
	void addContact(const Vector& pt, const Vector& norm, float interpen) {
		contacts.push_back({ pt, norm, interpen });
	}
};