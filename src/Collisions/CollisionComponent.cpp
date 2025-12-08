#include "CollisionComponent.h"
#include "ofMain.h"

CollisionComponent::CollisionComponent(float worldSize)
    : tree(std::make_unique<Octree>(Vector(0, 0, 0), worldSize))
    , debugDrawEnabled(true) {
}

void CollisionComponent::updateSpatialStructure(const std::vector<std::unique_ptr<RigidBody>>& bodies) {
    tree->clear();
    for (const auto& body : bodies) {
        tree->insert(body.get());
    }
}

bool CollisionComponent::broadPhaseCheck(RigidBody* a, RigidBody* b) const {
    // Bounding sphere intersection test
    float radiusSum = a->boundingRadius() + b->boundingRadius();
    float distSquared = a->center.distanceSquared(b->center);
    return distSquared <= radiusSum * radiusSum;
}

void CollisionComponent::detectCollisions(const std::vector<std::unique_ptr<RigidBody>>& bodies) {
    // Clear previous frame's collisions
    collisions.clear();

    // Step 1: Get collision partitions from octree (spatial partitioning)
    std::vector<std::vector<RigidBody*>> partitions;
    tree->getCollisionPartitions(partitions);

    // Step 2: For each partition, check pairs
    for (const auto& partition : partitions) {
        size_t count = partition.size();
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                RigidBody* bodyA = partition[i];
                RigidBody* bodyB = partition[j];

                // Step 3: Broad phase - bounding sphere check
                if (!broadPhaseCheck(bodyA, bodyB)) {
                    continue;
                }

                // Step 4: Narrow phase - precise collision detection
                CollisionData collision = NarrowPhase::CheckCollision(bodyA, bodyB);

                if (collision.hasCollision()) {
                    // Store collision for resolution
                    collisions.push_back(collision);
                }
            }
        }
    }
}

// ============== Debug Rendering ==============

void CollisionComponent::drawDebug() const {
    if (debugDrawEnabled) {
        drawOctree(tree.get());
    }
}

void CollisionComponent::drawOctree(const Octree* node) const {
    if (!node) return;

    Vector c = node->center;
    float h = node->halfSize;

    ofNoFill();
    ofSetColor(255, 243, 0);
    ofDrawBox(c.x, c.y, c.z, h * 2, h * 2, h * 2);

    for (const auto& child : node->children) {
        if (child) {
            drawOctree(child.get());
        }
    }
}

