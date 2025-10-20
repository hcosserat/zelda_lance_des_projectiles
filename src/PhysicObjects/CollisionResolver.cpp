#include "CollisionResolver.h"

#include "Blob.h"

// Construction des contacts
bool CollisionResolver::buildC_C(Circle &A, Circle &B, Contact &c, const float frame_duration) {
    const CollisionResult res = A.collidesWith(B, frame_duration);
    if (!res.collides) return false;

    c.a = &A;
    c.b = &B;
    c.n = res.normalVector;
    c.penetration = res.penetration;
    return true;
}

bool CollisionResolver::buildC_R(Circle &C, Rect &R, Contact &c, const float frame_duration) {
    const CollisionResult res = C.collidesWith(R, frame_duration);
    if (!res.collides) return false;

    c.a = &C;
    c.b = &R;
    c.n = res.normalVector;
    c.penetration = res.penetration;
    return c.penetration > 0.0f;
}

bool CollisionResolver::buildR_R(Rect &A, Rect &B, Contact &c, const float frame_duration) {
    const CollisionResult res = A.collidesWith(B, frame_duration);
    if (!res.collides) return false;

    c.a = &A;
    c.b = &B;
    c.n = res.normalVector;
    c.penetration = res.penetration;
    return true;
}

bool CollisionResolver::buildContact(Actor &a, Actor &b, Contact &out, const float frame_duration) {
    const Shape sa = a.getShape();
    const Shape sb = b.getShape();

    if (sa == CircleShape && sb == CircleShape)
        return buildC_C(dynamic_cast<Circle &>(a), dynamic_cast<Circle &>(b), out, frame_duration);

    if (sa == CircleShape && sb == RectShape)
        return buildC_R(dynamic_cast<Circle &>(a), dynamic_cast<Rect &>(b), out, frame_duration);

    if (sa == RectShape && sb == CircleShape) {
        Contact tmp;
        if (!buildC_R(dynamic_cast<Circle &>(b), dynamic_cast<Rect &>(a), tmp, frame_duration)) return false;
        out = {&a, &b, -tmp.n, tmp.penetration};
        return true;
    }

    if (sa == RectShape && sb == RectShape)
        return buildR_R(dynamic_cast<Rect &>(a), dynamic_cast<Rect &>(b), out, frame_duration);

    return false;
}

void CollisionResolver::resolveContact(Contact &c) const {
    // std::cout << "=== Resolving Contact ===" << std::endl;
    // std::cout << "Initial penetration: " << c.penetration << std::endl;
    // std::cout << "Normal vector: (" << c.n.x << ", " << c.n.y << ")" << std::endl;

    const float invMassA = c.a->centerParticle.inverseMass;
    const float invMassB = c.b->centerParticle.inverseMass;

    // Check for infinite mass cases
    const bool aIsInfinite = invMassA == 0.0f;
    const bool bIsInfinite = invMassB == 0.0f;

    // Case: Both objects have infinite mass
    if (aIsInfinite && bIsInfinite) {
        // std::cout << "Both objects have infinite mass - no resolution" << std::endl;
        return;
    }

    // Case: Only A has infinite mass
    if (aIsInfinite) {
        // std::cout << "Object A has infinite mass" << std::endl;
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
        // std::cout << "Object B has infinite mass" << std::endl;
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
    // 1) Correction de position
    const float aMass = 1 / c.a->centerParticle.inverseMass;
    const float bMass = 1 / c.b->centerParticle.inverseMass;
    // std::cout << "Mass A: " << aMass << ", Mass B: " << bMass << std::endl;

    const float inverseSumMasses = 1 / (aMass + bMass);
    const auto inverseSumMassesMultDMultN = inverseSumMasses * c.penetration * c.n;

    // std::cout << "Position correction A: (" << (bMass * inverseSumMassesMultDMultN).x << ", "
    //        << (bMass * inverseSumMassesMultDMultN).y << ")" << std::endl;
    c.a->centerParticle.pos += bMass * inverseSumMassesMultDMultN;
    c.b->centerParticle.pos -= aMass * inverseSumMassesMultDMultN;

    // 2) Application des impulsions
    const auto vRel = c.a->centerParticle.vel - c.b->centerParticle.vel;
    // std::cout << "Relative velocity: (" << vRel.x << ", " << vRel.y << ")" << std::endl;

    const auto sumInverseMasses = c.a->centerParticle.inverseMass + c.b->centerParticle.inverseMass;
    const auto k = vRel.dot(c.n) * (params.restitution + 1) / sumInverseMasses;
    // std::cout << "Impulse magnitude k: " << k << std::endl;

    c.a->centerParticle.vel -= k * c.n * c.a->centerParticle.inverseMass;
    c.b->centerParticle.vel += k * c.n * c.b->centerParticle.inverseMass;

    // std::cout << "Final velocity A: (" << c.a->centerParticle.vel.x << ", " << c.a->centerParticle.vel.y << ")" <<
    //        std::endl;
    // std::cout << "Final velocity B: (" << c.b->centerParticle.vel.x << ", " << c.b->centerParticle.vel.y << ")" <<
    //        std::endl;
}

void CollisionResolver::resolve(const std::vector<Actor *> &actors, const float frame_duration) const {
    // Get all basic shapes of world
    std::vector<Actor *> allActors;

    for (Actor *actor: actors) {
        if (actor->getShape() == BlobShape) {
            Blob &blob = dynamic_cast<Blob &>(*actor);
            for (Circle &circle: blob.circles) {
                allActors.push_back(&circle);
            }
        } else {
            allActors.push_back(actor);
        }
    }

    // Lister les contacts
    std::vector<Contact> contacts;
    contacts.reserve(allActors.size() * 2);

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
