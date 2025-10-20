#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include "Actor.h"
#include "Circle.h"
#include "Rect.h"
#include "../Maths/Vector.h"

struct Contact {
    Actor *a;
    Actor *b;
    Vector n; // normale unitaire
    float penetration; // > 0 si interpénétration
};

struct CollisionResolverParams {
    float restitution = 0.2f; // coefficient d'elasticité : 0 = pas de rebond | 1 = rebond parfait
    float slop = 0.01f; // marge anti-jitter sur la correction de position : petit monde = 0.005 | grand monde = 0.5
    float restEps = 0.05f; // seuil de "repos" sur la vitesse normale relative
};

class CollisionResolver {
public:
    using Params = CollisionResolverParams;

    explicit CollisionResolver(Params p = {}) : params(p) {
    }

    // Résout la pénétration + applique l'impulsion pour toutes les paires en collision
    void resolve(const std::vector<Actor *> &actors, float frame_duration) const;

private:
    Params params;

    // Construction de contact selon les types
    static bool buildContact(Actor &a, Actor &b, Contact &out, float frame_duration);

    static bool buildC_C(Circle &A, Circle &B, Contact &c, float frame_duration);

    static bool buildC_R(Circle &C, Rect &R, Contact &c, float frame_duration);

    static bool buildR_R(Rect &A, Rect &B, Contact &c, float frame_duration);

    // Étapes de résolution
    void resolveContact(Contact &c) const;
};
