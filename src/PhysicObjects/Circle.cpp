#include "Circle.h"
#include "Rect.h"

Shape Circle::getShape() const {
    return CircleShape;
}


CollisionResult Circle::collidesWithCircle(const Circle &other) const {
    const auto dist_squared = centerParticle.pos.distanceSquared(other.centerParticle.pos);
    const auto radii_sum = radius + other.radius;

    if (dist_squared <= radii_sum * radii_sum) {
        return {true, (other.centerParticle.pos - centerParticle.pos).normalized()};
    }

    return {false, Vector{0, 0, 0}};
}

CollisionResult Circle::collidesWithRect(const Rect &rect) const {
    // Circle center relative to rect center, expressed in rect basis
    const Vector d = centerParticle.pos - rect.centerParticle.pos;
    const float x = d.dot(rect.axisU);
    const float y = d.dot(rect.axisV);

    // Closest point on the rectangle in local coords
    float cx = x;
    if (cx < -rect.halfA) cx = -rect.halfA;
    else if (cx > rect.halfA) cx = rect.halfA;
    float cy = y;
    if (cy < -rect.halfB) cy = -rect.halfB;
    else if (cy > rect.halfB) cy = rect.halfB;

    // Closest point in world coords
    const Vector closest = rect.centerParticle.pos + cx * rect.axisU + cy * rect.axisV;

    // Vector from closest point to circle center
    const Vector delta = centerParticle.pos - closest;
    const float dist2 = delta.normSquared();

    if (dist2 > radius * radius) {
        return {false, Vector{0, 0, 0}};
    }

    // Compute collision normal pointing from circle toward the rectangle
    Vector normal;
    if (dist2 > 0.0f) {
        // Outside : from circle center to closest point on rect
        normal = (closest - centerParticle.pos).normalized();
    } else {
        // Inside : push toward nearest side
        const float dx = rect.halfA - std::fabs(x);
        const float dy = rect.halfB - std::fabs(y);
        if (dx < dy) {
            const float s = (x >= 0.0f) ? 1.0f : -1.0f;
            normal = s * rect.axisU;
        } else {
            const float s = (y >= 0.0f) ? 1.0f : -1.0f;
            normal = s * rect.axisV;
        }
    }

    return {true, normal};
}

CollisionResult Circle::collidesWith(const Actor &other) {
    switch (other.getShape()) {
        case CircleShape:
            return collidesWithCircle(dynamic_cast<const Circle &>(other));
        case RectShape:
            return collidesWithRect(dynamic_cast<const Rect &>(other));
        default:{
            std::cout << "Collision non gérée dans Circle :( C'est quoi un " << other.getShape() << " ?" << std::endl;
            return {false, Vector{0, 0, 0}
            };
    }
}
