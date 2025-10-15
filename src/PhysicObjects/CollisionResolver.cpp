#include "CollisionResolver.h"
#include <limits>

// Helpers de type
static inline bool isCircle(Actor &a) { return a.getShape() == CircleShape; }
static inline bool isRect(Actor &a) { return a.getShape() == RectShape; }

// Accès génériques adaptés à Particle
float CollisionResolver::invMassOf(Actor &x) {
    if (isCircle(x)) return static_cast<Circle &>(x).centerParticle.inverseMass;
    return static_cast<Rect &>(x).centerParticle.inverseMass;
}

Vector &CollisionResolver::posOf(Actor &x) {
    if (isCircle(x)) return static_cast<Circle &>(x).centerParticle.pos;
    return static_cast<Rect &>(x).centerParticle.pos;
}

Vector &CollisionResolver::velOf(Actor &x) {
    if (isCircle(x)) return static_cast<Circle &>(x).centerParticle.vel;
    return static_cast<Rect &>(x).centerParticle.vel;
}

// Construction des contacts
bool CollisionResolver::buildC_C(Circle &A, Circle &B, Contact &c, const float frame_duration) {
    if (!A.collidesWith(B, frame_duration).collides) return false;

    const Vector d = B.centerParticle.pos - A.centerParticle.pos;
    const float dist2 = d.normSquared();
    const float r = A.radius + B.radius;

    if (dist2 <= 0.0f) return false;
    const float dist = std::sqrt(dist2);
    if (dist >= r) return false;

    c.a = &A;
    c.b = &B;
    c.n = (1.0f / dist) * d; // normale A->B
    c.penetration = r - dist;
    return true;
}

bool CollisionResolver::buildC_R(Circle &C, Rect &R, Contact &c, const float frame_duration) {
    const CollisionResult res = C.collidesWith(R, frame_duration);
    if (!res.collides) return false;

    // Recalcule de la profondeur
    const Vector d = C.centerParticle.pos - R.centerParticle.pos;
    const float x = d.dot(R.axisU);
    const float y = d.dot(R.axisV);

    const float cx = std::clamp(x, -R.halfA, R.halfA);
    const float cy = std::clamp(y, -R.halfB, R.halfB);

    const Vector closest = R.centerParticle.pos + cx * R.axisU + cy * R.axisV;
    const Vector delta = C.centerParticle.pos - closest;
    const float dist2 = delta.normSquared();

    c.a = &C;
    c.b = &R;
    c.n = res.normalVector;

    if (dist2 > 0.0f) {
        const float dist = std::sqrt(dist2);
        c.penetration = std::max(0.0f, C.radius - dist);
    } else {
        const float dx = R.halfA - std::fabs(x);
        const float dy = R.halfB - std::fabs(y);
        c.penetration = std::max(0.0f, C.radius + std::min(dx, dy));
    }
    return c.penetration > 0.0f;
}

bool CollisionResolver::buildR_R(Rect &A, Rect &B, Contact &c) {
    const Vector d = B.centerParticle.pos - A.centerParticle.pos;
    const Vector axes[4] = {A.axisU, A.axisV, B.axisU, B.axisV};
    constexpr float eps = 1e-8f;

    auto projectRadius = [](float a, float b, const Vector &u, const Vector &v, const Vector &L) -> float {
        return a * std::fabs(u.dot(L)) + b * std::fabs(v.dot(L));
    };

    float minOverlap = std::numeric_limits<float>::infinity();
    Vector bestAxisRaw{0.0f, 0.0f, 0.0f};

    for (const Vector &L: axes) {
        if (L.normSquared() <= eps) continue;

        const float r1 = projectRadius(A.halfA, A.halfB, A.axisU, A.axisV, L);
        const float r2 = projectRadius(B.halfA, B.halfB, B.axisU, B.axisV, L);
        const float t = d.dot(L);
        const float overlap = r1 + r2 - std::fabs(t);

        if (overlap <= 0.0f) return false;
        if (overlap < minOverlap) {
            minOverlap = overlap;
            bestAxisRaw = (t < 0.0f) ? -L : L; // orienter A -> B
        }
    }

    if (bestAxisRaw.normSquared() <= eps) return false;

    c.a = &A;
    c.b = &B;
    c.n = bestAxisRaw.normalized();
    c.penetration = minOverlap;
    return true;
}

bool CollisionResolver::buildContact(Actor &a, Actor &b, Contact &out, const float frame_duration) {
    const Shape sa = a.getShape();
    const Shape sb = b.getShape();

    if (sa == CircleShape && sb == CircleShape)
        return
                buildC_C(static_cast<Circle &>(a), static_cast<Circle &>(b), out, frame_duration);
    if (sa == CircleShape && sb == RectShape)
        return buildC_R(static_cast<Circle &>(a), static_cast<Rect &>(b), out,
                        frame_duration);

    if (sa == RectShape && sb == CircleShape) {
        Contact tmp;
        if (!buildC_R(static_cast<Circle &>(b), static_cast<Rect &>(a), tmp, frame_duration)) return false;
        out = {&a, &b, -tmp.n, tmp.penetration};
        return true;
    }

    if (sa == RectShape && sb == RectShape) return buildR_R(static_cast<Rect &>(a), static_cast<Rect &>(b), out);

    return false;
}

// Étapes de résolution
void CollisionResolver::positionalCorrection(Contact &c) const {
    const float invA = invMassOf(*c.a);
    const float invB = invMassOf(*c.b);
    const float invSum = invA + invB;
    if (invSum <= 0.0f) return;

    const float pen = std::max(0.0f, c.penetration - params.slop);
    if (pen <= 0.0f) return;

    const Vector corr = (pen / invSum) * c.n;

    posOf(*c.a) -= invA * corr;
    posOf(*c.b) += invB * corr;
}

void CollisionResolver::applyImpulse(Contact &c) const {
    const float invA = invMassOf(*c.a);
    const float invB = invMassOf(*c.b);
    const float invSum = invA + invB;
    if (invSum <= 0.0f) return;

    const Vector relV = velOf(*c.b) - velOf(*c.a);
    const float vrn = relV.dot(c.n); // vitesse relative projetée sur la normale

    // Repos / s'éloignent -> pas d'impulsion
    if (std::fabs(vrn) < params.restEps) return;
    if (vrn > 0.0f) return;

    const float e = std::clamp(params.restitution, 0.0f, 1.0f);
    const float j = -(1.0f + e) * vrn / invSum;

    const Vector impulse = j * c.n;

    velOf(*c.a) -= invA * impulse;
    velOf(*c.b) += invB * impulse;
}

void CollisionResolver::resolve(std::vector<Actor *> &actors, const float frame_duration) {
    // Lister les contacts
    std::vector<Contact> contacts;
    contacts.reserve(actors.size() * 2);

    for (size_t i = 0; i < actors.size(); ++i) {
        for (size_t j = i + 1; j < actors.size(); ++j) {
            Contact c;
            if (buildContact(*actors[i], *actors[j], c, frame_duration)) {
                contacts.push_back(c);
            }
        }
    }

    // Itérations : correction de position puis impulsion
    for (int it = 0; it < params.iterations; ++it) {
        for (auto &c: contacts) {
            positionalCorrection(c);
            applyImpulse(c);
        }
    }
}
