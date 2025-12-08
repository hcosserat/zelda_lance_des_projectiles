#include "CollisionComponent.h"
#include "ofMain.h"

CollisionComponent::CollisionComponent(float worldSize)
    : tree(std::make_unique<Octree>(Vector(0, 0, 0), worldSize))
      , debugDrawEnabled(true)
      , elasticity(0.1f) {
}

void CollisionComponent::updateSpatialStructure(const std::vector<std::unique_ptr<RigidBody> > &bodies) {
    tree->clear();
    for (const auto &body: bodies) {
        tree->insert(body.get());
    }
}

bool CollisionComponent::broadPhaseCheck(RigidBody *a, RigidBody *b) const {
    // Bounding sphere intersection test
    float radiusSum = a->boundingRadius() + b->boundingRadius();
    float distSquared = a->center.distanceSquared(b->center);
    return distSquared <= radiusSum * radiusSum;
}

void CollisionComponent::detectCollisions(const std::vector<std::unique_ptr<RigidBody> > &bodies) {
    // Clear previous frame's collisions
    collisions.clear();

    // Step 1: Get collision partitions from octree (spatial partitioning)
    std::vector<std::vector<RigidBody *> > partitions;
    tree->getCollisionPartitions(partitions);

    // Step 2: For each partition, check pairs
    for (const auto &partition: partitions) {
        size_t count = partition.size();
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                RigidBody *bodyA = partition[i];
                RigidBody *bodyB = partition[j];

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

// ============== Collision Resolution ==============

void CollisionComponent::resolveCollisions(float dt) {
    // First, correct all positions to resolve interpenetration
    for (const auto &collision: collisions) {
        for (const auto &contact: collision.contacts) {
            correctPositions(contact, collision.body1, collision.body2);
        }
    }

    // Then, resolve impulses based on corrected state
    for (const auto &collision: collisions) {
        for (const auto &contact: collision.contacts) {
            resolveContact(contact, collision.body1, collision.body2, dt);
        }
    }
}

void CollisionComponent::resolveContact(const Contact &contact, RigidBody *body1, RigidBody *body2, float dt) {
    // Skip if both bodies are static (infinite mass)
    if (body1->invMass == 0 && body2->invMass == 0) {
        return;
    }

    // Contact point relative to each body's center of mass
    Vector r1 = contact.point - body1->massCenter;
    Vector r2 = contact.point - body2->massCenter;

    // Velocity at contact point for each body: v + ω × r
    Vector v1 = body1->vel + body1->angularVel.cross(r1);
    Vector v2 = body2->vel + body2->angularVel.cross(r2);

    // Relative velocity at contact point
    Vector relativeVel = v1 - v2;

    // Relative velocity along the collision normal
    float velAlongNormal = relativeVel.dot(contact.normal);

    // Calculate the change in velocity due to acceleration in this frame
    Vector accInducedVel = body1->acc - body2->acc;
    float accInducedVelAlongNormal = accInducedVel.dot(contact.normal) * dt;

    // Don't resolve if velocities are separating
    if (velAlongNormal > 0 && accInducedVelAlongNormal >= 0) {
        return;
    }

    // Compute the impulse scalar
    // j = -(1 + e) * v_rel · n / (1/m1 + 1/m2 + ((I1^-1 * (r1 × n)) × r1 + (I2^-1 * (r2 × n)) × r2) · n)

    Vector r1CrossN = r1.cross(contact.normal);
    Vector r2CrossN = r2.cross(contact.normal);

    // Transform through inverse inertia tensors
    Vector term1 = body1->invInertiaTensor * r1CrossN;
    Vector term2 = body2->invInertiaTensor * r2CrossN;

    // Cross with r again
    Vector angularTerm1 = term1.cross(r1);
    Vector angularTerm2 = term2.cross(r2);

    float angularComponent = (angularTerm1 + angularTerm2).dot(contact.normal);
    float inverseMassSum = body1->invMass + body2->invMass;

    float j = -(1.0f + elasticity) * velAlongNormal;
    j /= (inverseMassSum + angularComponent);

    // Apply impulse
    Vector impulse = contact.normal * j;

    // Update linear velocities
    body1->vel = body1->vel + impulse * body1->invMass;
    body2->vel = body2->vel - impulse * body2->invMass;

    // Update angular velocities
    body1->angularVel = body1->angularVel + body1->invInertiaTensor * r1.cross(impulse);
    body2->angularVel = body2->angularVel - body2->invInertiaTensor * r2.cross(impulse);
}

void CollisionComponent::correctPositions(const Contact &contact, RigidBody *body1, RigidBody *body2) {
    // Skip if both bodies are static
    if (body1->invMass == 0 && body2->invMass == 0) {
        return;
    }

    float totalInvMass = body1->invMass + body2->invMass;
    if (totalInvMass == 0) return;

    float correctionMagnitude = std::max(contact.interpenetration, 0.0f) / totalInvMass;
    Vector correction = contact.normal * correctionMagnitude;

    // Move bodies apart proportional to their inverse masses
    body1->center = body1->center + correction * body1->invMass;
    body1->massCenter = body1->massCenter + correction * body1->invMass;

    body2->center = body2->center - correction * body2->invMass;
    body2->massCenter = body2->massCenter - correction * body2->invMass;
}

// ============== Debug Rendering ==============

void CollisionComponent::drawDebug() const {
    if (debugDrawEnabled) {
        drawOctree(tree.get());
    }
}

void CollisionComponent::drawOctree(const Octree *node) const {
    if (!node) return;

    Vector c = node->center;
    float h = node->halfSize;

    ofNoFill();
    ofSetColor(255, 243, 0);
    ofDrawBox(c.x, c.y, c.z, h * 2, h * 2, h * 2);

    for (const auto &child: node->children) {
        if (child) {
            drawOctree(child.get());
        }
    }
}
