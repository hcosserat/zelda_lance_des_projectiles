#include "Rect.h"
#include "Circle.h"


Shape Rect::getShape() const {
    return RectShape;
}


CollisionResult Rect::collidesWithRect(const Rect &other) const {
    const Vector d = other.centerParticle.pos - centerParticle.pos;

    const std::array axes = {axisU, axisV, other.axisU, other.axisV};
    constexpr float eps = 1e-8f;

    auto projectRadius = [](const float a, const float b, const Vector &u, const Vector &v, const Vector &L) -> float {
        return a * std::fabs(u.dot(L)) + b * std::fabs(v.dot(L));
    };

    float minOverlap = std::numeric_limits<float>::infinity();
    Vector bestAxisRaw{0.0f, 0.0f, 0.0f};

    for (const Vector &L: axes) {
        if (L.normSquared() <= eps) continue;

        const float r1 = projectRadius(halfA, halfB, axisU, axisV, L);
        const float r2 = projectRadius(other.halfA, other.halfB, other.axisU, other.axisV, L);
        const float t = d.dot(L);
        const float overlap = r1 + r2 - std::fabs(t);

        if (overlap <= 0.0f) {
            return {false, Vector{0.0f, 0.0f, 0.0f}};
        }

        if (overlap < minOverlap) {
            minOverlap = overlap;
            bestAxisRaw = (t < 0.0f) ? -L : L; // orient from this rect toward the other
        }
    }

    if (bestAxisRaw.normSquared() <= eps) {
        return {false, Vector{0.0f, 0.0f, 0.0f}};
    }

    return {true, bestAxisRaw.normalized()};
}

CollisionResult Rect::collidesWith(const Actor &other) {
    switch (other.getShape()) {
        case CircleShape: {
            const CollisionResult collision_result = dynamic_cast<const Circle &>(other).collidesWithRect(*this);
            return {collision_result.collides, -collision_result.normalVector};
        }
        case RectShape:
            return collidesWithRect(dynamic_cast<const Rect &>(other));
        default:
            return {false, Vector{0, 0, 0}};
    }
}
