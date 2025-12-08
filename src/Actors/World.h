#pragma once
#include "Collisions/Octree.h" // ton chemin r�el ici
#include "Actors/RigidBody.h"
#include <vector>

class World {
public:
	std::vector<std::unique_ptr<RigidBody>> rigidBodies;

	explicit World();

	void update(float dt);

	void draw() const;

	void addRigidBody(std::unique_ptr<RigidBody> body);

private:
	Octree *tree; // Le spatial index
	float treeHalfSize = 200.0f; // Taille du monde (� ajuster)

	void rebuildOctree();

	void drawOctree(const Octree *node) const;
};
