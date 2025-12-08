#pragma once

#include "Octree.h"
#include "NarrowPhase.h"
#include "CollisionData.h"
#include "../Actors/RigidBody.h"
#include <vector>
#include <memory>

class CollisionComponent {
public:
    explicit CollisionComponent(float worldSize = 200.0f);

    // Rebuild spatial structure with current bodies
    void updateSpatialStructure(const std::vector<std::unique_ptr<RigidBody> > &bodies);

    // Run full collision detection pipeline
    void detectCollisions(const std::vector<std::unique_ptr<RigidBody> > &bodies);

    // Get detected collisions from last frame
    const std::vector<CollisionData> &getCollisions() const { return collisions; }

    // Check if any collisions occurred
    bool hasCollisions() const { return !collisions.empty(); }

    // Debug rendering
    void drawDebug() const;

    void setDebugDraw(bool enabled) { debugDrawEnabled = enabled; }

    // Access octree for external queries
    const Octree *getOctree() const { return tree.get(); }

private:
    std::unique_ptr<Octree> tree;
    std::vector<CollisionData> collisions;
    bool debugDrawEnabled;

    // Broad phase: bounding sphere intersection
    bool broadPhaseCheck(RigidBody *a, RigidBody *b) const;

    // Debug drawing helpers
    void drawOctree(const Octree *node) const;
};

