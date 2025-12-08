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

    // Resolve all detected collisions using impulse-based resolution
    void resolveCollisions(float dt);

    // Get detected collisions from last frame
    const std::vector<CollisionData> &getCollisions() const { return collisions; }

    // Check if any collisions occurred
    bool hasCollisions() const { return !collisions.empty(); }

    // Configuration
    void setElasticity(float e) { elasticity = e; }
    float getElasticity() const { return elasticity; }

    // Debug rendering
    void drawDebug() const;
    void setDebugDraw(bool enabled) { debugDrawEnabled = enabled; }
    bool isDebugDrawEnabled() const { return debugDrawEnabled; }

    // Access octree for external queries
    const Octree *getOctree() const { return tree.get(); }

private:
    std::unique_ptr<Octree> tree;
    std::vector<CollisionData> collisions;
    bool debugDrawEnabled;
    float elasticity; // Coefficient of restitution (0 = inelastic, 1 = perfectly elastic)

    // Broad phase: bounding sphere intersection
    bool broadPhaseCheck(RigidBody *a, RigidBody *b) const;

    // Impulse-based collision resolution for a single contact
    void resolveContact(const Contact &contact, RigidBody *body1, RigidBody *body2, float dt);

    // Positional correction to prevent sinking
    void correctPositions(const Contact &contact, RigidBody *body1, RigidBody *body2);

    // Debug drawing helpers
    void drawOctree(const Octree *node) const;
};
