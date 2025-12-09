#include "CollisionComponent.h"
#include "ofMain.h"

CollisionComponent::CollisionComponent(float worldSize)
    : tree(std::make_unique<Octree>(Vector(0, 0, 0), worldSize))
      , debugDrawEnabled(true)
      , elasticity(0.6f)
      , damping(0.8f) {
}

void CollisionComponent::updateSpatialStructure(const std::vector<std::unique_ptr<RigidBody> > &bodies) {
    tree->clear();
    for (const auto &body: bodies) {
        tree->insert(body.get());
    }
}

bool CollisionComponent::broadPhaseCheck(RigidBody *a, RigidBody *b) const {
    // Test d'intersection de sphères englobantes
    float radiusSum = a->boundingRadius() + b->boundingRadius();
    float distSquared = a->center.distanceSquared(b->center);
    return distSquared <= radiusSum * radiusSum;
}

void CollisionComponent::detectCollisions(const std::vector<std::unique_ptr<RigidBody> > &bodies) {
    // Effacer les collisions de la frame précédent
    collisions.clear();

    // Étape 1 : partitions de collision via l'octree
    std::vector<std::vector<RigidBody *> > partitions;
    tree->getCollisionPartitions(partitions);

    // Étape 2 : pour chaque partition, tester les paires
    for (const auto & partition : partitions) {
        size_t count = partition.size();

        if (count == 0) continue; // Skip empty partitions

        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                RigidBody *bodyA = partition[i];
                RigidBody *bodyB = partition[j];

                // Étape 3 : broad phase
                if (!broadPhaseCheck(bodyA, bodyB)) {
                    continue;
                }

                // Étape 4 : narrow phase
                CollisionData collision = NarrowPhase::CheckCollision(bodyA, bodyB);

                if (collision.hasCollision()) {
                    // Stocker pour résolution
                    collisions.push_back(collision);
                }
            }
        }
    }
}

// ============== Résolution des collisions ==============

void CollisionComponent::resolveCollisions(float dt) {
    // Correction positionnelle pour tous les contacts
    for (const auto &collision: collisions) {
        for (const auto &contact: collision.contacts) {
            correctPositions(contact, collision.body1, collision.body2);
        }
    }

    // Résolution par impulsions
    for (const auto &collision: collisions) {
        for (const auto &contact: collision.contacts) {
            resolveContact(contact, collision.body1, collision.body2, dt);
        }
    }
}

void CollisionComponent::resolveContact(const Contact &contact, RigidBody *body1, RigidBody *body2, float dt) {
    // Ignorer si les deux corps sont statiques
    if (body1->invMass == 0 && body2->invMass == 0) {
        return;
    }

    // Point relatif aux centres de masse
    Vector r1 = contact.point - body1->massCenter;
    Vector r2 = contact.point - body2->massCenter;

    // Vitesse au point de contact : v + ω × r
    Vector v1 = body1->vel + body1->angularVel.cross(r1);
    Vector v2 = body2->vel + body2->angularVel.cross(r2);

    // Vitesse relative
    Vector relativeVel = v1 - v2;

    // Composante le long de la normale
    float velAlongNormal = relativeVel.dot(contact.normal);

    // Variation due aux accélérations
    Vector accInducedVel = body1->acc - body2->acc;
    float accInducedVelAlongNormal = accInducedVel.dot(contact.normal) * dt;

    // Ne pas résoudre si les vitesses s'écartent
    if (velAlongNormal > 0 && accInducedVelAlongNormal >= 0) {
        return;
    }

    // Calcul de l'impulsion
    Vector r1CrossN = r1.cross(contact.normal);
    Vector r2CrossN = r2.cross(contact.normal);

    Vector term1 = body1->invInertiaTensor * r1CrossN;
    Vector term2 = body2->invInertiaTensor * r2CrossN;

    Vector angularTerm1 = term1.cross(r1);
    Vector angularTerm2 = term2.cross(r2);

    float angularComponent = (angularTerm1 + angularTerm2).dot(contact.normal);
    float inverseMassSum = body1->invMass + body2->invMass;

    float j = -(1.0f + elasticity) * velAlongNormal;
    j /= (inverseMassSum + angularComponent);

    // Appliquer l'impulsion
    Vector impulse = contact.normal * j;

    // Mettre à jour vitesses linéaires
    body1->vel = body1->vel + impulse * body1->invMass;
    body2->vel = body2->vel - impulse * body2->invMass;

    // Mettre à jour vitesses angulaires
    body1->angularVel = body1->angularVel + body1->invInertiaTensor * r1.cross(impulse);
    body2->angularVel = body2->angularVel - body2->invInertiaTensor * r2.cross(impulse);

    // Amortissement
    if (body1->invMass > 0) {
        body1->vel = body1->vel * damping;
        body1->angularVel = body1->angularVel * damping;
    }
    if (body2->invMass > 0) {
        body2->vel = body2->vel * damping;
        body2->angularVel = body2->angularVel * damping;
    }
}

void CollisionComponent::correctPositions(const Contact &contact, RigidBody *body1, RigidBody *body2) {
    // Ignorer si les deux sont statiques
    if (body1->invMass == 0 && body2->invMass == 0) {
        return;
    }

    float totalInvMass = body1->invMass + body2->invMass;
    if (totalInvMass == 0) return;

    float correctionMagnitude = std::max(contact.interpenetration, 0.0f) / totalInvMass;
    Vector correction = contact.normal * correctionMagnitude;

    // Déplacer proportionnellement à la masse inverse
    body1->center = body1->center + correction * body1->invMass;
    body1->massCenter = body1->massCenter + correction * body1->invMass;

    body2->center = body2->center - correction * body2->invMass;
    body2->massCenter = body2->massCenter - correction * body2->invMass;
}

// ============== Rendu debug ==============

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
