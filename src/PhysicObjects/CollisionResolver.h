#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include "Actor.h"
#include "Circle.h"
#include "Rect.h"
#include "../Maths/Vector.h"

struct Contact {
    Actor* a;
    Actor* b;
    Vector n;           // normale unitaire
    float  penetration; // > 0 si interpénétration
};

class CollisionResolver {
public:
    struct Params {
        float restitution = 0.2f; // coefficient d'elasticité : 0 = pas de rebond | 1 = rebond parfait
        float slop       = 0.01f; // marge anti-jitter sur la correction de position : petit monde = 0.005 | grand monde = 0.5
        float restEps    = 0.05f; // seuil de "repos" sur la vitesse normale relative
        int   iterations = 2;     // passes de résolution
    };

    explicit CollisionResolver(Params p = {}) : params(p) {}

    // Résout la pénétration + applique l'impulsion pour toutes les paires en collision
    void resolve(std::vector<Actor*>& actors);

private:
    Params params;

    // Construction de contact selon les types
    static bool buildContact(Actor& a, Actor& b, Contact& out);

    static bool buildC_C(Circle& A, Circle& B, Contact& c);
    static bool buildC_R(Circle& C, Rect&   R, Contact& c);
    static bool buildR_R(Rect&   A, Rect&   B, Contact& c);

    // Accès génériques au "corps" (via Particle)
    static float invMassOf(Actor& x);
    static Vector& posOf(Actor& x);
    static Vector& velOf(Actor& x);

    // Étapes de résolution
    void positionalCorrection(Contact& c) const;
    void applyImpulse(Contact& c) const;
};
