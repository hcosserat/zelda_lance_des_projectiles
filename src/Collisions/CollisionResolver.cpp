#include "CollisionResolver.h"
#include "../Actors/Blob.h"

// Construction des contacts
bool CollisionResolver::buildC_C(Circle &A, Circle &B, Contact &c, const float frame_duration) {
    const CollisionResult res = A.collidesWith(B, frame_duration);
    if (!res.collides) return false;

    c.a = &A;
    c.b = &B;
    c.n = res.normalVector;
    c.penetration = res.penetration;
    c.type = res.collisionType;
    return true;
}

bool CollisionResolver::buildC_R(Circle &C, Rect &R, Contact &c, const float frame_duration) {
    const CollisionResult res = C.collidesWith(R, frame_duration);
    if (!res.collides) return false;

    c.a = &C;
    c.b = &R;
    c.n = res.normalVector;
    c.penetration = res.penetration;
    c.type = res.collisionType;
    return c.penetration > 0.0f;
}

bool CollisionResolver::buildR_R(Rect &A, Rect &B, Contact &c, const float frame_duration) {
    const CollisionResult res = A.collidesWith(B, frame_duration);
    if (!res.collides) return false;

    c.a = &A;
    c.b = &B;
    c.n = res.normalVector;
    c.penetration = res.penetration;
    c.type = res.collisionType;
    return true;
}

bool CollisionResolver::buildContact(Actor &a, Actor &b, Contact &out, const float frame_duration) {
    const Shape sa = a.getShape();
    const Shape sb = b.getShape();

    if (sa == CircleShape && sb == CircleShape)
        return buildC_C(dynamic_cast<Circle &>(a), dynamic_cast<Circle &>(b), out, frame_duration);

    if (sa == CircleShape && sb == RectShape) {
        return buildC_R(dynamic_cast<Circle &>(a), dynamic_cast<Rect &>(b), out, frame_duration);
    }

    if (sa == RectShape && sb == CircleShape) {
        Contact tmp;
        if (!buildC_R(dynamic_cast<Circle &>(b), dynamic_cast<Rect &>(a), tmp, frame_duration)) return false;
        out = {&a, &b, -tmp.n, tmp.penetration, tmp.type};
        return true;
    }

    if (sa == RectShape && sb == RectShape)
        return buildR_R(dynamic_cast<Rect &>(a), dynamic_cast<Rect &>(b), out, frame_duration);

    return false;
}

void CollisionResolver::buildConstraintContacts(std::vector<Contact> &contacts, const ConstraintRegistry *registry) {
    for (const Constraint constraint: registry->getConstraints()) {
        const Vector d = constraint.b->centerParticle.pos - constraint.a->centerParticle.pos;
        const float currentLength = d.norm();

        constexpr float epsilon = 1e-6f;

        if (currentLength < epsilon) {
            // Actors are at same position
            continue;
        }

        switch (constraint.type) {
            case Rod: {
                const float diff = fabs(currentLength - constraint.length);
                if (diff < epsilon) continue;

                Vector normal = d.normalized();
                if (currentLength < constraint.length) {
                    normal = -normal;
                }

                contacts.push_back(Contact{
                    constraint.a, constraint.b, normal, diff, RodConstraint
                });
                break;
            }
            case Cable: {
                const float diff = currentLength - constraint.length;
                if (diff < epsilon) continue;
                contacts.push_back(Contact{
                    constraint.a, constraint.b, d.normalized(), diff, CableConstraint
                });
                break;
            }
        }
    }
}

void CollisionResolver::resolveContact(Contact &c) const {
    switch (c.type) {
        case RestingContactsCollision:
            resolveRestingContact(c);
            break;
        case RodConstraint:
            resolveRodConstraint(c);
            break;
        case CableConstraint:
            resolveCableConstraint(c);
            break;
        case InterpenetrationCollision:
        default:
            resolveInterpenetration(c);
            break;
    }
}

void CollisionResolver::resolveInterpenetration(Contact &c) const {
    const float invMassA = c.a->centerParticle.inverseMass;
    const float invMassB = c.b->centerParticle.inverseMass;

    // Check for infinite mass cases
    const bool aIsInfinite = invMassA == 0.0f;
    const bool bIsInfinite = invMassB == 0.0f;

    // Case: Both objects have infinite mass
    if (aIsInfinite && bIsInfinite) {
        return;
    }

    // Case: Only A has infinite mass
    if (aIsInfinite) {
        // Only move B
        c.b->centerParticle.pos -= c.penetration * c.n;

        // Apply impulse only to B (bounce off A as if it's a wall)
        const Vector vRel = c.a->centerParticle.vel - c.b->centerParticle.vel;
        const float vrn = vRel.dot(c.n);
        if (vrn < 0.0f) {
            const float j = -(1.0f + params.restitution) * vrn;
            c.b->centerParticle.vel -= j * c.n;
        }
        return;
    }

    // Case: Only B has infinite mass
    if (bIsInfinite) {
        // Only move A
        c.a->centerParticle.pos += c.penetration * c.n;

        // Apply impulse only to A (bounce off B as if it's a wall)
        const Vector vRel = c.a->centerParticle.vel - c.b->centerParticle.vel;
        const float vrn = vRel.dot(c.n);
        if (vrn < 0.0f) {
            const float j = -(1.0f + params.restitution) * vrn;
            c.a->centerParticle.vel += j * c.n;
        }
        return;
    }

    // Case: Both objects have finite mass
    // 1) Position correction
    const float aMass = 1 / c.a->centerParticle.inverseMass;
    const float bMass = 1 / c.b->centerParticle.inverseMass;

    const float inverseSumMasses = 1 / (aMass + bMass);
    const auto inverseSumMassesMultDMultN = inverseSumMasses * c.penetration * c.n;

    c.a->centerParticle.pos -= bMass * inverseSumMassesMultDMultN;
    c.b->centerParticle.pos += aMass * inverseSumMassesMultDMultN;

    // 2) Impluses
    const auto vRel = c.a->centerParticle.vel - c.b->centerParticle.vel;

    const auto sumInverseMasses = c.a->centerParticle.inverseMass + c.b->centerParticle.inverseMass;
    const auto k = vRel.dot(c.n) * (params.restitution + 1) / sumInverseMasses;

    c.a->centerParticle.vel -= k * c.n * c.a->centerParticle.inverseMass;
    c.b->centerParticle.vel += k * c.n * c.b->centerParticle.inverseMass;
}

