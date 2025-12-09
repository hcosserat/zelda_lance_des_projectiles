#pragma once
#include "../Maths/Vector.h"
#include "../Actors/RigidBody.h"

class Octree {
public:
	Octree(const Vector &center, float halfSize,
	       int depth = 0, int maxDepth = 3, int maxElements = 3);

	void insert(RigidBody *body);

	void query(const Vector &point, float radius,
	           std::vector<RigidBody *> &result);

	void clear();

	std::unique_ptr<Octree> children[8];
	std::vector<RigidBody *> elements;

	Vector center;
	float halfSize;

	int depth;
	int maxDepth;
	int maxElements;

	// Test si un rigidbody est dans le cube
	bool contains(const RigidBody *body) const;

	// Test si une sphère intersecte le cube
	bool intersects(const Vector &point, float radius) const;

	// Division de ce noeud en 8
	void subdivide();

	// Trouver l'enfant correspondant à une position
	int getChildIndex(const Vector &pos) const;

	void getCollisionPartitions(std::vector<std::vector<RigidBody *> > &out) const;

private:
	void redistributeElements();

	void collectPartitions(std::vector<std::vector<RigidBody *> > &out) const;
};
