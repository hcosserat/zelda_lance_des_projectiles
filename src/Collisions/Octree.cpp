#include "Octree.h"

#include "../Actors/ShapeComponent.h"

// Aide: vérifier si un corps est strictement inclus dans les bornes d'un noeud
static bool containsStrict(const Vector &nodeCenter, float halfSize, const RigidBody *body) {
    float r = body->shape->boundingRadius();
    // Si rayon infini (Plane), ne peut jamais être strictement inclus
    if (std::isinf(r)) return false;

    return body->center.x - r >= nodeCenter.x - halfSize &&
           body->center.x + r <= nodeCenter.x + halfSize &&
           body->center.y - r >= nodeCenter.y - halfSize &&
           body->center.y + r <= nodeCenter.y + halfSize &&
           body->center.z - r >= nodeCenter.z - halfSize &&
           body->center.z + r <= nodeCenter.z + halfSize;
}

// ------------------------------------------------------------
// Constructeur
// ------------------------------------------------------------
Octree::Octree(const Vector &center, float halfSize,
               int depth, int maxDepth, int maxElements)
    : center(center)
      , halfSize(halfSize)
      , depth(depth)
      , maxDepth(maxDepth)
      , maxElements(maxElements) {
    for (auto &c: children)
        c = nullptr;
}

// ------------------------------------------------------------
// Test si le rigidbody est dans notre cube
// ------------------------------------------------------------
bool Octree::contains(const RigidBody *body) const {
    float r = body->shape->boundingRadius();

    return body->center.x + r >= center.x - halfSize && body->center.x - r <= center.x + halfSize && body->center.y + r
           >= center.y - halfSize && body->center.y - r <= center.y + halfSize && body->center.z + r >= center.z -
           halfSize && body->center.z - r <= center.z + halfSize;
}

// ------------------------------------------------------------
// Intersection cube / sphère
// ------------------------------------------------------------
bool Octree::intersects(const Vector &point, float radius) const {
    float x = std::max(center.x - halfSize,
                       std::min(point.x, center.x + halfSize));
    float y = std::max(center.y - halfSize,
                       std::min(point.y, center.y + halfSize));
    float z = std::max(center.z - halfSize,
                       std::min(point.z, center.z + halfSize));

    return point.distanceSquared(Vector{x, y, z}) <= radius * radius;
}

// ------------------------------------------------------------
// Subdivide
// ------------------------------------------------------------
void Octree::subdivide() {
    float h = halfSize / 2.0f;
    int nextDepth = depth + 1;

    for (int i = 0; i < 8; ++i) {
        Vector childCenter(
            center.x + h * ((i & 1) ? 1 : -1),
            center.y + h * ((i & 2) ? 1 : -1),
            center.z + h * ((i & 4) ? 1 : -1)
        );

        children[i] = std::make_unique<Octree>(
            childCenter, h, nextDepth, maxDepth, maxElements);
    }

    // Redistribuer les éléments existants
    redistributeElements();
}

// ------------------------------------------------------------
// Redistribue les éléments après subdivision
// ------------------------------------------------------------
void Octree::redistributeElements() {
    std::vector<RigidBody *> oldElements = std::move(elements);
    elements.clear();

    for (auto *body: oldElements) {
        insert(body);
    }
}

// ------------------------------------------------------------
// Trouver l'enfant selon la position
// ------------------------------------------------------------
int Octree::getChildIndex(const Vector &pos) const {
    int idx = 0;
    if (pos.x >= center.x) idx |= 1;
    if (pos.y >= center.y) idx |= 2;
    if (pos.z >= center.z) idx |= 4;
    return idx;
}

// ------------------------------------------------------------
// Insert
// ------------------------------------------------------------
void Octree::insert(RigidBody *body) {
    // Si on a des enfants
    if (children[0] != nullptr) {
        int index = getChildIndex(body->center);
        // Si l'objet rentre STRICTEMENT dans l'enfant, on le descend
        if (containsStrict(children[index]->center, children[index]->halfSize, body)) {
            children[index]->insert(body);
        } else {
            // Sinon on le garde ici
            elements.push_back(body);
        }
        return;
    }

    // Feuille : stocker
    elements.push_back(body);

    // Si trop d'éléments, subdiviser
    if (elements.size() > maxElements && depth < maxDepth) {
        subdivide();
    }
}

// ------------------------------------------------------------
// Query
// ------------------------------------------------------------
void Octree::query(const Vector &point, float radius,
                   std::vector<RigidBody *> &result) {
    if (!intersects(point, radius))
        return;

    // Tester les éléments locaux
    for (auto *e: elements) {
        float r = radius + e->shape->boundingRadius();
        if (point.distanceSquared(e->center) <= r * r)
            result.push_back(e);
    }

    // Descendre dans les enfants
    for (auto &child: children)
        if (child) child->query(point, radius, result);
}

// ------------------------------------------------------------
// Clear complet
// ------------------------------------------------------------
void Octree::clear() {
    elements.clear();
    for (auto &child: children)
        child.reset();
}

// ------------------------------------------------------------
// Get collision partitions
// ------------------------------------------------------------
void Octree::getCollisionPartitions(std::vector<std::vector<RigidBody *> > &out) const {
    out.clear();
    collectPartitions(out);
}

void Octree::collectPartitions(std::vector<std::vector<RigidBody *> > &out) const {
    // If this node has elements, add them as a partition
    if (!elements.empty()) {
        out.push_back(elements);
    }

    // Recursively collect partitions from children
    for (const auto &child: children) {
        if (child) {
            child->collectPartitions(out);
        }
    }
}
