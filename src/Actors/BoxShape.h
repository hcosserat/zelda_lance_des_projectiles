#pragma once

#include "ShapeComponent.h"
#include "../Maths/Vector.h"
#include "../Maths/Matrix3.h"
#include "ofMain.h"
#include <vector>
#include <cmath>

class BoxShape : public ShapeComponent {
    Vector dimensions;
    float cachedBoundingRadius;

public:
    BoxShape(const Vector &dims)
        : dimensions(dims) {
        // Compute bounding radius from half-extents
        float hx = dims.x / 2.0f;
        float hy = dims.y / 2.0f;
        float hz = dims.z / 2.0f;
        cachedBoundingRadius = sqrt(hx * hx + hy * hy + hz * hz);
    }

    ShapeType type() const override { return BOX; }

    float boundingRadius() const override {
        return cachedBoundingRadius;
    }

    void drawShape() const override {
        ofNoFill();
        ofSetColor(200, 50, 50);
        ofDrawBox(0, 0, 0, dimensions.x, dimensions.y, dimensions.z);
        ofFill();
    }

    const Vector &getDimensions() const { return dimensions; }

    std::vector<Vector> getVerticesWorld(const RigidBody &body) const override;

    // Compute inertia tensor for a box with given mass
    static Matrix3 computeInertiaTensor(const Vector &dims, float mass) {
        float w = dims.x;
        float h = dims.y;
        float d = dims.z;
        float k = (1.0f / 12.0f) * mass;
        Matrix3 tensor = Matrix3::Identity();
        tensor(0, 0) = k * (h * h + d * d);
        tensor(1, 1) = k * (w * w + d * d);
        tensor(2, 2) = k * (w * w + h * h);
        return tensor;
    }
};
