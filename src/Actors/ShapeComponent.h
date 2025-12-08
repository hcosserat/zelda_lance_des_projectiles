#pragma once

#include "../Maths/Vector.h"
#include <vector>

enum ShapeType {
    BOX,
    PLANE
};

class RigidBody; // Déclaration anticipée

class ShapeComponent {
public:
    virtual ~ShapeComponent() = default;

    virtual ShapeType type() const = 0;

    virtual float boundingRadius() const = 0;

    virtual void drawShape() const = 0;

    // Pour la détection de collision (par défaut aucun sommet)
    virtual std::vector<Vector> getVerticesWorld(const RigidBody &body) const { return {}; }
};
