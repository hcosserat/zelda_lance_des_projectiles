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
    const Vector gravity_acc = Vector{0, 9.8, 0};

    if ((relative_velocity - gravity_acc * frame_length).normSquared() < 1e-6) {
        // La seule force est la force graviationnelle
        collisionResult.collisionType = RestingContactsCollision;
    } else {
        collisionResult.collisionType = InterpenetrationCollision;
    }

    return collisionResult;
}
