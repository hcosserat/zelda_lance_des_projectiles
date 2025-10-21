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
    const Vector relative_acc = other.centerParticle.acc - centerParticle.acc;
    const Vector gravity_acc = Vector{0, 9.8, 0};

    const float relative_acc_squared = relative_acc.normSquared();
    const float gravity_acc_squared = gravity_acc.normSquared();

    if (fabs(relative_acc_squared - gravity_acc_squared) <= 1e-6) {
        // La vitesse relative est inférieure ou égale à la vitesse gravitationnelle
        collisionResult.collisionType = RestingContactsCollision;
    } else {
        collisionResult.collisionType = InterpenetrationCollision;
    }

    return collisionResult;
}
