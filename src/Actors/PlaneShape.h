#pragma once

#include "ShapeComponent.h"
#include "../Maths/Vector.h"
#include <limits>

class PlaneShape : public ShapeComponent {
private:
    Vector point;
    Vector normal;

public:
    PlaneShape(const Vector& p, const Vector& n)
        : point(p), normal(n) {
        this->normal.normalize();
    }

    ShapeType type() const override { return PLANE; }

    float boundingRadius() const override {
        // Planes are infinite, return a large value or infinity
        return std::numeric_limits<float>::infinity();
    }

    void drawShape() const override {
        // Planes don't render themselves in this implementation
        // Could draw a debug quad here if needed
    }

    const Vector& getPoint() const { return point; }
    const Vector& getNormal() const { return normal; }
};
