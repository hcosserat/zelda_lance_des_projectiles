#include "CollisionComponent.h"
#include "ofMain.h"
#include <unordered_set>

// Déduplication de paires
namespace {
    struct PairKey {
        RigidBody *a;
        RigidBody *b;
        bool operator==(const PairKey &other) const { return a == other.a && b == other.b; }
    };

    struct PairKeyHash {
        size_t operator()(const PairKey &k) const {
            return std::hash<RigidBody *>()(k.a) ^ (std::hash<RigidBody *>()(k.b) << 1);
        }
    };
}

CollisionComponent::CollisionComponent(float worldSize)
    : tree(std::make_unique<Octree>(Vector(30, 30, 30), worldSize))
      , debugDrawEnabled(true)
      , elasticity(0.6f)
      , damping(0.95f) {
}

void CollisionComponent::updateSpatialStructure(const std::vector<std::unique_ptr<RigidBody> > &bodies) {
    tree->clear();
    currentBodies.clear();
    currentBodies.reserve(bodies.size());
    for (const auto &body: bodies) {
        tree->insert(body.get());
        currentBodies.push_back(body.get());
    }
}

bool CollisionComponent::broadPhaseCheck(RigidBody *a, RigidBody *b) const {
    // Test d'intersection de sphères englobantes
    float radiusSum = a->boundingRadius() + b->boundingRadius();
    float distSquared = a->center.distanceSquared(b->center);
    return distSquared <= radiusSum * radiusSum;
}

void CollisionComponent::detectCollisions() {
    // Effacer les collisions de la frame précédente
    collisions.clear();

    // Neighbor-query broad phase
    // Construire une liste de plans (rayon infini) pour les inclure avec tout le monde
    std::vector<RigidBody *> planes;
    planes.reserve(currentBodies.size());
    for (RigidBody *b: currentBodies) {
        if (std::isinf(b->shape->boundingRadius())) {
            planes.push_back(b);
        }
    }

    // Déduplication: paire triée de pointeurs -> hash
    std::unordered_set<PairKey, PairKeyHash> testedPairs;
    testedPairs.reserve(currentBodies.size() * 4);

    const float neighborMargin = 0.25f; // petite marge pour les limites de noeuds

    // Pour chaque corps, interroger ses voisins à partir de l'octree
    for (RigidBody *bodyA: currentBodies) {
        // Rayon de requête: rayon englobant + marge
        float queryRadius = bodyA->boundingRadius();
        if (!std::isinf(queryRadius)) {
            queryRadius += neighborMargin;
        } else {
            // Pour un plan (rayon infini), on ne fait pas de query; on traitera via liste planes
            continue;
        }

        std::vector<RigidBody *> neighbors;
        neighbors.reserve(8);
        tree->query(bodyA->center, queryRadius, neighbors);

        // Ajouter les plans comme voisins potentiels
        for (RigidBody *planeBody: planes) {
            neighbors.push_back(planeBody);
        }

        // Tester les paires (A, B) avec déduplication
        for (RigidBody *bodyB: neighbors) {
            if (bodyA == bodyB) continue;

            RigidBody *minPtr = bodyA < bodyB ? bodyA : bodyB;
            RigidBody *maxPtr = bodyA < bodyB ? bodyB : bodyA;
            PairKey key{minPtr, maxPtr};
            if (testedPairs.find(key) != testedPairs.end()) {
                continue; // déjà testé
            }
            testedPairs.insert(key);

            if (!broadPhaseCheck(bodyA, bodyB)) {
                continue;
            }

            CollisionData collision = NarrowPhase::CheckCollision(bodyA, bodyB);
            if (collision.hasCollision()) {
                collisions.push_back(collision);
            }
        }
    }
}

// ============== Résolution des collisions ==============

void CollisionComponent::resolveCollisions(float dt) {
	// Correction positionnelle pour tous les contacts
	for (const auto & collision : collisions) {
		for (const auto & contact : collision.contacts) {
			correctPositions(contact, collision.body1, collision.body2);
		}
	}

	// On garde une trace des corps qui ont eu au moins un contact
	std::unordered_set<RigidBody *> touched;
	touched.reserve(collisions.size() * 2);

	// Résolution par impulsions (sans damping ici)
	for (const auto & collision : collisions) {
		for (const auto & contact : collision.contacts) {
			resolveContact(contact, collision.body1, collision.body2, dt);

			// marquer les deux corps comme "touchés" (s'ils existent)
			if (collision.body1 && collision.body1->invMass > 0) touched.insert(collision.body1);
			if (collision.body2 && collision.body2->invMass > 0) touched.insert(collision.body2);
		}
	}

	// Appliquer l'amortissement **UNE** seule fois par corps touché cette frame
	for (RigidBody * body : touched) {
		if (body->invMass > 0) {
			body->vel = body->vel * damping;
			body->angularVel = body->angularVel * damping;
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
