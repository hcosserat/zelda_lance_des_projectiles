#include "Circle.h"
#include "Rect.h"

Shape Circle::getShape() const {
    return CircleShape;
}


CollisionResult Circle::collidesWithCircle(const Circle &other) const {
    const auto dist_squared = centerParticle.pos.distanceSquared(other.centerParticle.pos);
    const auto radii_sum = radius + other.radius;

    if (dist_squared <= radii_sum * radii_sum) {
        const float dist = std::sqrt(dist_squared);
        const float penetration = radii_sum - dist;
        return {true, (other.centerParticle.pos - centerParticle.pos).normalized(), penetration};
    }

    return {false};
}

CollisionResult Circle::collidesWithRect(const Rect &rect) const {
    // Circle center relative to rect center, expressed in rect basis
    const Vector d = centerParticle.pos - rect.centerParticle.pos;
    const float x = d.dot(rect.axisU);
    const float y = d.dot(rect.axisV);

    // Closest point on the rectangle in local coords
    const float cx = std::clamp(x, -rect.halfA, rect.halfA);
    const float cy = std::clamp(y, -rect.halfB, rect.halfB);

    // Check if circle center is inside rectangle
    const bool insideX = (x >= -rect.halfA && x <= rect.halfA);
    const bool insideY = (y >= -rect.halfB && y <= rect.halfB);
    const bool centerInside = insideX && insideY;

    if (centerInside) {
        // Find shortest distance to each edge
        const float distToRight = rect.halfA - x;
        const float distToLeft = rect.halfA + x;
        const float distToTop = rect.halfB - y;
        const float distToBottom = rect.halfB + y;

        const float minDist = std::min({distToRight, distToLeft, distToTop, distToBottom});
        const float penetration = radius + minDist;

        // Determine normal based on closest edge
        Vector normal;
        if (minDist == distToRight) {
            normal = rect.axisU;
        } else if (minDist == distToLeft) {
            normal = -rect.axisU;
        } else if (minDist == distToTop) {
            normal = rect.axisV;
        } else {
            normal = -rect.axisV;
        }

        return {true, normal, penetration};
    }

    // Circle center outside: use closest point approach
    const Vector closest = rect.centerParticle.pos + cx * rect.axisU + cy * rect.axisV;
    const Vector delta = centerParticle.pos - closest;
    const float dist2 = delta.normSquared();

    if (dist2 > radius * radius) {
        return {false};
    }

    return {
        true,
        delta.normalized(),
        radius - std::sqrt(dist2)
    };
}


CollisionResult Circle::_collidesWith(const Actor &other) {
    switch (other.getShape()) {
        case CircleShape:
            return collidesWithCircle(dynamic_cast<const Circle &>(other));
        case RectShape:
            return collidesWithRect(dynamic_cast<const Rect &>(other));
        default: {
            std::cout << "Collision non gérée dans Circle :( C'est quoi un " << other.getShape() << " ?" << std::endl;
            return {false};
        }
    }
}
