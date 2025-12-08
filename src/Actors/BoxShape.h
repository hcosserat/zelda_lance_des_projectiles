#pragma once

#include "ShapeComponent.h"
#include "../Maths/Vector.h"
#include "ofMain.h"
#include <vector>
#include <cmath>

class BoxShape : public ShapeComponent {
    Vector dimensions;
    float cachedBoundingRadius;

public:
    BoxShape(const Vector& dims)
        : dimensions(dims) {
        // Compute bounding radius from half-extents
        float hx = dims.x / 2.0f;
        float hy = dims.y / 2.0f;
        float hz = dims.z / 2.0f;
        cachedBoundingRadius = sqrt(hx*hx + hy*hy + hz*hz);
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

    const Vector& getDimensions() const { return dimensions; }

    std::vector<Vector> getVerticesWorld(const RigidBody& body) const override;
};
