#pragma once

#include "Octree.h"
#include "NarrowPhase.h"
#include "CollisionData.h"
#include "../Actors/RigidBody.h"
#include <vector>
#include <memory>
#include <unordered_set>

class CollisionComponent {
public:
    explicit CollisionComponent(float worldSize = 200.0f);

    // Reconstruire la structure spatiale avec les corps actuels
    void updateSpatialStructure(const std::vector<std::unique_ptr<RigidBody> > &bodies);

    // Exécuter la pipeline complète de détection de collision
    void detectCollisions();

    // Résoudre toutes les collisions détectées (impulsions)
    void resolveCollisions(float dt);

    // Obtenir les collisions détectées
    const std::vector<CollisionData> &getCollisions() const { return collisions; }

    // Indique si des collisions ont eu lieu
    bool hasCollisions() const { return !collisions.empty(); }

    // Configuration
    void setElasticity(float e) { elasticity = e; }
    float getElasticity() const { return elasticity; }

    void setDamping(float d) { damping = d; }
    float getDamping() const { return damping; }

    // Rendu debug
    void drawDebug() const;
    void setDebugDraw(bool enabled) { debugDrawEnabled = enabled; }
    bool isDebugDrawEnabled() const { return debugDrawEnabled; }

    // Accès à l'octree pour requêtes externes
    const Octree *getOctree() const { return tree.get(); }

private:
    std::unique_ptr<Octree> tree;
    std::vector<CollisionData> collisions;
    bool debugDrawEnabled;
    float elasticity; // Coef de restitution (0 = inélastique, 1 = élastique)
    float damping; // Amortissement après collision

    // Cache: corps actuels (pointeurs bruts) pour les requêtes de voisins
    std::vector<RigidBody*> currentBodies;

    // Broad phase : test sphères englobantes
    bool broadPhaseCheck(RigidBody *a, RigidBody *b) const;

    // Résolution par impulsion pour un contact
    void resolveContact(const Contact &contact, RigidBody *body1, RigidBody *body2, float dt);

    // Correction positionnelle pour éviter le sinking
    void correctPositions(const Contact &contact, RigidBody *body1, RigidBody *body2);

    // Aides dessin debug
    void drawOctree(const Octree *node) const;
};