void CollisionResolver::resolveRestingContact(Contact &c) const {
    // For resting contacts, we do position correction and no bounce
    const float invMassA = c.a->centerParticle.inverseMass;
    const float invMassB = c.b->centerParticle.inverseMass;
    const float sumInverseMasses = invMassA + invMassB;

    // Position correction
    const Vector correction = c.penetration * c.n;
    c.a->centerParticle.pos += (invMassA / sumInverseMasses) * correction;
    c.b->centerParticle.pos -= (invMassB / sumInverseMasses) * correction;

    // Cancel velocity along the normal
    const Vector vRel = c.a->centerParticle.vel - c.b->centerParticle.vel;
    const float vrn = vRel.dot(c.n);
    const float j = -vrn / sumInverseMasses;
    const Vector impulse = j * c.n;

    c.a->centerParticle.vel += invMassA * impulse;
    c.b->centerParticle.vel -= invMassB * impulse;
}

void CollisionResolver::resolveRodConstraint(Contact &c) const {
    const float invMassA = c.a->centerParticle.inverseMass;
    const float invMassB = c.b->centerParticle.inverseMass;
    const float sumInverseMasses = invMassA + invMassB;

    if (sumInverseMasses <= 0.0f) return;

    // Position correction
    const Vector correction = c.penetration * c.n;
    c.a->centerParticle.pos += (invMassA / sumInverseMasses) * correction;
    c.b->centerParticle.pos -= (invMassB / sumInverseMasses) * correction;

    // Velocity correction
    const Vector vRel = c.a->centerParticle.vel - c.b->centerParticle.vel;
    const float vrn = vRel.dot(c.n);

    // Remove all relative velocity along the rod (no bouncing)
    const float j = -vrn / sumInverseMasses;
    const Vector impulse = j * c.n;

    c.a->centerParticle.vel += invMassA * impulse;
    c.b->centerParticle.vel -= invMassB * impulse;
}

void CollisionResolver::resolveCableConstraint(Contact &c) const {
    const float invMassA = c.a->centerParticle.inverseMass;
    const float invMassB = c.b->centerParticle.inverseMass;
    const float sumInverseMasses = invMassA + invMassB;

    if (sumInverseMasses <= 0.0f) return;

    // Position correction
    const Vector correction = c.penetration * c.n;
    c.a->centerParticle.pos += (invMassA / sumInverseMasses) * correction;
    c.b->centerParticle.pos -= (invMassB / sumInverseMasses) * correction;

    // Velocity correction
    const Vector vRel = c.a->centerParticle.vel - c.b->centerParticle.vel;
    const float vrn = vRel.dot(c.n);

    if (vrn < 0.0f) {
        // Objects moving apart - stop the separation (with some elasticity)
        const float j = -(1.0f + params.restitution * 0.5f) * vrn / sumInverseMasses; // Less bouncy than collisions
        const Vector impulse = j * c.n;

        c.a->centerParticle.vel += invMassA * impulse;
        c.b->centerParticle.vel -= invMassB * impulse;
    }
}

void CollisionResolver::resolve(const std::vector<Actor *> &actors, const float frame_duration,
                                const ConstraintRegistry *constraintRegistry) const {
    // Get all basic shapes of world
    std::vector<Actor *> allActors;

    for (Actor *actor: actors) {
        if (actor->getShape() == BlobShape) {
            Blob &blob = dynamic_cast<Blob &>(*actor);
            for (Circle &circle: blob.circles) {
                allActors.push_back(&circle);
            }
            Circle *center = new Circle(blob.getCenter());
            allActors.push_back(center);
        } else {
            allActors.push_back(actor);
        }
    }

    // Lister les contacts
    std::vector<Contact> contacts;
    contacts.reserve(allActors.size() * 2);

    // Check for constraints
    if (constraintRegistry) {
        buildConstraintContacts(contacts, constraintRegistry);
    }

    // Check for regular collisions
    for (size_t i = 0; i < allActors.size(); ++i) {
        for (size_t j = i + 1; j < allActors.size(); ++j) {
            Contact c;
            if (buildContact(*allActors[i], *allActors[j], c, frame_duration)) {
                contacts.push_back(c);
            }
        }
    }

    for (auto &c: contacts) {
        resolveContact(c);
    }
}
