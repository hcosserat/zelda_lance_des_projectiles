#pragma once
#include "Collisions/Octree.h" // ton chemin réel ici
#include "Actors/RigidBody.h"
#include <vector>

class World {
public:
	std::vector<RigidBody *> rigidBodies;

	explicit World();

	void update(float dt);
	void draw() const;

	void addRigidBody(RigidBody * body);

private:
	Octree * tree; // Le spatial index
	float treeHalfSize = 200.0f; // Taille du monde (à ajuster)

	void rebuildOctree();
	void drawOctree(const Octree * node) const;
};
