#include "Actor.h"

CollisionResult Actor::collidesWith(const Actor &other, const float frame_length) {
    CollisionResult collisionResult = _collidesWith(other);

    if (not collisionResult.collides) return collisionResult;

    const bool this_have_infinite_mass = centerParticle.inverseMass == 0.0f;
    const bool other_have_infinite_mass = other.centerParticle.inverseMass == 0.0f;

    if (this_have_infinite_mass and other_have_infinite_mass) {
        // Wtf ?
        // Deux objets statiques, pas de collision
        return {false};
    }

    if ((not this_have_infinite_mass) and (not other_have_infinite_mass)) {
        // Deux objets dynamiques
        collisionResult.collisionType = InterpenetrationCollision;
        return collisionResult;
    }

    // Un objet statique et un objet dynamique
    const Vector relative_velocity = other.centerParticle.vel - centerParticle.vel;
    const float velocity_along_vertical = relative_velocity.y;
    constexpr float gravity_acc_along_vertical = 9.8;

    if (velocity_along_vertical < gravity_acc_along_vertical * frame_length) {
        // La seule force verticale est la force graviationnelle
        collisionResult.collisionType = RestingContactsCollision;
    } else {
        collisionResult.collisionType = InterpenetrationCollision;
    }

    return collisionResult;
}
