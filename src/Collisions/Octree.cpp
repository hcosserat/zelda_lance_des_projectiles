#include "Octree.h"

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
    float r = body->boundingRadius;

    return body->center.x + r >= center.x - halfSize && body->center.x - r <= center.x + halfSize && body->center.y + r
           >= center.y - halfSize && body->center.y - r <= center.y + halfSize && body->center.z + r >= center.z -
           halfSize && body->center.z - r <= center.z + halfSize;
}

// ------------------------------------------------------------
// Intersection cube / sph�re
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
        Vector childCenter (
            center.x + h * ((i & 1) ? 1 : -1),
            center.y + h * ((i & 2) ? 1 : -1),
            center.z + h * ((i & 4) ? 1 : -1)
        );

        children[i] = std::make_unique<Octree>(
            childCenter, h, nextDepth, maxDepth, maxElements);
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
    if (!contains(body))
        return;

    // Node feuille avec place disponible
    if (depth == maxDepth || (elements.size() < maxElements && children[0] == nullptr)) {
        elements.push_back(body);
        return;
    }

    // Si pas encore subdivis�
    if (children[0] == nullptr)
        subdivide();

    // Ins�rer dans l�enfant correspondant
    int idx = getChildIndex(body->center);
    children[idx]->insert(body);
}

// ------------------------------------------------------------
// Query
// ------------------------------------------------------------
void Octree::query(const Vector &point, float radius,
                   std::vector<RigidBody *> &result) {
    if (!intersects(point, radius))
        return;

    // Tester les �l�ments locaux
    for (auto *e: elements) {
        float r = radius + e->boundingRadius;
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
